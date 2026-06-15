#include <ntddk.h>
#include <wdmsec.h>

#define DEVICE_NAME      L"\\Device\\VStorCache"
#define SYMLINK_NAME     L"\\DosDevices\\VStorCache"

#define IOCTL_VSTORCACHE_SET_ROOT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VSTORCACHE_FETCH    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _SET_SOURCE_REQUEST {
    ULONG PathChars;
    WCHAR Path[1];
} SET_SOURCE_REQUEST, *PSET_SOURCE_REQUEST;

typedef struct _READ_FILE_REQUEST {
    ULONG NameChars;
    WCHAR Name[260];
} READ_FILE_REQUEST, *PREAD_FILE_REQUEST;

typedef struct _VSTORCACHE_STATE {
    WCHAR Scratch[0x80];
    WCHAR SourcePath[0x100];
} VSTORCACHE_STATE, *PVSTORCACHE_STATE;

static VSTORCACHE_STATE g_State;

static SIZE_T VstorCacheWcsnlen(PCWSTR Value, SIZE_T MaxChars)
{
    SIZE_T i;

    for (i = 0; i < MaxChars; i++) {
        if (Value[i] == L'\0') {
            break;
        }
    }

    return i;
}

static VOID VstorCacheResetSource(VOID)
{
    static const WCHAR defaultPath[] = L"\\??\\C:\\ProgramData\\VStorCache\\cache";

    RtlZeroMemory(&g_State, sizeof(g_State));
    RtlCopyMemory(g_State.SourcePath, defaultPath, sizeof(defaultPath));
}

static NTSTATUS VstorCacheSetRoot(PVOID Buffer, ULONG InputLength)
{
    PSET_SOURCE_REQUEST req = (PSET_SOURCE_REQUEST)Buffer;
    UNICODE_STRING allowed;
    UNICODE_STRING candidate;
    USHORT bytes;
    ULONG copyBytes;

    if (InputLength < (ULONG)FIELD_OFFSET(SET_SOURCE_REQUEST, Path)) {
        return STATUS_INVALID_PARAMETER;
    }

    if (req->PathChars > 0x10000) {
        return STATUS_INVALID_PARAMETER;
    }

    /*
     * Intended bug: byte count is truncated to 16 bits before the bounds check,
     * but the copy uses the real client-supplied input size. A request can look
     * small to validation and still overflow Scratch into SourcePath.
     */
    bytes = (USHORT)((req->PathChars + 1) * sizeof(WCHAR));
    if (bytes > sizeof(g_State.Scratch)) {
        return STATUS_NAME_TOO_LONG;
    }

    if (InputLength < (ULONG)FIELD_OFFSET(SET_SOURCE_REQUEST, Path) + bytes) {
        return STATUS_BUFFER_TOO_SMALL;
    }

    RtlZeroMemory(g_State.Scratch, sizeof(g_State.Scratch));
    copyBytes = InputLength - FIELD_OFFSET(SET_SOURCE_REQUEST, Path);
    RtlCopyMemory(g_State.Scratch, req->Path, copyBytes);
    g_State.Scratch[RTL_NUMBER_OF(g_State.Scratch) - 1] = L'\0';

    RtlInitUnicodeString(&allowed, L"\\??\\C:\\ProgramData\\VStorCache\\cache");
    RtlInitUnicodeString(&candidate, g_State.Scratch);
    if (!RtlPrefixUnicodeString(&allowed, &candidate, TRUE)) {
        VstorCacheResetSource();
        return STATUS_ACCESS_DENIED;
    }

    if (copyBytes < sizeof(g_State.Scratch)) {
        RtlZeroMemory(g_State.SourcePath, sizeof(g_State.SourcePath));
        RtlCopyMemory(g_State.SourcePath, g_State.Scratch, candidate.Length + sizeof(WCHAR));
    }

    return STATUS_SUCCESS;
}

static NTSTATUS VstorCacheFetch(PVOID Buffer, ULONG InputLength, PIRP Irp)
{
    PIO_STACK_LOCATION sp = IoGetCurrentIrpStackLocation(Irp);
    PREAD_FILE_REQUEST req = (PREAD_FILE_REQUEST)Buffer;
    WCHAR fullPath[512];
    UNICODE_STRING path;
    OBJECT_ATTRIBUTES oa;
    IO_STATUS_BLOCK iosb;
    HANDLE file = NULL;
    NTSTATUS status;
    ULONG prefixChars;
    ULONG outMax;

    if (InputLength < sizeof(READ_FILE_REQUEST) || req->NameChars >= RTL_NUMBER_OF(req->Name)) {
        return STATUS_INVALID_PARAMETER;
    }

    req->Name[req->NameChars] = L'\0';
    prefixChars = (ULONG)VstorCacheWcsnlen(g_State.SourcePath, RTL_NUMBER_OF(g_State.SourcePath));

    if (prefixChars + 1 + req->NameChars >= RTL_NUMBER_OF(fullPath)) {
        return STATUS_NAME_TOO_LONG;
    }

    RtlCopyMemory(fullPath, g_State.SourcePath, prefixChars * sizeof(WCHAR));
    fullPath[prefixChars] = L'\\';
    RtlCopyMemory(fullPath + prefixChars + 1, req->Name, req->NameChars * sizeof(WCHAR));
    fullPath[prefixChars + 1 + req->NameChars] = L'\0';

    RtlInitUnicodeString(&path, fullPath);
    InitializeObjectAttributes(&oa, &path, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

    status = ZwCreateFile(
        &file,
        GENERIC_READ,
        &oa,
        &iosb,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ,
        FILE_OPEN,
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    outMax = sp->Parameters.DeviceIoControl.OutputBufferLength;
    if (outMax == 0) {
        ZwClose(file);
        return STATUS_BUFFER_TOO_SMALL;
    }

    status = ZwReadFile(file, NULL, NULL, NULL, &iosb, Buffer, outMax, NULL, NULL);
    ZwClose(file);

    if (NT_SUCCESS(status)) {
        Irp->IoStatus.Information = (ULONG_PTR)iosb.Information;
    }

    return status;
}

static NTSTATUS VstorCacheCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

static NTSTATUS VstorCacheDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PIO_STACK_LOCATION sp = IoGetCurrentIrpStackLocation(Irp);
    ULONG code = sp->Parameters.DeviceIoControl.IoControlCode;
    PVOID buffer = Irp->AssociatedIrp.SystemBuffer;
    ULONG inLen = sp->Parameters.DeviceIoControl.InputBufferLength;
    NTSTATUS status;

    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Information = 0;

    switch (code) {
    case IOCTL_VSTORCACHE_SET_ROOT:
        status = VstorCacheSetRoot(buffer, inLen);
        break;
    case IOCTL_VSTORCACHE_FETCH:
        status = VstorCacheFetch(buffer, inLen, Irp);
        break;
    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

static VOID VstorCacheUnload(PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING sym;

    RtlInitUnicodeString(&sym, SYMLINK_NAME);
    IoDeleteSymbolicLink(&sym);
    IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNICODE_STRING dev;
    UNICODE_STRING sym;
    UNICODE_STRING sddl;
    GUID classGuid = { 0xa8e2f5d1, 0x0b88, 0x4fd7, { 0x9a, 0x2b, 0x91, 0xe5, 0x51, 0x21, 0x42, 0x10 } };
    PDEVICE_OBJECT deviceObject = NULL;
    NTSTATUS status;

    UNREFERENCED_PARAMETER(RegistryPath);

    RtlZeroMemory(&g_State, sizeof(g_State));
    VstorCacheResetSource();

    RtlInitUnicodeString(&dev, DEVICE_NAME);
    RtlInitUnicodeString(&sddl, L"D:P(A;;GA;;;SY)(A;;GA;;;BA)(A;;GA;;;WD)(A;;GA;;;AC)(A;;GA;;;S-1-15-2-2)");
    status = IoCreateDeviceSecure(
        DriverObject,
        0,
        &dev,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &sddl,
        &classGuid,
        &deviceObject);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    RtlInitUnicodeString(&sym, SYMLINK_NAME);
    status = IoCreateSymbolicLink(&sym, &dev);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(deviceObject);
        return status;
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE] = VstorCacheCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = VstorCacheCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = VstorCacheDeviceControl;
    DriverObject->DriverUnload = VstorCacheUnload;

    deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    return STATUS_SUCCESS;
}

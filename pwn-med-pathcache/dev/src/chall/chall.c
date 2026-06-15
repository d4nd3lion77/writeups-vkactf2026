#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>

#define IOCTL_VSTORCACHE_SET_ROOT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VSTORCACHE_FETCH    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _READ_FILE_REQUEST {
    DWORD NameChars;
    WCHAR Name[260];
} READ_FILE_REQUEST;

static HANDLE g_in;
static HANDLE g_out;

static int write_all(const char *buf, DWORD len)
{
    DWORD done = 0;
    while (done < len) {
        DWORD n = 0;
        if (!WriteFile(g_out, buf + done, len - done, &n, NULL) || n == 0) {
            return -1;
        }
        done += n;
    }
    return 0;
}

static int write_text(const char *s)
{
    return write_all(s, (DWORD)strlen(s));
}

static int read_line(char *buf, DWORD cap)
{
    DWORD used = 0;
    while (used + 1 < cap) {
        char c;
        DWORD n = 0;
        if (!ReadFile(g_in, &c, 1, &n, NULL) || n == 0) {
            return -1;
        }
        if (c == '\n') {
            break;
        }
        if (c != '\r') {
            buf[used++] = c;
        }
    }
    buf[used] = 0;
    return (int)used;
}

static int hex_nibble(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

static int decode_hex(const char *line, int len, unsigned char *out, int cap)
{
    int used = 0;

    if (len & 1) {
        return -1;
    }

    for (int i = 0; i < len; i += 2) {
        int hi = hex_nibble(line[i]);
        int lo = hex_nibble(line[i + 1]);
        if (hi < 0 || lo < 0 || used >= cap) {
            return -1;
        }
        out[used++] = (unsigned char)((hi << 4) | lo);
    }

    return used;
}

int main(void)
{
    HANDLE dev;
    char line[8192];
    char out[4096];

    g_in = GetStdHandle(STD_INPUT_HANDLE);
    g_out = GetStdHandle(STD_OUTPUT_HANDLE);

    dev = CreateFileA("\\\\.\\VStorCache", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    write_text("storage cache\n1) update\n2) fetch\n> ");

    if (dev == INVALID_HANDLE_VALUE) {
        _snprintf_s(out, sizeof(out), _TRUNCATE, "driver open failed: %lu\n", GetLastError());
        write_text(out);
        return 1;
    }

    for (;;) {
        DWORD ret = 0;
        int n = read_line(line, sizeof(line));
        if (n <= 0) {
            break;
        }

        if (line[0] == '1') {
            unsigned char raw[4096];
            int rawLen;

            write_text("hex bytes: ");
            n = read_line(line, sizeof(line));
            if (n <= 0) {
                break;
            }

            rawLen = decode_hex(line, n, raw, sizeof(raw));
            if (rawLen < 0) {
                write_text("bad hex\n> ");
                continue;
            }

            if (DeviceIoControl(dev, IOCTL_VSTORCACHE_SET_ROOT, raw, rawLen, raw, rawLen, &ret, NULL)) {
                write_text("ok\n> ");
            } else {
                _snprintf_s(out, sizeof(out), _TRUNCATE, "err=%lu\n> ", GetLastError());
                write_text(out);
            }
        } else if (line[0] == '2') {
            READ_FILE_REQUEST req;

            ZeroMemory(&req, sizeof(req));
            write_text("name: ");
            n = read_line(line, sizeof(line));
            if (n <= 0 || n > 259) {
                write_text("bad name\n> ");
                continue;
            }

            req.NameChars = MultiByteToWideChar(CP_UTF8, 0, line, n, req.Name, 260);
            if (DeviceIoControl(dev, IOCTL_VSTORCACHE_FETCH, &req, sizeof(req), out, sizeof(out), &ret, NULL)) {
                write_all(out, ret);
                write_text("\n> ");
            } else {
                _snprintf_s(out, sizeof(out), _TRUNCATE, "err=%lu\n> ", GetLastError());
                write_text(out);
            }
        } else {
            break;
        }
    }

    CloseHandle(dev);
    return 0;
}


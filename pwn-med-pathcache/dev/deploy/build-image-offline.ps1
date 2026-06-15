param(
    [string]$WindowsIso = "$PSScriptRoot\SERVER_EVAL_x64FRE_en-us.iso",
    [string]$QemuRoot = "C:\Program Files\qemu",
    [UInt64]$DiskSizeBytes = 34359738368,
    [int]$ImageIndex = 1
)

$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
$ImageDir = Join-Path $PSScriptRoot "image"
$Vhdx = Join-Path $ImageDir "vstor_cache.vhdx"
$Qcow2 = Join-Path $ImageDir "vstor_cache.qcow2"
$Build = Join-Path $Root "build\x64\Release"
$QemuImg = Join-Path $QemuRoot "qemu-img.exe"

if (!(Test-Path $WindowsIso)) { throw "Missing Windows ISO: $WindowsIso" }
if (!(Test-Path $QemuImg)) { throw "Missing qemu-img: $QemuImg" }

foreach ($Name in @("VStorCache.sys", "VStorCache.inf", "VStorCacheTest.cer", "chall.exe")) {
    $Path = Join-Path $Build $Name
    if (!(Test-Path $Path)) { throw "Missing build artifact: $Path" }
}

$Launcher = Join-Path $Root "third_party\appjaillauncher-rs\target\release\appjaillauncher-rs.exe"
if (!(Test-Path $Launcher)) { throw "Missing AppJailLauncher: $Launcher" }

New-Item -ItemType Directory -Force $ImageDir | Out-Null
foreach ($Path in @($Vhdx, $Qcow2)) {
    if (Test-Path $Path) {
        $Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
        Move-Item -LiteralPath $Path -Destination "$Path.$Stamp.bak"
    }
}

$IsoImage = $null
$Vhd = $null
try {
    $IsoImage = Mount-DiskImage -ImagePath (Resolve-Path $WindowsIso).Path -PassThru
    $IsoVolume = $IsoImage | Get-Volume
    $Wim = Join-Path ($IsoVolume.DriveLetter + ":\") "sources\install.wim"
    if (!(Test-Path $Wim)) { throw "Missing install.wim in ISO" }

    New-VHD -Path $Vhdx -SizeBytes $DiskSizeBytes -Dynamic | Out-Null
    $Vhd = Mount-VHD -Path $Vhdx -PassThru
    $Disk = $Vhd | Get-Disk
    Initialize-Disk -Number $Disk.Number -PartitionStyle MBR
    $SystemPart = New-Partition -DiskNumber $Disk.Number -Size 500MB -IsActive
    $WindowsPart = New-Partition -DiskNumber $Disk.Number -UseMaximumSize
    Format-Volume -Partition $SystemPart -FileSystem NTFS -NewFileSystemLabel "System" -Confirm:$false | Out-Null
    Format-Volume -Partition $WindowsPart -FileSystem NTFS -NewFileSystemLabel "Windows" -Confirm:$false | Out-Null
    $SystemPart | Add-PartitionAccessPath -AssignDriveLetter
    $WindowsPart | Add-PartitionAccessPath -AssignDriveLetter
    $SystemLetter = (($SystemPart | Get-Volume).DriveLetter + ":")
    $WindowsLetter = (($WindowsPart | Get-Volume).DriveLetter + ":")

    dism /English /Apply-Image /ImageFile:$Wim /Index:$ImageIndex /ApplyDir:$WindowsLetter\
    if ($LASTEXITCODE -ne 0) { throw "DISM Apply-Image failed with exit code $LASTEXITCODE" }
    bcdboot "$WindowsLetter\Windows" /s $SystemLetter /f BIOS
    if ($LASTEXITCODE -ne 0) { throw "bcdboot failed with exit code $LASTEXITCODE" }

    $ChallengeDir = Join-Path $WindowsLetter "ProgramData\VStorCache"
    $DriverDir = Join-Path $WindowsLetter "Windows\System32\drivers"
    $PantherDir = Join-Path $WindowsLetter "Windows\Panther"
    $SetupScripts = Join-Path $WindowsLetter "Windows\Setup\Scripts"
    New-Item -ItemType Directory -Force $ChallengeDir, $PantherDir, $SetupScripts | Out-Null
    Copy-Item (Join-Path $Root "guest\setup.ps1") (Join-Path $ChallengeDir "setup.ps1") -Force
    Copy-Item (Join-Path $Root "guest\VStorSvcTask.xml") (Join-Path $ChallengeDir "VStorSvcTask.xml") -Force
    Copy-Item (Join-Path $Root "guest\sync-flag.ps1") (Join-Path $ChallengeDir "sync-flag.ps1") -Force
    Copy-Item (Join-Path $Root "guest\FlagSyncTask.xml") (Join-Path $ChallengeDir "FlagSyncTask.xml") -Force
    Copy-Item (Join-Path $Root "guest\Unattend.offline.xml") (Join-Path $PantherDir "Unattend.xml") -Force
    Copy-Item (Join-Path $Root "guest\SetupComplete.cmd") (Join-Path $SetupScripts "SetupComplete.cmd") -Force
    Copy-Item (Join-Path $Build "VStorCache.sys") (Join-Path $DriverDir "VStorCache.sys") -Force
    Copy-Item (Join-Path $Build "VStorCache.sys") (Join-Path $ChallengeDir "VStorCache.sys") -Force
    Copy-Item (Join-Path $Build "VStorCache.inf") (Join-Path $ChallengeDir "VStorCache.inf") -Force
    Copy-Item (Join-Path $Build "VStorCacheTest.cer") (Join-Path $ChallengeDir "VStorCacheTest.cer") -Force
    Copy-Item (Join-Path $Build "chall.exe") (Join-Path $ChallengeDir "chall.exe") -Force
    Copy-Item $Launcher (Join-Path $ChallengeDir "appjaillauncher-rs.exe") -Force
} finally {
    if ($Vhd -ne $null) { Dismount-VHD -Path $Vhdx }
    if ($IsoImage -ne $null) { Dismount-DiskImage -ImagePath (Resolve-Path $WindowsIso).Path }
}

& $QemuImg convert -p -O qcow2 $Vhdx $Qcow2
if ($LASTEXITCODE -ne 0) { throw "qemu-img convert failed with exit code $LASTEXITCODE" }
Write-Host "Built qcow2 image: $Qcow2"

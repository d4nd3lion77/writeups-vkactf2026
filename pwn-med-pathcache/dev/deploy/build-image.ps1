param(
    [string]$WindowsIso = "$PSScriptRoot\SERVER_EVAL_x64FRE_en-us.iso",
    [string]$QemuRoot = "C:\Program Files\qemu",
    [string]$DiskSize = "32G",
    [string]$Display = "none",
    [string]$Accel = "whpx",
    [string]$Cpu = "qemu64"
)

$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
$ImageDir = Join-Path $PSScriptRoot "image"
$Image = Join-Path $ImageDir "vstor_cache.qcow2"
$SeedDir = Join-Path $ImageDir "seed"
$Payload = Join-Path $SeedDir "payload"
$Build = Join-Path $Root "build\x64\Release"
$Unattend = Join-Path $Root "guest\Autounattend.servercore.xml"
$Qemu = Join-Path $QemuRoot "qemu-system-x86_64.exe"
$QemuImg = Join-Path $QemuRoot "qemu-img.exe"

if (!(Test-Path $WindowsIso)) { throw "Missing Windows ISO: $WindowsIso" }
foreach ($Path in @($Qemu, $QemuImg, $Unattend)) {
    if (!(Test-Path $Path)) { throw "Missing dependency: $Path" }
}

foreach ($Name in @("VStorCache.sys", "VStorCache.inf", "VStorCacheTest.cer", "chall.exe")) {
    $Path = Join-Path $Build $Name
    if (!(Test-Path $Path)) { throw "Missing build artifact: $Path" }
}

$Launcher = Join-Path $Root "third_party\appjaillauncher-rs\target\release\appjaillauncher-rs.exe"
if (!(Test-Path $Launcher)) { throw "Missing AppJailLauncher: $Launcher" }

if (Test-Path $SeedDir) { Remove-Item -LiteralPath $SeedDir -Recurse -Force }
New-Item -ItemType Directory -Force $Payload | Out-Null
Copy-Item $Unattend (Join-Path $SeedDir "Autounattend.xml") -Force
Copy-Item (Join-Path $Root "guest\setup.ps1") (Join-Path $SeedDir "setup.ps1") -Force
Copy-Item (Join-Path $Root "guest\VStorSvcTask.xml") (Join-Path $SeedDir "VStorSvcTask.xml") -Force
Copy-Item (Join-Path $Root "guest\sync-flag.ps1") (Join-Path $SeedDir "sync-flag.ps1") -Force
Copy-Item (Join-Path $Root "guest\FlagSyncTask.xml") (Join-Path $SeedDir "FlagSyncTask.xml") -Force
Copy-Item (Join-Path $Build "VStorCache.sys") $Payload -Force
Copy-Item (Join-Path $Build "VStorCache.inf") $Payload -Force
Copy-Item (Join-Path $Build "VStorCacheTest.cer") $Payload -Force
Copy-Item (Join-Path $Build "chall.exe") $Payload -Force
Copy-Item $Launcher $Payload -Force

if (Test-Path $Image) {
    $Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
    Move-Item -LiteralPath $Image -Destination (Join-Path $ImageDir "vstor_cache.$Stamp.qcow2")
}

$ImageQemuPath = "image/vstor_cache.qcow2"
$SeedQemuPath = "image/seed"
$IsoFull = (Resolve-Path $WindowsIso).Path
$DeployFull = (Resolve-Path $PSScriptRoot).Path
if ($IsoFull.StartsWith($DeployFull, [System.StringComparison]::OrdinalIgnoreCase)) {
    $IsoQemuPath = ($IsoFull.Substring($DeployFull.Length).TrimStart("\") -replace "\\", "/")
} else {
    $IsoQemuPath = $IsoFull
}

Push-Location $PSScriptRoot
try {
    & $QemuImg create -f qcow2 $ImageQemuPath $DiskSize
    if ($LASTEXITCODE -ne 0) { throw "qemu-img failed with exit code $LASTEXITCODE" }
    & $Qemu -m 4096 -smp 2 -accel $Accel -cpu $Cpu `
        -drive "file=$ImageQemuPath,if=ide,format=qcow2,cache=writeback" `
        -cdrom $IsoQemuPath `
        -fda "fat:floppy:rw:$SeedQemuPath" `
        -boot "order=d" `
        -device "e1000,netdev=n0" `
        -netdev "user,id=n0" `
        -display $Display `
        -serial "mon:stdio"
    if ($LASTEXITCODE -ne 0) { throw "qemu exited with code $LASTEXITCODE" }
} finally {
    Pop-Location
}

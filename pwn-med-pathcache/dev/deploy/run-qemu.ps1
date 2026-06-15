param(
    [string]$Image = "$PSScriptRoot\image\vstor_cache.qcow2",
    [string]$QemuRoot = "C:\Program Files\qemu",
    [string]$Display = "none",
    [string]$Accel = "whpx",
    [string]$Cpu = "qemu64",
    [int]$HostPort = 31337
)

$ErrorActionPreference = "Stop"

$Qemu = Join-Path $QemuRoot "qemu-system-x86_64.exe"
if (!(Test-Path $Qemu)) {
    throw "Missing QEMU: $Qemu"
}

if (!(Test-Path $Image)) {
    throw "Missing image: $Image"
}

$DeployFull = (Resolve-Path $PSScriptRoot).Path
$ImageFull = (Resolve-Path $Image).Path
if ($ImageFull.StartsWith($DeployFull, [System.StringComparison]::OrdinalIgnoreCase)) {
    $ImageQemuPath = ($ImageFull.Substring($DeployFull.Length).TrimStart("\") -replace "\\", "/")
} else {
    $ImageQemuPath = $ImageFull
}

Push-Location $PSScriptRoot
try {
    & $Qemu `
        -m 4096 `
        -smp 2 `
        -accel $Accel `
        -cpu $Cpu `
        -drive "file=$ImageQemuPath,if=ide,format=qcow2,cache=writeback" `
        -netdev "user,id=n0,hostfwd=tcp::$HostPort-:31337" `
        -device "e1000,netdev=n0" `
        -display $Display `
        -serial "mon:stdio"
} finally {
    Pop-Location
}


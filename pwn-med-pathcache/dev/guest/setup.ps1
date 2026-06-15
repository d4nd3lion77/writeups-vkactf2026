$ErrorActionPreference = "Stop"

function Copy-IfDifferent {
    param(
        [string]$Source,
        [string]$Destination
    )

    if ((Resolve-Path $Source).Path -ne (Resolve-Path $Destination -ErrorAction SilentlyContinue).Path) {
        Copy-Item $Source $Destination -Force
    }
}

$LocalRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$SeedRoot = $LocalRoot

if (!(Test-Path (Join-Path $SeedRoot "VStorCache.sys")) -and !(Test-Path C:\Windows\System32\drivers\VStorCache.sys)) {
    $SeedRoot = Get-PSDrive -PSProvider FileSystem |
        ForEach-Object { Join-Path $_.Root "payload" } |
        Where-Object { Test-Path (Join-Path $_ "VStorCache.sys") } |
        Select-Object -First 1
}

if ($null -eq $SeedRoot -or (!(Test-Path (Join-Path $SeedRoot "VStorCache.sys")) -and !(Test-Path C:\Windows\System32\drivers\VStorCache.sys))) {
    throw "Cannot find provisioning payload"
}

New-Item -ItemType Directory -Force C:\ProgramData\VStorCache\cache | Out-Null
Set-Content C:\ProgramData\VStorCache\cache\hello.txt "hello from public storage"
Set-Content C:\flag.txt "vkactf{dummy_flag}"
icacls C:\flag.txt /inheritance:r
icacls C:\flag.txt /grant "NT AUTHORITY\SYSTEM:R"
icacls C:\flag.txt /remove "Users" "Authenticated Users" "Everyone"

if (Test-Path (Join-Path $SeedRoot "VStorCache.sys")) {
    Copy-Item (Join-Path $SeedRoot "VStorCache.sys") C:\Windows\System32\drivers\VStorCache.sys -Force
}
Copy-IfDifferent (Join-Path $SeedRoot "VStorCache.inf") C:\ProgramData\VStorCache\VStorCache.inf
Copy-IfDifferent (Join-Path $SeedRoot "VStorCacheTest.cer") C:\ProgramData\VStorCache\VStorCacheTest.cer
Copy-IfDifferent (Join-Path $SeedRoot "chall.exe") C:\ProgramData\VStorCache\chall.exe
Copy-IfDifferent (Join-Path $SeedRoot "appjaillauncher-rs.exe") C:\ProgramData\VStorCache\appjaillauncher-rs.exe
Copy-IfDifferent (Join-Path $SeedRoot "VStorSvcTask.xml") C:\ProgramData\VStorCache\VStorSvcTask.xml
New-Item -ItemType Directory -Force C:\ProgramData\VStorCache\mazes | Out-Null

certutil.exe -addstore -f Root C:\ProgramData\VStorCache\VStorCacheTest.cer
certutil.exe -addstore -f TrustedPublisher C:\ProgramData\VStorCache\VStorCacheTest.cer

bcdedit.exe /set testsigning on

sc.exe create VStorCache type= kernel binPath= C:\Windows\System32\drivers\VStorCache.sys start= auto
schtasks.exe /Create /TN VStorSvc /XML C:\ProgramData\VStorCache\VStorSvcTask.xml /F

netsh advfirewall firewall add rule name="VStorSvc" dir=in action=allow protocol=TCP localport=31337

New-Item -ItemType File -Force C:\ProgramData\VStorCache\provisioned.txt | Out-Null

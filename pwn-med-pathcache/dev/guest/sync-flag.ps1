$ErrorActionPreference = "Stop"

$Destination = "C:\flag.txt"
$FallbackFlag = "VKACTF{test_flag_replace_me}"
$HttpFlag = "http://10.0.2.2:8080/flag.txt"

try {
    Invoke-WebRequest -Uri $HttpFlag -OutFile $Destination -UseBasicParsing -TimeoutSec 5
} catch {
    $Source = Get-PSDrive -PSProvider FileSystem |
        ForEach-Object { Join-Path $_.Root "flag.txt" } |
        Where-Object {
            (Test-Path $_) -and
            ([System.IO.Path]::GetFullPath($_) -ne [System.IO.Path]::GetFullPath($Destination))
        } |
        Select-Object -First 1

    if ($null -ne $Source) {
        Copy-Item $Source $Destination -Force
    } elseif (!(Test-Path $Destination)) {
        Set-Content $Destination $FallbackFlag
    }
}

icacls $Destination /inheritance:r | Out-Null
icacls $Destination /grant "NT AUTHORITY\SYSTEM:R" | Out-Null
icacls $Destination /remove "Users" "Authenticated Users" "Everyone" | Out-Null

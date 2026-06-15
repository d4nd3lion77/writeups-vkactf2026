param(
    [string]$Configuration = "Release",
    [string]$Platform = "x64",
    [string]$KitVersion = "10.0.22621.0",
    [string]$CertName = "VStorCache Test Certificate"
)

$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$Out = Join-Path $Root "build\$Platform\$Configuration"
$DriverSys = Join-Path $Out "VStorCache.sys"
$CertOut = Join-Path $Out "VStorCacheTest.cer"
$SignTool = "C:\Program Files (x86)\Windows Kits\10\bin\$KitVersion\x64\signtool.exe"

if (!(Test-Path $DriverSys)) {
    throw "Missing driver: $DriverSys. Run .\build.ps1 first."
}

if (!(Test-Path $SignTool)) {
    throw "Missing signtool: $SignTool"
}

$Cert = Get-ChildItem Cert:\CurrentUser\My | Where-Object { $_.Subject -eq "CN=$CertName" } | Select-Object -First 1
if ($null -eq $Cert) {
    $Cert = New-SelfSignedCertificate `
        -Type CodeSigningCert `
        -Subject "CN=$CertName" `
        -CertStoreLocation Cert:\CurrentUser\My `
        -KeyExportPolicy Exportable `
        -KeyUsage DigitalSignature `
        -NotAfter (Get-Date).AddYears(5)
}

& $SignTool sign /v /fd SHA256 /s My /n "$CertName" "$DriverSys"
if ($LASTEXITCODE -ne 0) {
    throw "signtool failed with exit code $LASTEXITCODE"
}

Export-Certificate -Cert $Cert -FilePath $CertOut | Out-Null
Write-Host "Signed $DriverSys"
Write-Host "Exported certificate to $CertOut"


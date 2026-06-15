$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

Push-Location $PSScriptRoot
try {
    if (Test-Path .\dist) {
        Remove-Item .\dist -Recurse -Force
    }

    if (Test-Path .\payload.zip) {
        Remove-Item .\payload.zip -Force
    }
    Compress-Archive -Path .\payload\* -DestinationPath .\payload.zip

    dotnet publish .\Launcher.csproj -c Release -r win-x64 -p:PublishSingleFile=true -p:SelfContained=true -o .\dist
    if ($LASTEXITCODE -ne 0) {
        throw "dotnet publish failed with exit code $LASTEXITCODE"
    }

    $releaseDir = Join-Path $PSScriptRoot "..\release"
    if (Test-Path $releaseDir) {
        Remove-Item $releaseDir -Recurse -Force
    }
    New-Item -ItemType Directory -Path $releaseDir | Out-Null

    Copy-Item .\dist\Message.exe (Join-Path $releaseDir "Message.exe") -Force

    Write-Host "Done: $releaseDir"
}
finally {
    Pop-Location
}

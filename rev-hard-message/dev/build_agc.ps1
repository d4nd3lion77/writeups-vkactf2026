$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

Push-Location $PSScriptRoot
try {
    $repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..\..\..")
    $yaYul = Join-Path $repoRoot "yaYUL\yaYUL.exe"
    if (-not (Test-Path $yaYul)) { $yaYul = Join-Path $repoRoot "yaYUL\yaYUL" }
    if (-not (Test-Path $yaYul)) { throw "yaYUL not found" }

    cmd /c "`"$yaYul`" Message.agc > Message.agc.lst"
    if ($LASTEXITCODE -ne 0) { throw "yaYUL failed: $LASTEXITCODE" }

    if (-not (Test-Path .\Message.agc.bin) -or -not (Test-Path .\Message.agc.symtab)) {
        throw "AGC output not found"
    }

    Copy-Item .\Message.agc.bin .\payload\Message.agc.bin -Force
    Copy-Item .\Message.agc.symtab .\payload\Message.agc.symtab -Force

    Write-Host "AGC build ok: Message.agc.bin / Message.agc.symtab"
}
finally {
    Pop-Location
}



param(
    [int]$Iterations = 120
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

Push-Location $PSScriptRoot
try {
    if (-not (Test-Path .\Message.agc.bin) -or -not (Test-Path .\Message.agc.symtab)) {
        throw "Build first: .\build_agc.ps1"
    }

    $agcExe = Resolve-Path .\payload\yaAGC.exe
    if (-not (Test-Path $agcExe)) {
        throw "payload\yaAGC.exe not found"
    }

    $cmdFile = Join-Path $PSScriptRoot "agc_console_cmds.txt"
    $rawOut = Join-Path $PSScriptRoot "agc_console_raw.txt"

    $lines = New-Object System.Collections.Generic.List[string]
    $lines.Add('break SNAP')
    $lines.Add('run')
    for ($i = 0; $i -lt $Iterations; $i++) {
        $lines.Add('print ERRNUM')
        $lines.Add('print ERRSUB')
        $lines.Add('cont')
    }
    $lines.Add('quit')
    Set-Content -Path $cmdFile -Value $lines -Encoding ascii

    cmd /c "`"$agcExe`" Message.agc.bin --symbols=Message.agc.symtab --no-resume --dump-time=0 < `"$cmdFile`" > `"$rawOut`""

    $vals = @()
    foreach ($line in Get-Content $rawOut) {
        if ($line -match '^\(agc\) \$1 = ([0-7]+)$') {
            $vals += $Matches[1]
        }
    }

    if ($vals.Count -lt 2) {
        throw "No values parsed. Check agc_console_raw.txt"
    }

    $step = 0
    for ($i = 0; $i + 1 -lt $vals.Count; $i += 2) {
        $prog = $vals[$i].PadLeft(2, '0')
        $noun = $vals[$i + 1].PadLeft(2, '0')
        "step={0:D4}  PROG={1}  NOUN={2}" -f $step, $prog, $noun
        $step++
    }
}
finally {
    Pop-Location
}

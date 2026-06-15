param(
    [string]$Configuration = "Release",
    [string]$Platform = "x64",
    [string]$KitVersion = "10.0.22621.0",
    [string]$VsRoot = "C:\Program Files\Microsoft Visual Studio\2022\Community"
)

$ErrorActionPreference = "Stop"

if ($Platform -ne "x64") {
    throw "Only x64 is supported for now."
}

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$Out = Join-Path $Root "build\$Platform\$Configuration"
$Obj = Join-Path $Out "obj"
$VcVars = Join-Path $VsRoot "VC\Auxiliary\Build\vcvars64.bat"
$KitRoot = "C:\Program Files (x86)\Windows Kits\10"
$KmInclude = Join-Path $KitRoot "Include\$KitVersion\km"
$SharedInclude = Join-Path $KitRoot "Include\$KitVersion\shared"
$UcrtInclude = Join-Path $KitRoot "Include\$KitVersion\ucrt"
$UmInclude = Join-Path $KitRoot "Include\$KitVersion\um"
$KmLib = Join-Path $KitRoot "Lib\$KitVersion\km\x64"
$UmLib = Join-Path $KitRoot "Lib\$KitVersion\um\x64"
$UcrtLib = Join-Path $KitRoot "Lib\$KitVersion\ucrt\x64"

foreach ($Path in @($VcVars, $KmInclude, $SharedInclude, $KmLib, $UmLib, $UcrtLib)) {
    if (!(Test-Path $Path)) {
        throw "Missing build dependency: $Path"
    }
}

New-Item -ItemType Directory -Force $Obj | Out-Null

$DriverSrc = Join-Path $Root "src\driver\VStorCache.c"
$DriverObj = Join-Path $Obj "VStorCache.obj"
$DriverSys = Join-Path $Out "VStorCache.sys"
$ChallSrc = Join-Path $Root "src\chall\chall.c"
$ChallExe = Join-Path $Out "chall.exe"
$DriverBuildCmd = Join-Path $Obj "build_driver.cmd"
$ChallBuildCmd = Join-Path $Obj "build_chall.cmd"

function Write-BuildCmd {
    param(
        [string]$Path,
        [string[]]$Lines
    )

    $Body = "@echo off`r`nchcp 65001 >nul`r`n" + (($Lines | ForEach-Object {
        "$_`r`nif errorlevel 1 exit /b %errorlevel%"
    }) -join "`r`n")
    $Utf8NoBom = New-Object System.Text.UTF8Encoding($false)
    [System.IO.File]::WriteAllText($Path, $Body, $Utf8NoBom)
}

Write-BuildCmd $DriverBuildCmd @(
    "call `"$VcVars`" >nul",
    "cl /nologo /c /Zi /W4 /WX- /Gz /kernel /GS- /GR- /D_AMD64_=1 /DWIN64=1 /D_WIN64=1 /D_KERNEL_MODE=1 /D_NTDRIVER_=1 /DWINNT=1 /D_WIN32_WINNT=0x0A00 /DNTDDI_VERSION=0x0A000008 /I`"$KmInclude`" /I`"$SharedInclude`" /Fo`"$DriverObj`" `"$DriverSrc`"",
    "link /nologo /debug /driver /subsystem:native /entry:DriverEntry /nodefaultlib /out:`"$DriverSys`" /libpath:`"$KmLib`" `"$DriverObj`" ntoskrnl.lib hal.lib BufferOverflowK.lib wdmsec.lib"
)
cmd.exe /d /s /c "`"$DriverBuildCmd`""
if ($LASTEXITCODE -ne 0) {
    throw "Driver build failed with exit code $LASTEXITCODE"
}

Write-BuildCmd $ChallBuildCmd @(
    "call `"$VcVars`" >nul",
    "cl /nologo /Zi /W4 /O2 /MT /D_CRT_SECURE_NO_WARNINGS /I`"$UmInclude`" /I`"$SharedInclude`" /I`"$UcrtInclude`" `"$ChallSrc`" /Fe`"$ChallExe`" /link /libpath:`"$UmLib`" /libpath:`"$UcrtLib`""
)
cmd.exe /d /s /c "`"$ChallBuildCmd`""
if ($LASTEXITCODE -ne 0) {
    throw "chall.exe build failed with exit code $LASTEXITCODE"
}

Copy-Item (Join-Path $Root "src\driver\VStorCache.inf") (Join-Path $Out "VStorCache.inf") -Force
Write-Host "Built artifacts in $Out"

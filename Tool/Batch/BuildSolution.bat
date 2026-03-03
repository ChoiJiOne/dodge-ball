@echo off

set BUILD_TYPE=%~1
set IS_REBUILD=%~2
if "%IS_REBUILD%"=="" (
    set IS_REBUILD=false
)

if /I "%IS_REBUILD%"=="true" (
    set IS_REBUILD=true
) else if /I "%IS_REBUILD%"=="false" (
    set IS_REBUILD=false
) else (
    echo [ERROR] Invalid argument defined!
    echo Usage: %~nx0 ^<BuildType^> [true^|false]
    echo Example: %~nx0 Debug true
    exit /b 1
)

set SCRIPT_DIR=%~dp0..\Script\
set SOLUTION_DIR=%~dp0..\..\Solution
set TEMP_DIR=%~dp0..\..\Temp

if not exist "%TEMP_DIR%" (
    mkdir "%TEMP_DIR%"
)

python "%SCRIPT_DIR%cli.py" build-solution ^
    --solution-path "%SOLUTION_DIR%" ^
    --config "%BUILD_TYPE%" ^
    --log-file-path "%TEMP_DIR%" ^
    --is-rebuild %IS_REBUILD%

pause

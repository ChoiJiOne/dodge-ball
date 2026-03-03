@echo off
setlocal

set BUILD_TYPE=%~1
if "%BUILD_TYPE%"=="" (
    set BUILD_TYPE=Debug
)

set EXE_DIR=%~dp0..\..\Solution\%BUILD_TYPE%
set EXE_PATH=%EXE_DIR%\DataChunkGenerator.exe

if not exist "%EXE_PATH%" (
    echo [ERROR] DataChunkGenerator.exe not found!
    echo Build Type: %BUILD_TYPE%
    echo Expected path: %EXE_PATH%
    echo Please build the project first.
    exit /b 1
)

pushd "%~dp0..\..\"

echo [INFO] Running DataChunkGenerator (%BUILD_TYPE%)...
"%EXE_PATH%"
set RUN_ERRORLEVEL=%ERRORLEVEL%

popd

if %RUN_ERRORLEVEL% neq 0 (
    echo [ERROR] DataChunkGenerator failed with error code %RUN_ERRORLEVEL%.
    exit /b %RUN_ERRORLEVEL%
)

echo [INFO] DataChunkGenerator finished successfully.
endlocal

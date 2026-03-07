@echo off

set SCRIPT_DIR=%~dp0..\Script\
set XLSX_DIR=%~dp0..\..\XLSX
set CSV_OUT_DIR=%~dp0..\..\CSV
set TEMP_DIR=%~dp0..\..\Temp

if not exist "%TEMP_DIR%" (
    mkdir "%TEMP_DIR%"
)
if not exist "%CSV_OUT_DIR%" (
    mkdir "%CSV_OUT_DIR%"
)

if "%~1" == "" (
    echo [ERROR] Target name is required!
    echo Usage: ConvertXLSXToCSV.bat ^<TargetName^>
    pause
    exit /b 1
)
set TARGET_NAME=%~1

python "%SCRIPT_DIR%cli.py" convert-xlsx-to-csv ^
    --target-xlsx-path "%XLSX_DIR%" ^
    --target-name "%TARGET_NAME%" ^
    --output-csv-path "%CSV_OUT_DIR%" ^
    --log-file-path "%TEMP_DIR%"

pause

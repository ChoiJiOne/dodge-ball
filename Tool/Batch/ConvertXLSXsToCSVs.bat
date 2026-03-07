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

python "%SCRIPT_DIR%cli.py" convert-xlsxs-to-csvs ^
    --target-xlsx-path "%XLSX_DIR%" ^
    --output-csv-path "%CSV_OUT_DIR%" ^
    --log-file-path "%TEMP_DIR%"

pause

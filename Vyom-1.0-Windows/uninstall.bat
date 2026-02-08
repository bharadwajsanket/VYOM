@echo off
setlocal EnableDelayedExpansion
title Vyom Uninstaller v1.0
color 0C

:: =====================================================
:: VYOM UNINSTALLER v1.0
:: =====================================================

cls
echo.
echo  =============================================
echo.
echo       VYOM UNINSTALLER
echo       Clean Removal - No Leftovers
echo.
echo  =============================================
echo.

:: ---------------- CONFIRM ----------------
echo  [?] Are you sure you want to uninstall Vyom?
echo      This will remove Vyom v1.0 and all examples.
echo.
set /p "confirm=      Type Y to continue: "
if /i not "%confirm%"=="Y" (
    echo.
    echo  Uninstall cancelled.
    echo.
    pause
    exit /b 0
)

echo.
echo  Uninstalling...
echo.

:: ---------------- PATHS ----------------
set "INSTALL_DIR=C:\Vyom"

:: Step 1: Check Install
echo  [1/3] Checking installation...
if not exist "%INSTALL_DIR%" (
    echo        Vyom is not installed at %INSTALL_DIR%
    goto :cleanup_path
)
echo        Found installation at %INSTALL_DIR%

:: Step 2: Remove Files
echo  [2/3] Removing files...
if exist "%INSTALL_DIR%\vyom.exe" del /Q "%INSTALL_DIR%\vyom.exe" >nul 2>&1
if exist "%INSTALL_DIR%\examples" rmdir /S /Q "%INSTALL_DIR%\examples" >nul 2>&1
rmdir /S /Q "%INSTALL_DIR%" >nul 2>&1
echo        Removed C:\Vyom

:cleanup_path
:: Step 3: Clean PATH
echo  [3/3] Cleaning System PATH...
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% equ 0 (
    powershell -Command ^
    "$p=[Environment]::GetEnvironmentVariable('Path','Machine'); ^
     $n=($p.Split(';') | Where-Object { $_ -ne 'C:\Vyom' -and $_ -ne '' }) -join ';'; ^
     [Environment]::SetEnvironmentVariable('Path',$n,'Machine')" >nul 2>&1
    if %errorlevel% equ 0 (
        echo        Removed C:\Vyom from PATH
    ) else (
        echo        [WARNING] Could not update PATH automatically
    )
) else (
    echo        PATH does not contain C:\Vyom
)

echo.
echo  =============================================
echo.
echo  SUCCESS! Vyom has been removed from your system.
echo.
echo  Removed Items:
echo    - Installation Directory (C:\Vyom)
echo    - Environment Variable (PATH)
echo    - Example Files
echo.
echo  =============================================
echo.
echo  Thank you for using Vyom.
echo.
pause
exit /b 0

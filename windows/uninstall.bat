@echo off
title Vyom Uninstaller v0.7
color 0C
setlocal

:: =====================================================
:: Vyom Uninstaller v0.7
:: =====================================================

cls
echo.
echo =====================================================
echo   VYOM PROGRAMMING LANGUAGE
echo.
echo   v0.7 — Uninstaller
echo   Clean • Explicit • No Leftovers
echo.
echo   Created by Sanket Bharadwaj
echo =====================================================
echo.

:: ---------------- CONFIRM ----------------
color 0E
choice /C YN /M "Are you sure you want to uninstall Vyom?"
if errorlevel 2 (
    echo.
    echo Uninstallation cancelled.
    pause
    goto :end
)
color 0C

:: ---------------- PATH ----------------
set INSTALL_DIR=C:\Vyom

:: ---------------- STEP 1 ----------------
echo [1/4] Checking installation...
if not exist "%INSTALL_DIR%" (
    echo     Vyom is not installed
    goto :cleanup_path
)
echo     Installation found
echo.

:: ---------------- STEP 2 ----------------
echo [2/4] Removing files...
if exist "%INSTALL_DIR%\vyom.exe" del /Q "%INSTALL_DIR%\vyom.exe" >nul 2>&1
if exist "%INSTALL_DIR%\examples" rmdir /S /Q "%INSTALL_DIR%\examples" >nul 2>&1
rmdir /S /Q "%INSTALL_DIR%" >nul 2>&1
echo     Files removed
echo.

:cleanup_path
:: ---------------- STEP 3 ----------------
echo [3/4] Cleaning PATH...
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% equ 0 (
    powershell -Command ^
    "$p=[Environment]::GetEnvironmentVariable('Path','User'); ^
     $n=($p.Split(';') | Where-Object { $_ -ne 'C:\Vyom' }) -join ';'; ^
     [Environment]::SetEnvironmentVariable('Path',$n,'User')" >nul 2>&1
    echo     PATH cleaned
) else (
    echo     Vyom not found in PATH
)
echo.

:: ---------------- STEP 4 ----------------
echo [4/4] Finalizing...
echo.

:: ---------------- DONE ----------------
color 0A
echo =====================================================
echo   Vyom v0.7 Uninstalled Successfully
echo =====================================================
echo.
echo   Removed:
echo     - C:\Vyom directory
echo     - vyom.exe
echo     - examples
echo     - PATH entry
echo.
echo   IMPORTANT:
echo   - Close this terminal
echo   - Open a NEW terminal
echo.
echo   Verify:
echo     vyom --version
echo     (should say command not found)
echo.
echo =====================================================
pause

:end
endlocal
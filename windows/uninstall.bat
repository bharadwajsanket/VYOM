@echo off
title Vyom Uninstaller v0.6
color 0C

cls
echo.
echo ============================================================
echo   VYOM PROGRAMMING LANGUAGE
echo.
echo   v0.6 — Uninstaller
echo.
echo   This will remove Vyom from your system.
echo   Created by Sanket Bharadwaj
echo.
echo ============================================================
echo.
timeout /t 1 >nul

:: ---------- PATH ----------
set INSTALL_DIR=C:\Vyom

:: ---------- CONFIRM ----------
color 0E
choice /C YN /M "Are you sure you want to uninstall Vyom?"
if errorlevel 2 (
    echo.
    echo Uninstallation cancelled.
    pause
    exit /b 0
)
color 0C

:: ---------- STEP 1 ----------
echo.
echo [1/4] Checking installation...
if not exist "%INSTALL_DIR%" (
    color 0E
    echo     [i] Vyom directory not found
    echo     [i] It may already be uninstalled
    goto :cleanup_path
)

if exist "%INSTALL_DIR%\vyom.exe" (
    echo     [+] Vyom installation detected
) else (
    color 0E
    echo     [!] vyom.exe not found
    echo     [i] Cleaning remaining files anyway
)
timeout /t 1 >nul

:: ---------- STEP 2 ----------
echo.
echo [2/4] Removing files...

if exist "%INSTALL_DIR%\vyom.exe" (
    del /Q "%INSTALL_DIR%\vyom.exe" >nul 2>&1
    echo     [+] vyom.exe removed
)

if exist "%INSTALL_DIR%\examples" (
    rmdir /S /Q "%INSTALL_DIR%\examples" >nul 2>&1
    echo     [+] Examples removed
)

rmdir /S /Q "%INSTALL_DIR%" >nul 2>&1
if not exist "%INSTALL_DIR%" (
    echo     [+] Installation directory removed
) else (
    color 0E
    echo     [!] Could not remove directory completely
    echo     [!] You may delete manually: %INSTALL_DIR%
)
timeout /t 1 >nul

:cleanup_path
:: ---------- STEP 3 ----------
echo.
echo [3/4] Removing Vyom from PATH...

echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    echo     [i] Vyom not found in PATH
    goto :finalize
)

powershell -Command ^
"$p=[Environment]::GetEnvironmentVariable('Path','User'); ^
 $n=($p.Split(';') | Where-Object { $_ -ne 'C:\Vyom' }) -join ';'; ^
 [Environment]::SetEnvironmentVariable('Path',$n,'User')" >nul 2>&1

if %errorlevel% equ 0 (
    echo     [+] PATH entry removed
) else (
    color 0E
    echo     [!] Automatic PATH cleanup failed
    echo     [!] Remove C:\Vyom manually from PATH if needed
)
timeout /t 1 >nul

:finalize
:: ---------- STEP 4 ----------
echo.
echo [4/4] Finalizing...
timeout /t 1 >nul

color 0A
echo.
echo ============================================================
echo   Vyom v0.6 Uninstalled Successfully
echo ============================================================
echo.
echo Removed:
echo   [x] C:\Vyom directory
echo   [x] vyom.exe
echo   [x] example programs
echo   [x] PATH entry
echo.
echo IMPORTANT:
echo   - Close this terminal window
echo   - Open a NEW terminal session
echo.
echo To verify (in new terminal):
echo   vyom --version
echo   (should say command not found)
echo.
echo ============================================================
pause
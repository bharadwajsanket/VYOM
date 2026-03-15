@echo off
setlocal EnableDelayedExpansion
title Vyom Uninstaller
color 0C
cls

:: ================================================
::  Collect system info
:: ================================================

for /f "tokens=1* delims=:" %%A in ('systeminfo ^| findstr /B /C:"OS Name"') do set "OS_RAW=%%B"
set "OS_RAW=!OS_RAW:~1!"

for /f "tokens=1* delims=:" %%A in ('systeminfo ^| findstr /B /C:"OS Version"') do set "VER_RAW=%%B"
set "VER_RAW=!VER_RAW:~1!"

for /f "tokens=1* delims=:" %%A in ('systeminfo ^| findstr /B /C:"Total Physical Memory"') do set "RAM_RAW=%%B"
set "RAM_RAW=!RAM_RAW:~1!"

for /f "skip=1 tokens=*" %%A in ('wmic cpu get Name 2^>nul') do (
    if not defined CPU_NAME if not "%%A"=="" set "CPU_NAME=%%A"
)
if not defined CPU_NAME set "CPU_NAME=Unknown"

:: ================================================
::  Neofetch-style display
:: ================================================

echo.
echo    ██╗   ██╗██╗   ██╗ ██████╗ ███╗   ███╗     %USERNAME%@%COMPUTERNAME%
echo    ██║   ██║╚██╗ ██╔╝██╔═══██╗████╗ ████║     -------------------------
echo    ██║   ██║ ╚████╔╝ ██║   ██║██╔████╔██║     OS      : !OS_RAW!
echo    ╚██╗ ██╔╝  ╚██╔╝  ██║   ██║██║╚██╔╝██║     Version : !VER_RAW!
echo     ╚████╔╝    ██║   ╚██████╔╝██║ ╚═╝ ██║     CPU     : !CPU_NAME!
echo      ╚═══╝     ╚═╝    ╚═════╝ ╚═╝     ╚═╝     RAM     : !RAM_RAW!
echo                                                Shell   : cmd.exe
echo                                                Vyom    : v1.0
echo.
echo    https://getvyom.vercel.app
echo.
echo  ============================================
echo   Uninstaller
echo  ============================================
echo.

:: ---- Admin check ----
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo  [!] Administrator privileges required.
    echo.
    echo      Please run as Administrator to modify System PATH
    echo.
    echo      Right-click "uninstall.bat" and select
    echo      "Run as administrator"
    echo.
    echo  ============================================
    echo.
    pause
    exit /b 1
)

set "INSTALL_DIR=C:\Vyom"

echo  This will remove Vyom from %INSTALL_DIR%
echo  and clean up your system PATH.
echo.
echo  Press any key to continue, or close this window to cancel...
pause >nul
echo.

:: ---- Step 1: Remove install directory ----
echo  [1/2] Removing %INSTALL_DIR%...
if exist "%INSTALL_DIR%" (
    rmdir /S /Q "%INSTALL_DIR%"
    if %errorlevel% neq 0 (
        echo.
        echo  [ERROR] Could not remove %INSTALL_DIR%
        echo  Close any programs using vyom.exe and try again.
        echo.
        pause
        exit /b 1
    )
    echo         OK
) else (
    echo         %INSTALL_DIR% not found, skipping
)

:: ---- Step 2: Remove from system PATH using PowerShell ----
echo  [2/2] Cleaning system PATH...

powershell -NoProfile -ExecutionPolicy Bypass -Command "$target = 'C:\Vyom'; $regPath = 'Registry::HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment'; $current = (Get-ItemProperty -Path $regPath -Name Path).Path; $parts = $current -split ';' | Where-Object { $_.Trim() -ne '' -and $_.TrimEnd('\') -ne $target.TrimEnd('\') }; $newPath = $parts -join ';'; Set-ItemProperty -Path $regPath -Name Path -Value $newPath -Type ExpandString; Write-Host '        Removed C:\Vyom from system PATH'"

if %errorlevel% neq 0 (
    echo.
    echo  [WARNING] Could not update PATH automatically.
    echo  Manually remove C:\Vyom from your system PATH:
    echo  Control Panel ^> System ^> Advanced ^> Environment Variables
    echo.
)

echo.
echo  ============================================
echo   Vyom has been removed.
echo  ============================================
echo.
echo  Restart your terminal to apply changes.
echo.
pause
exit /b 0
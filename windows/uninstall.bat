@echo off
setlocal EnableDelayedExpansion
title Vyom Uninstaller
color 0C
cls

echo.
echo    ============================================
echo      Vyom Language  v1.0  --  Uninstaller
echo    ============================================
echo.

:: ---- Admin check ----
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo    [!] Administrator privileges required.
    echo.
    echo        Right-click uninstall.bat and select
    echo        "Run as administrator"
    echo.
    pause
    exit /b 1
)

set "INSTALL_DIR=C:\Vyom"

echo    This will remove Vyom from your system.
echo    Press any key to continue, or close to cancel...
echo.
pause >nul

:: ---- Step 1 ----
echo    [1/2]  Removing %INSTALL_DIR%...
if exist "%INSTALL_DIR%" (
    rmdir /S /Q "%INSTALL_DIR%"
    echo           done.
) else (
    echo           not found, skipping.
)

:: ---- Step 2: PATH via PowerShell ----
echo    [2/2]  Cleaning system PATH...

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
    "$dir = 'C:\Vyom';" ^
    "$reg = 'Registry::HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment';" ^
    "$cur = (Get-ItemProperty -Path $reg -Name Path).Path;" ^
    "$parts = $cur -split ';' | Where-Object { $_.Trim() -ne '' -and $_.TrimEnd('\') -ne $dir.TrimEnd('\') };" ^
    "$new = $parts -join ';';" ^
    "Set-ItemProperty -Path $reg -Name Path -Value $new -Type ExpandString;" ^
    "Write-Host '           done.';"

echo.
echo    ============================================
echo      Vyom has been removed.
echo.
echo      Restart your terminal to apply changes.
echo    ============================================
echo.
pause
exit /b 0
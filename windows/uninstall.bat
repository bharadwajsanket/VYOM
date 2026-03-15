@echo off
setlocal EnableDelayedExpansion
title Vyom Uninstaller

:: ================================================
::  VYOM UNINSTALLER
::  Removes C:\Vyom and cleans up system PATH
:: ================================================

color 0C
cls
echo.
echo  ============================================
echo   VYOM PROGRAMMING LANGUAGE  -  Uninstaller
echo  ============================================
echo.

:: ---- Admin check ----
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo  [ERROR] Administrator privileges required.
    echo.
    echo  Right-click uninstall.bat and select
    echo  "Run as administrator", then try again.
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
        echo  [ERROR] Could not remove %INSTALL_DIR%
        echo  Close any programs using vyom.exe and try again.
        pause
        exit /b 1
    )
    echo         OK
) else (
    echo         %INSTALL_DIR% not found, skipping
)

:: ---- Step 2: Remove from system PATH ----
:: Read system PATH from registry directly.
:: Rebuild it without the Vyom entry.
echo  [2/2] Cleaning system PATH...

for /f "tokens=2*" %%A in (
    'reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path 2^>nul'
) do set "SYS_PATH=%%B"

:: Check if Vyom is in PATH at all
echo !SYS_PATH! | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    echo         Vyom was not in system PATH, nothing to clean
) else (
    :: Rebuild PATH without the Vyom entry
    set "NEW_PATH="
    set "SEP="

    :: Split on semicolons and rebuild without the Vyom dir
    for %%E in ("!SYS_PATH:;=" "!") do (
        set "ENTRY=%%~E"
        if /i not "!ENTRY!" == "%INSTALL_DIR%" (
            if defined NEW_PATH (
                set "NEW_PATH=!NEW_PATH!;!ENTRY!"
            ) else (
                set "NEW_PATH=!ENTRY!"
            )
        )
    )

    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" ^
        /v Path /t REG_EXPAND_SZ /d "!NEW_PATH!" /f >nul 2>&1
    if %errorlevel% neq 0 (
        echo  [WARNING] Could not update PATH automatically.
        echo            Remove %INSTALL_DIR% from PATH manually if needed.
    ) else (
        echo         Removed %INSTALL_DIR% from system PATH
    )
)

:: Clean up the marker variable
reg delete "HKCU\Environment" /v VYOM_INSTALLED /f >nul 2>&1

echo.
echo  ============================================
echo   Vyom has been removed.
echo  ============================================
echo.
echo  Restart your terminal to apply PATH changes.
echo.
pause
exit /b 0
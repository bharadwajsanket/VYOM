@echo off
title Vyom Uninstaller v0.4
color 0C

cls
echo ========================================
echo        Vyom v0.4 Uninstaller
echo ========================================
echo.
echo This will remove Vyom from your system.
echo Created by Sanket Bharadwaj
echo.
echo ----------------------------------------
echo Initializing uninstaller...
echo ----------------------------------------
timeout /t 1 >nul

set INSTALL_DIR=C:\Vyom

:: Step 1 — Check installation
echo [1/3] Checking installation...
if not exist "%INSTALL_DIR%\vyom.exe" (
    color 0E
    echo     ! Vyom does not appear to be installed.
    echo     ! No action needed.
    echo.
    pause
    exit /b 0
)
echo     + Installation found
timeout /t 1 >nul

:: Step 2 — Remove files
echo [2/3] Removing Vyom files...
del /Q "%INSTALL_DIR%\vyom.exe" >nul 2>&1
rmdir /Q "%INSTALL_DIR%" >nul 2>&1
echo     + Files removed
timeout /t 1 >nul

:: Step 3 — Finish
echo [3/3] Finalizing uninstallation...
timeout /t 1 >nul

color 0A
echo.
echo ========================================
echo  Vyom v0.4 uninstalled successfully!
echo ========================================
echo.
echo NOTE:
echo - PATH entries are not removed automatically.
echo - Restart terminal if needed.
echo.
pause
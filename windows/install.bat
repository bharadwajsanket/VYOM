@echo off
setlocal EnableDelayedExpansion
title Vyom Installer v1.0
color 0B

:: =====================================================
:: VYOM INSTALLER v1.0
:: =====================================================

cls
echo.
echo  =============================================
echo.
echo       VYOM PROGRAMMING LANGUAGE
echo       v1.0 - Strict, Minimal, Honest
echo.
echo  =============================================
echo.

:: ---------------- ADMIN CHECK ----------------
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo  [!] Administrator privileges required
    echo.
    echo      Please run as Administrator to update System PATH
    echo.
    echo      Right-click "install.bat" and select
    echo      "Run as administrator"
    echo.
    echo  =============================================
    echo.
    pause
    exit /b 1
)

echo  Welcome to the Vyom Installer.
echo  This will install Vyom v1.0 to C:\Vyom
echo.
echo  Press any key to continue...
pause >nul
echo.

:: ---------------- PATHS ----------------
set "INSTALL_DIR=C:\Vyom"
set "EXAMPLES_DIR=%INSTALL_DIR%\examples"
set "SRC_EXAMPLES=%~dp0examples"
set "SRC_EXE=%~dp0vyom.exe"

:: ---------------- CHECKS ----------------
echo  [1/5] Checking for vyom.exe...
if not exist "%SRC_EXE%" (
    echo.
    echo  [ERROR] vyom.exe not found!
    echo  Please ensure install.bat is in the same folder as vyom.exe
    echo.
    pause
    exit /b 1
)
echo        Found vyom.exe

:: ---------------- INSTALLATION ----------------
echo  [2/5] Creating directories...
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
if not exist "%EXAMPLES_DIR%" mkdir "%EXAMPLES_DIR%"
echo        Created C:\Vyom

echo  [3/6] Copying executable...
copy /Y "%SRC_EXE%" "%INSTALL_DIR%\vyom.exe" >nul
if %errorlevel% neq 0 (
    echo  [ERROR] Failed to copy vyom.exe
    pause
    exit /b 1
)
echo        Copied vyom.exe

:: Copy icon if exists
set "SRC_ICO=%~dp0assets\vyom.ico"
if exist "%SRC_ICO%" (
    copy /Y "%SRC_ICO%" "%INSTALL_DIR%\vyom.ico" >nul
    echo        Copied vyom.ico
)

echo  [4/6] Installing examples...
if exist "%SRC_EXAMPLES%" (
    xcopy /E /I /Y "%SRC_EXAMPLES%" "%EXAMPLES_DIR%" >nul
    echo        Copied examples
) else (
    echo        No examples folder found, skipping
)

echo  [5/6] Updating System PATH...
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    setx /M PATH "%PATH%;%INSTALL_DIR%" >nul 2>&1
    if %errorlevel% neq 0 (
        echo        [WARNING] Could not update PATH automatically
        echo        Please add C:\Vyom to your PATH manually
    ) else (
        echo        Added C:\Vyom to PATH
    )
) else (
    echo        PATH already contains C:\Vyom
)

echo  [6/6] Registering .vy file association...
:: Register .vy extension
reg add "HKCR\.vy" /ve /d "VyomFile" /f >nul 2>&1
reg add "HKCR\VyomFile" /ve /d "Vyom Source File" /f >nul 2>&1
:: Set icon
if exist "%INSTALL_DIR%\vyom.ico" (
    reg add "HKCR\VyomFile\DefaultIcon" /ve /d "%INSTALL_DIR%\vyom.ico" /f >nul 2>&1
) else (
    reg add "HKCR\VyomFile\DefaultIcon" /ve /d "%INSTALL_DIR%\vyom.exe,0" /f >nul 2>&1
)
:: Set double-click to open in terminal and keep it open
reg add "HKCR\VyomFile\shell" /ve /d "open" /f >nul 2>&1
reg add "HKCR\VyomFile\shell\open" /ve /d "Run with Vyom" /f >nul 2>&1
reg add "HKCR\VyomFile\shell\open\command" /ve /d "cmd.exe /k C:\Vyom\vyom.exe \"%%1\"" /f >nul 2>&1
echo        Registered .vy file type

:: Refresh icon cache
ie4uinit.exe -show >nul 2>&1
echo        Refreshed icon cache

echo.
echo  =============================================
echo.
echo  SUCCESS! Vyom v1.0 has been installed.
echo.
echo  Installation Path:  %INSTALL_DIR%
echo  Executable:         vyom.exe
echo  Examples:           %EXAMPLES_DIR%
echo.
echo  =============================================
echo.
echo  IMPORTANT:
echo  Please RESTART your terminal to use the 'vyom' command.
echo.
echo  Try running:
echo    vyom --version
echo    vyom C:\Vyom\examples\main.vy
echo.
echo  =============================================
echo.
pause
exit /b 0

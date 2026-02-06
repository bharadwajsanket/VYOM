@echo off
setlocal EnableDelayedExpansion
title Vyom Installer v0.9
color 0B

:: =====================================================
:: VYOM INSTALLER v0.9
:: =====================================================

:header
cls
echo.
echo  [96m__      __     _  _     ______     __  __ [0m
echo  [96m\ \    / /    ( \/ )   / ____ \   |  \/  |[0m
echo  [96m \ \  / /      \  /   | |    | |  | \  / |[0m
echo  [96m  \ \/ /        \/    | |    | |  | |\/| |[0m
echo  [96m   \  /         ||    | |____| |  | |  | |[0m
echo  [96m    \/          ()     \______/   |_|  |_|[0m
echo.
echo  [97m   VYOM PROGRAMMING LANGUAGE[0m
echo  [90m   v0.9 | Strict • Minimal • Honest[0m
echo.
echo  [90m=============================================[0m
echo.

:: ---------------- ADMIN CHECK ----------------
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo  [93m[!] Administrator privileges required[0m
    echo      Please run as Administrator to update System PATH
    echo.
    echo      Right-click "install.bat" and select
    echo      "Run as administrator"
    echo.
    echo  [90m=============================================[0m
    pause
    goto :eof
)

echo  [97mWelcome to the Vyom Installer.[0m
echo  This will install Vyom v0.9 to [96mC:\Vyom[0m
echo.
pause
goto :install_start

:install_start
cls
call :header

:: ---------------- PATHS ----------------
set "INSTALL_DIR=C:\Vyom"
set "EXAMPLES_DIR=%INSTALL_DIR%\examples"
set "SRC_EXAMPLES=%~dp0examples"
set "SRC_EXE=%~dp0vyom.exe"

:: ---------------- CHECKS ----------------
if not exist "%SRC_EXE%" (
    echo.
    echo  [91m[ERROR] vyom.exe not found![0m
    echo  Please ensure install.bat is in the same folder as vyom.exe
    echo.
    pause
    goto :eof
)

:: ---------------- INSTALLATION ----------------

echo.
echo  [97mInstalling...[0m
echo.

:: Step 1: Directory Creation
call :progress 0 "Creating directories..."
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
if not exist "%EXAMPLES_DIR%" mkdir "%EXAMPLES_DIR%"
timeout /t 1 /nobreak >nul

:: Step 2: Main Executable
call :progress 25 "Copying executable..."
copy /Y "%SRC_EXE%" "%INSTALL_DIR%\vyom.exe" >nul
if %errorlevel% neq 0 (
    echo  [91mFailed to copy vyom.exe[0m
    pause
    goto :eof
)
timeout /t 1 /nobreak >nul

:: Step 3: Examples
call :progress 50 "Installing examples..."
if exist "%SRC_EXAMPLES%" (
    xcopy /E /I /Y "%SRC_EXAMPLES%" "%EXAMPLES_DIR%" >nul
)
timeout /t 1 /nobreak >nul

:: Step 4: PATH Environment
call :progress 75 "Updating System PATH..."
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    setx /M PATH "%PATH%;%INSTALL_DIR%" >nul
)
timeout /t 1 /nobreak >nul

:: Step 5: Verification
call :progress 90 "Verifying installation..."
"%INSTALL_DIR%\vyom.exe" --version >nul 2>&1
if %errorlevel% neq 0 (
    echo  [93mWarning: Verification warning (version check failed)[0m
)
timeout /t 1 /nobreak >nul

call :progress 100 "Installation Complete"
echo.
echo.

:: ---------------- SUCCESS ----------------
echo  [92mSUCCESS![0m Vyom v0.9 has been installed.
echo.
echo  [90m---------------------------------------------[0m
echo  [97mInstallation Path:[0m  %INSTALL_DIR%
echo  [97mExecutable:[0m         vyom.exe
echo  [97mExamples:[0m           %EXAMPLES_DIR%
echo  [90m---------------------------------------------[0m
echo.
echo  [96mIMPORTANT:[0m
echo  Please [97mRESTART[0m your terminal/console to use the 'vyom' command.
echo.
echo  Try running:
echo    [93mvyom --version[0m
echo    [93mvyom examples\main.vy[0m
echo.
echo  [90m=============================================[0m
pause
goto :eof

:: =====================================================
:: PROGRESS BAR FUNCTION
:: =====================================================
:progress
set "percent=%~1"
set "message=%~2"

rem Calculate number of bars (20 chars total width)
set /a "bars=%percent% / 5"
set "progressbar="

rem Build bar string
if %bars% gtr 0 (
    for /l %%i in (1,1,%bars%) do set "progressbar=!progressbar!█"
)
set /a "spaces=20-%bars%"
if %spaces% gtr 0 (
    for /l %%i in (1,1,%spaces%) do set "progressbar=!progressbar!░"
)

rem Display line with carriage return hack not needed if we clear specific lines, 
rem but for simplicity in batch we just redraw or print nicely.
rem Here we use a clean formatted echo.

echo  [%progressbar%] %percent%%% - %message%
exit /b
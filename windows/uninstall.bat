@echo off
setlocal EnableDelayedExpansion
title Vyom Uninstaller v0.9
color 0C

:: =====================================================
:: VYOM UNINSTALLER v0.9
:: =====================================================

:header
cls
echo.
echo   [91m__      __     _  _     ______     __  __  [0m
echo   [91m\ \    / /    ( \/ )   / ____ \   |  \/  | [0m
echo   [91m \ \  / /      \  /   | |    | |  | \  / | [0m
echo   [91m  \ \/ /        \/    | |    | |  | |\/| | [0m
echo   [91m   \  /         ||    | |____| |  | |  | | [0m
echo   [91m    \/          ()     \______/   |_|  |_| [0m
echo.
echo   [97m   VYOM UNINSTALLER [0m
echo   [90m   Clean Removal • No Leftovers [0m
echo.
echo   [90m============================================= [0m
echo.

:: ---------------- CONFIRM ----------------
echo   [93m[?] Are you sure you want to uninstall Vyom? [0m
echo      This will remove Vyom v0.9 and all examples.
echo.
set /p "confirm=      Type Y to continue: "
if /i not "%confirm%"=="Y" goto :eof

echo.
echo   [97mUninstalling... [0m
echo.

:: ---------------- PATHS ----------------
set "INSTALL_DIR=C:\Vyom"

:: Step 1: Check Install
call :progress 10 "Checking installation..."
if not exist "%INSTALL_DIR%" (
    timeout /t 1 /nobreak >nul
    echo   [93mVyom is not installed at %INSTALL_DIR% [0m
    timeout /t 2 /nobreak >nul
    goto :cleanup_path
)
timeout /t 1 /nobreak >nul

:: Step 2: Remove Files
call :progress 40 "Removing files..."
if exist "%INSTALL_DIR%\vyom.exe" del /Q "%INSTALL_DIR%\vyom.exe" >nul 2>&1
if exist "%INSTALL_DIR%\examples" rmdir /S /Q "%INSTALL_DIR%\examples" >nul 2>&1
timeout /t 1 /nobreak >nul

call :progress 60 "Removing directories..."
rmdir /S /Q "%INSTALL_DIR%" >nul 2>&1
timeout /t 1 /nobreak >nul

:cleanup_path
:: Step 3: Clean PATH
call :progress 80 "Cleaning System PATH..."
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% equ 0 (
    powershell -Command ^
    "$p=[Environment]::GetEnvironmentVariable('Path','User'); ^
     $n=($p.Split(';') | Where-Object { $_ -ne 'C:\Vyom' }) -join ';'; ^
     [Environment]::SetEnvironmentVariable('Path',$n,'User')" >nul 2>&1
)
timeout /t 1 /nobreak >nul

call :progress 100 "Uninstallation Complete"
echo.
echo.

:: ---------------- SUCCESS ----------------
echo   [92mSUCCESS! [0m Vyom has been removed from your system.
echo.
echo   [90m--------------------------------------------- [0m
echo   [97mRemoved Items: [0m
echo     - Installation Directory (C:\Vyom)
echo     - Environment Variable (PATH)
echo     - Example Files
echo   [90m--------------------------------------------- [0m
echo.
echo   [97mThank you for using Vyom. [0m
echo.
echo   [90m============================================= [0m
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

echo  [%progressbar%] %percent%%% - %message%
exit /b
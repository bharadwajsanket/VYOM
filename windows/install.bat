@echo off
setlocal EnableDelayedExpansion
title Vyom Installer

:: ================================================
::  VYOM INSTALLER
::  Installs Vyom to C:\Vyom and updates PATH
:: ================================================

color 0B
cls
echo.
echo  ============================================
echo   VYOM PROGRAMMING LANGUAGE  -  Installer
echo  ============================================
echo.

:: ---- Admin check ----
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo  [ERROR] Administrator privileges required.
    echo.
    echo  Right-click install.bat and select
    echo  "Run as administrator", then try again.
    echo.
    pause
    exit /b 1
)

:: ---- Check vyom.exe exists next to this script ----
if not exist "%~dp0vyom.exe" (
    echo  [ERROR] vyom.exe not found.
    echo  Make sure install.bat is in the same folder as vyom.exe.
    echo.
    pause
    exit /b 1
)

set "INSTALL_DIR=C:\Vyom"

echo  Installing to: %INSTALL_DIR%
echo.

:: ---- Step 1: Create directory ----
echo  [1/4] Creating install directory...
if not exist "%INSTALL_DIR%" (
    mkdir "%INSTALL_DIR%"
    if %errorlevel% neq 0 (
        echo  [ERROR] Could not create %INSTALL_DIR%
        pause
        exit /b 1
    )
)
echo         OK

:: ---- Step 2: Copy executable ----
echo  [2/4] Copying vyom.exe...
copy /Y "%~dp0vyom.exe" "%INSTALL_DIR%\vyom.exe" >nul
if %errorlevel% neq 0 (
    echo  [ERROR] Failed to copy vyom.exe
    pause
    exit /b 1
)
echo         OK

:: ---- Step 3: Copy examples if present ----
echo  [3/4] Copying examples...
if exist "%~dp0examples" (
    if not exist "%INSTALL_DIR%\examples" mkdir "%INSTALL_DIR%\examples"
    xcopy /E /I /Y "%~dp0examples" "%INSTALL_DIR%\examples" >nul
    echo         OK
) else (
    echo         No examples folder found, skipping
)

:: ---- Step 4: Add to SYSTEM PATH via registry ----
:: Read the CURRENT system PATH directly from registry.
:: Do NOT use %PATH% — that includes session variables and can corrupt the entry.
echo  [4/4] Updating system PATH...

for /f "tokens=2*" %%A in (
    'reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path 2^>nul'
) do set "SYS_PATH=%%B"

echo !SYS_PATH! | find /i "%INSTALL_DIR%" >nul
if %errorlevel% equ 0 (
    echo         PATH already contains %INSTALL_DIR%, skipping
) else (
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" ^
        /v Path /t REG_EXPAND_SZ /d "!SYS_PATH!;%INSTALL_DIR%" /f >nul 2>&1
    if %errorlevel% neq 0 (
        echo  [WARNING] Could not update PATH automatically.
        echo            Add %INSTALL_DIR% to your PATH manually.
    ) else (
        echo         Added %INSTALL_DIR% to system PATH
    )
)

:: ---- Notify running processes of the PATH change ----
setx VYOM_INSTALLED 1 >nul 2>&1

echo.
echo  ============================================
echo   Installation complete.
echo.
echo   Path:  %INSTALL_DIR%\vyom.exe
echo  ============================================
echo.
echo  IMPORTANT: Restart your terminal before use.
echo.
echo  Then verify with:
echo    vyom --version
echo.
pause
exit /b 0
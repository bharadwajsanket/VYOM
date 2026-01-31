@echo off
title Vyom Installer v0.7
color 0A
setlocal

:: =====================================================
:: Vyom Installer v0.7
:: =====================================================

cls
echo.
echo =====================================================
echo   VYOM PROGRAMMING LANGUAGE
echo.
echo   v0.7 — Strict Core & Developer Quality
echo   Minimal • Explicit • Honest Execution
echo.
echo   Created by Sanket Bharadwaj
echo =====================================================
echo.

:: ---------------- ADMIN CHECK ----------------
net session >nul 2>&1
if %errorlevel% neq 0 (
    color 0E
    echo [!] Administrator privileges not detected
    echo     Vyom will be added to USER PATH only
    echo.
    choice /C YN /M "Continue installation?"
    if errorlevel 2 goto :end
    color 0A
)

:: ---------------- PATHS ----------------
set INSTALL_DIR=C:\Vyom
set EXAMPLES_DIR=%INSTALL_DIR%\examples

:: ---------------- STEP 1 ----------------
echo [1/5] Creating directories...
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
if not exist "%EXAMPLES_DIR%" mkdir "%EXAMPLES_DIR%"
echo     OK
echo.

:: ---------------- STEP 2 ----------------
echo [2/5] Installing Vyom executable...
if not exist "%~dp0vyom.exe" (
    color 0C
    echo     ERROR: vyom.exe not found
    echo     Keep install.bat and vyom.exe together
    pause
    goto :end
)
copy /Y "%~dp0vyom.exe" "%INSTALL_DIR%\vyom.exe" >nul
echo     OK
echo.

:: ---------------- STEP 3 ----------------
echo [3/5] Installing examples...
if exist "%~dp0examples" (
    xcopy /E /I /Y "%~dp0examples" "%EXAMPLES_DIR%" >nul
    echo     OK
) else (
    echo     (no examples found)
)
echo.

:: ---------------- STEP 4 ----------------
echo [4/5] Adding Vyom to PATH...
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    powershell -Command ^
    "$p=[Environment]::GetEnvironmentVariable('Path','User'); ^
     if($p -notlike '*C:\Vyom*'){ ^
     [Environment]::SetEnvironmentVariable('Path',$p+';C:\Vyom','User')}"
    echo     OK
) else (
    echo     Already in PATH
)
echo.

:: ---------------- STEP 5 ----------------
echo [5/5] Verifying installation...
"%INSTALL_DIR%\vyom.exe" --version
echo.

:: ---------------- DONE ----------------
echo =====================================================
echo   Vyom v0.7 Installed Successfully
echo =====================================================
echo.
echo   IMPORTANT:
echo   - Close this terminal
echo   - Open a NEW terminal
echo.
echo   Try:
echo     vyom --version
echo     vyom C:\Vyom\examples\main.vy
echo.
echo =====================================================
pause

:end
endlocal
@echo off
setlocal EnableDelayedExpansion
title Vyom Installer
color 0B
cls

echo.
echo    ============================================
echo      Vyom Language  v1.0  --  Installer
echo    ============================================
echo.

:: ---- Admin check ----
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo    [!] Administrator privileges required.
    echo.
    echo        Right-click install.bat and select
    echo        "Run as administrator"
    echo.
    pause
    exit /b 1
)

:: ---- Check vyom.exe ----
if not exist "%~dp0vyom.exe" (
    echo    [ERROR] vyom.exe not found.
    echo    Place install.bat next to vyom.exe.
    echo.
    pause
    exit /b 1
)

set "INSTALL_DIR=C:\Vyom"

echo    Installing to  %INSTALL_DIR%
echo.

:: ---- Step 1 ----
echo    [1/4]  Creating directory...
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
echo           done.

:: ---- Step 2 ----
echo    [2/4]  Copying vyom.exe...
copy /Y "%~dp0vyom.exe" "%INSTALL_DIR%\vyom.exe" >nul
if %errorlevel% neq 0 (
    echo    [ERROR] Failed to copy vyom.exe
    pause
    exit /b 1
)
echo           done.

:: ---- Step 3 ----
echo    [3/4]  Copying examples...
if exist "%~dp0examples" (
    if not exist "%INSTALL_DIR%\examples" mkdir "%INSTALL_DIR%\examples"
    xcopy /E /I /Y "%~dp0examples" "%INSTALL_DIR%\examples" >nul
    echo           done.
) else (
    echo           skipped.
)

:: ---- Step 4: Add to PATH via PowerShell ----
echo    [4/4]  Updating system PATH...

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
    "$dir = 'C:\Vyom';" ^
    "$reg = 'Registry::HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment';" ^
    "$cur = (Get-ItemProperty -Path $reg -Name Path).Path;" ^
    "if ($cur -notlike ('*' + $dir + '*')) {" ^
    "    $new = $cur.TrimEnd(';') + ';' + $dir;" ^
    "    Set-ItemProperty -Path $reg -Name Path -Value $new -Type ExpandString;" ^
    "    Write-Host '           done.';" ^
    "} else {" ^
    "    Write-Host '           already in PATH.';" ^
    "}"

echo.
echo    ============================================
echo      Vyom installed successfully.
echo.
echo      Restart your terminal, then run:
echo        vyom --version
echo    ============================================
echo.
pause
exit /b 0
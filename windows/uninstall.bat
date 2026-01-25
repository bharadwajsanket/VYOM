@echo off
title Vyom Uninstaller v0.5
color 0C

cls
echo ========================================
echo        Vyom v0.5 Uninstaller
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

:: Confirmation prompt
echo.
color 0E
choice /C YN /M "Are you sure you want to uninstall Vyom"
if errorlevel 2 (
    echo.
    echo Uninstallation cancelled.
    pause
    exit /b 0
)
color 0C
echo.

:: Step 1 — Check installation
echo [1/4] Checking installation...
if not exist "%INSTALL_DIR%" (
    color 0E
    echo     [i] Vyom directory not found
    echo     [i] May already be uninstalled
    goto :cleanup_path
)

if not exist "%INSTALL_DIR%\vyom.exe" (
    color 0E
    echo     [!] Vyom executable not found
    echo     [i] Cleaning up directory anyway...
) else (
    echo     [+] Installation found
)
timeout /t 1 >nul

:: Step 2 — Remove files
echo [2/4] Removing Vyom files...

:: Delete executable first
if exist "%INSTALL_DIR%\vyom.exe" (
    del /Q "%INSTALL_DIR%\vyom.exe" >nul 2>&1
    if %errorlevel% equ 0 (
        echo     [+] vyom.exe removed
    ) else (
        echo     [!] Failed to remove vyom.exe
    )
)

:: Remove examples directory
if exist "%INSTALL_DIR%\examples" (
    rmdir /S /Q "%INSTALL_DIR%\examples" >nul 2>&1
    echo     [+] Examples removed
)

:: Remove main directory
rmdir /S /Q "%INSTALL_DIR%" >nul 2>&1
if not exist "%INSTALL_DIR%" (
    echo     [+] Installation directory removed
) else (
    color 0E
    echo     [!] Could not remove directory completely
    echo     [!] You may need to delete manually: %INSTALL_DIR%
)
timeout /t 1 >nul

:cleanup_path
:: Step 3 — Remove from PATH (using safe PowerShell method)
echo [3/4] Removing from system PATH...

:: Check if Vyom is in PATH
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    echo     [i] Not found in PATH (already removed)
    goto :finalize
)

echo     [i] Removing from PATH via PowerShell...

powershell -Command "$currentPath = [Environment]::GetEnvironmentVariable('Path', 'User'); $newPath = ($currentPath.Split(';') | Where-Object { $_ -ne 'C:\Vyom' }) -join ';'; [Environment]::SetEnvironmentVariable('Path', $newPath, 'User')" >nul 2>&1

if %errorlevel% equ 0 (
    echo     [+] Removed from PATH successfully
) else (
    color 0E
    echo     [!] Automatic PATH removal failed
    echo.
    echo ----------------------------------------
    echo Manual PATH Cleanup Required:
    echo ----------------------------------------
    echo 1. Press Win + R
    echo 2. Type: sysdm.cpl
    echo 3. Go to Advanced ^> Environment Variables
    echo 4. Under User variables, select Path
    echo 5. Click Edit
    echo 6. Find and remove: C:\Vyom
    echo 7. Click OK
    echo ----------------------------------------
    echo.
)
timeout /t 1 >nul

:finalize
:: Step 4 — Finalize
echo [4/4] Finalizing uninstallation...
timeout /t 1 >nul

color 0A
echo.
echo ========================================
echo  Vyom v0.5 Uninstalled Successfully!
echo ========================================
echo.
echo What was removed:
echo   [x] C:\Vyom directory
echo   [x] vyom.exe executable
echo   [x] Example programs
echo   [x] PATH entry
echo.
echo IMPORTANT:
echo   - Close this terminal window
echo   - Open a NEW terminal for PATH changes to apply
echo   - Old terminals may still have 'vyom' command cached
echo.
echo To verify removal (in NEW terminal):
echo   vyom --version
echo   (should show 'command not found')
echo.
echo Thank you for trying Vyom!
echo.
pause
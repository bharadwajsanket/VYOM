@echo off
title Vyom Installer v0.5
color 0A

cls
echo ========================================
echo        Vyom v0.5 Installer
echo ========================================
echo.
echo   Control Flow ^& Boolean Logic
echo   Minimal, Explicit Scripting Language
echo.
echo   Created by Sanket Bharadwaj
echo ========================================
echo.
timeout /t 1 >nul

:: Check for admin privileges
net session >nul 2>&1
if %errorLevel% neq 0 (
    color 0E
    echo [WARNING] Running without administrator privileges
    echo           PATH update may fail for system-wide access
    echo.
    echo Recommendation: Right-click install.bat ^> Run as administrator
    echo.
    choice /C YN /M "Continue anyway"
    if errorlevel 2 exit /b 1
    echo.
)

set INSTALL_DIR=C:\Vyom
set EXAMPLES_DIR=%INSTALL_DIR%\examples

:: Step 1 — Create directories
echo [1/5] Preparing installation directories...
if not exist "%INSTALL_DIR%" (
    mkdir "%INSTALL_DIR%"
    echo     [+] Created %INSTALL_DIR%
) else (
    echo     [i] Directory already exists
)

if not exist "%EXAMPLES_DIR%" (
    mkdir "%EXAMPLES_DIR%"
    echo     [+] Created %EXAMPLES_DIR%
) else (
    echo     [i] Examples directory already exists
)
timeout /t 1 >nul

:: Step 2 — Copy executable
echo [2/5] Installing Vyom executable...
if not exist "%~dp0vyom.exe" (
    color 0C
    echo     [!] ERROR: vyom.exe not found
    echo     [!] Keep install.bat and vyom.exe in same folder
    echo.
    pause
    exit /b 1
)

copy /Y "%~dp0vyom.exe" "%INSTALL_DIR%\vyom.exe" >nul
if %errorlevel% equ 0 (
    echo     [+] vyom.exe installed
) else (
    color 0C
    echo     [!] Failed to copy vyom.exe
    pause
    exit /b 1
)
timeout /t 1 >nul

:: Step 3 — Copy examples (if available)
echo [3/5] Installing example programs...
if exist "%~dp0examples" (
    xcopy /E /I /Y "%~dp0examples" "%EXAMPLES_DIR%" >nul
    echo     [+] Example programs installed
) else (
    if exist "%~dp0main.vy" (
        copy /Y "%~dp0main.vy" "%EXAMPLES_DIR%\main.vy" >nul
        echo     [+] main.vy installed
    ) else (
        echo     [i] No examples found (optional)
    )
)
timeout /t 1 >nul

:: Step 4 — Add to PATH (SAFE METHOD - avoids truncation)
echo [4/5] Registering Vyom in system PATH...

:: Check if already in PATH
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% equ 0 (
    echo     [i] Already in PATH
    goto :verify_install
)

:: Use PowerShell to safely update PATH (avoids setx truncation bug)
echo     [i] Updating PATH via PowerShell (safer method)...

powershell -Command "$currentPath = [Environment]::GetEnvironmentVariable('Path', 'User'); if ($currentPath -notlike '*C:\Vyom*') { [Environment]::SetEnvironmentVariable('Path', $currentPath + ';C:\Vyom', 'User') }" >nul 2>&1

if %errorlevel% equ 0 (
    echo     [+] PATH updated successfully
    echo     [i] User-level PATH modified
) else (
    color 0E
    echo     [!] Automatic PATH update failed
    echo.
    echo ----------------------------------------
    echo Manual PATH Setup Required:
    echo ----------------------------------------
    echo 1. Press Win + R
    echo 2. Type: sysdm.cpl
    echo 3. Go to Advanced ^> Environment Variables
    echo 4. Under User variables, select Path
    echo 5. Click Edit ^> New
    echo 6. Add: C:\Vyom
    echo 7. Click OK
    echo ----------------------------------------
    echo.
)

:verify_install
timeout /t 1 >nul

:: Step 5 — Verify installation
echo [5/5] Verifying installation...
if exist "%INSTALL_DIR%\vyom.exe" (
    echo     [+] Vyom executable found
    
    :: Try to get version
    "%INSTALL_DIR%\vyom.exe" --version 2>nul
    if %errorlevel% equ 0 (
        echo     [+] Vyom responds correctly
    )
) else (
    color 0C
    echo     [!] Installation verification failed
)
timeout /t 1 >nul

echo.
echo ========================================
echo  Installation Completed Successfully!
echo ========================================
echo.
echo Installation location:
echo   %INSTALL_DIR%
echo.
echo IMPORTANT - NEXT STEPS:
echo   1. CLOSE this terminal window
echo   2. Open a NEW Command Prompt / PowerShell
echo   3. Run: vyom --version
echo   4. Try: vyom "%EXAMPLES_DIR%\main.vy"
echo.
echo Why restart terminal?
echo   PATH changes only apply to NEW terminal sessions
echo.
echo Commands:
echo   vyom --version       Show version
echo   vyom file.vy         Run a Vyom program
echo.
echo Documentation:
echo   README.md            Language guide
echo   CHANGELOG.md         Version history
echo   examples/            Sample programs
echo.
echo ========================================
echo  Ready to code in Vyom v0.5!
echo ========================================
echo.
echo NOTE: If 'vyom' is not found after restart,
echo       see manual PATH setup instructions above.
echo.
pause
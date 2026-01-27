@echo off
title Vyom Installer v0.6
color 0A

cls
echo.
echo =====================================================================
echo.
echo   VYOM PROGRAMMING LANGUAGE
echo.
echo   v0.6 — Control Flow + Loops
echo   Minimal • Explicit • Designed for Speed
echo.
echo   Created by Sanket Bharadwaj
echo.
echo =====================================================================
echo.

:: ---------- ASCII BANNER ----------
echo :+++++++++++++++++++++++++++++=++++++++++++++++:
echo +@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%@@@@@@@@@@@@@@@+
echo +@@@@@@@@@@@@@@@@@@@@@@@@@@%%%@%@@%%@@@@@@@@@@@+
echo +@@@@%@@@@@+        @@@@%%%%%@%@@@@@@@@@%%%%%@%=
echo =%@@%@@@:      :=+:   %@@%@@@%@%@@@@@@@@@%@%%@%=
echo +@@@@@   =@@@@@=    -:  @@@@%@@@@%@@@@@@@@@@@@@+
echo +%%@* @@@@@@@@@@@@@#    =@@@%@@@@@@@@@@@@@@@@%@+
echo +@@@@@@@@:     @@@@@@@   @@@@@@%          .@@@@+
echo +@@@@     :==-     @@@@@  @@@@##@:  .**+=-   @@+
echo +@@   :==++++=====   @@@@@:@@@@@@@@@ :+**+++  @@
echo %@  :*******+****=++:  @@@@@@@@%%@%@@ ++**=++  @
echo @  =**++=---===--=++++:  @@@@@%@@@@@@:****+++= @
echo @ -++++=#@%%%*:    .=+++:   @@@@@@@  =++**+*+= @
echo @ +*+++%#**=  *@@@@    =++=        .++*#*=+*+  @
echo @ -+++#***+ *@@@@@@@@@.  -+++-:*######+=+*#+  @@
echo @  +****+++ @@@@@@@@@@@@#  :***+::=+++***-   @@+
echo @@ :++*+*++ @@@@@@@@@@@@@@@   .+***+==:    @@@@+
echo +@@ -+++**+  @@@%@@     @@@@@@         *@@@@@@@+
echo +@@@ :+=+++= #@@@@  **=   +@@@@@@@@@@@@@@@@%@@@+
echo +@@@@  -=+++-  @@@  =+*+=:   .%@@@@@@*  *@@%@@@+
echo +@@@@@+  :+++=  @@@  ++**+==:         @@@@@@@@@+
echo +@@@@@@@*       @@@@   =**#####*-  .@@@@%%%%@@@+
echo +@%@@@@@@@@=  @@@@@@@@           @@@@@%%@@@@@@@+
echo +@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%@@@%%%@@@%+
echo +@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+
echo :+=+++++++++++++++++=+++++++=+==+++++++++++===+:
echo.

:: ---------- ADMIN CHECK ----------
net session >nul 2>&1
if %errorLevel% neq 0 (
    color 0E
    echo [WARNING] Not running as administrator
    echo PATH update may require admin privileges.
    echo.
    choice /C YN /M "Continue installation anyway?"
    if errorlevel 2 exit /b 1
    color 0A
)

:: ---------- PATHS ----------
set INSTALL_DIR=C:\Vyom
set EXAMPLES_DIR=%INSTALL_DIR%\examples

:: ---------- STEP 1 ----------
echo.
echo [1/5] Creating directories...
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
if not exist "%EXAMPLES_DIR%" mkdir "%EXAMPLES_DIR%"
echo     [+] Directories ready
timeout /t 1 >nul

:: ---------- STEP 2 ----------
echo.
echo [2/5] Installing Vyom executable...
if not exist "%~dp0vyom.exe" (
    color 0C
    echo [ERROR] vyom.exe not found in installer folder
    echo Keep install.bat and vyom.exe in the same directory
    pause
    exit /b 1
)
copy /Y "%~dp0vyom.exe" "%INSTALL_DIR%\vyom.exe" >nul
echo     [+] vyom.exe installed
timeout /t 1 >nul

:: ---------- STEP 3 ----------
echo.
echo [3/5] Installing examples...
if exist "%~dp0examples" (
    xcopy /E /I /Y "%~dp0examples" "%EXAMPLES_DIR%" >nul
) else if exist "%~dp0main.vy" (
    copy /Y "%~dp0main.vy" "%EXAMPLES_DIR%\main.vy" >nul
)
echo     [+] Examples installed
timeout /t 1 >nul

:: ---------- STEP 4 ----------
echo.
echo [4/5] Adding Vyom to PATH (safe method)...
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    powershell -Command ^
    "$p=[Environment]::GetEnvironmentVariable('Path','User'); ^
     if($p -notlike '*C:\Vyom*'){ ^
     [Environment]::SetEnvironmentVariable('Path',$p+';C:\Vyom','User')}"
    echo     [+] PATH updated (User scope)
) else (
    echo     [i] PATH already contains Vyom
)
timeout /t 1 >nul

:: ---------- STEP 5 ----------
echo.
echo [5/5] Verifying installation...
if exist "%INSTALL_DIR%\vyom.exe" (
    "%INSTALL_DIR%\vyom.exe" --version
) else (
    color 0C
    echo [ERROR] Vyom executable not found after installation
)
timeout /t 1 >nul

:: ---------- DONE ----------
echo.
echo ============================================================
echo   Vyom v0.6 Installed Successfully!
echo ============================================================
echo.
echo   Location: C:\Vyom
echo.
echo   IMPORTANT:
echo   - Close this terminal
echo   - Open a NEW terminal
echo.
echo   Try:
echo     vyom --version
echo     vyom C:\Vyom\examples\main.vy
echo.
echo ============================================================
pause
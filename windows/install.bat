@echo off
title Vyom Installer v0.7
color 0A
setlocal EnableDelayedExpansion

:: ================= CONFIG =================
set TOTAL_STEPS=5
set CURRENT_STEP=0

:: ================= BANNER =================
cls
echo.
echo =====================================================================
echo.
echo   VYOM PROGRAMMING LANGUAGE
echo.
echo   v0.7 — Strict Core & Developer Quality
echo   Minimal • Explicit • Honest Execution
echo.
echo   Created by Sanket Bharadwaj
echo.
echo =====================================================================
echo.
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

timeout /t 2 >nul

:: ================= ADMIN CHECK =================
net session >nul 2>&1
if %errorLevel% neq 0 (
    color 0E
    echo [WARNING] Not running as administrator
    echo PATH will be updated for USER only.
    choice /C YN /M "Continue installation?"
    if errorlevel 2 goto :end
    color 0A
)

:: ================= PATHS =================
set INSTALL_DIR=C:\Vyom
set EXAMPLES_DIR=%INSTALL_DIR%\examples

:: ================= STEP 1 =================
set /a CURRENT_STEP+=1
echo [1/5] Creating directories...
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
if not exist "%EXAMPLES_DIR%" mkdir "%EXAMPLES_DIR%"
timeout /t 1 >nul

:: ================= STEP 2 =================
set /a CURRENT_STEP+=1
echo [2/5] Installing Vyom executable...
if not exist "%~dp0vyom.exe" (
    color 0C
    echo ERROR: vyom.exe not found
    pause
    goto :end
)
copy /Y "%~dp0vyom.exe" "%INSTALL_DIR%\vyom.exe" >nul
timeout /t 1 >nul

:: ================= STEP 3 =================
set /a CURRENT_STEP+=1
echo [3/5] Installing examples...
if exist "%~dp0examples" (
    xcopy /E /I /Y "%~dp0examples" "%EXAMPLES_DIR%" >nul
)
timeout /t 1 >nul

:: ================= STEP 4 =================
set /a CURRENT_STEP+=1
echo [4/5] Adding Vyom to PATH...
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    powershell -Command ^
    "$p=[Environment]::GetEnvironmentVariable('Path','User'); ^
     if($p -notlike '*C:\Vyom*'){ ^
     [Environment]::SetEnvironmentVariable('Path',$p+';C:\Vyom','User')}"
)
timeout /t 1 >nul

:: ================= STEP 5 =================
set /a CURRENT_STEP+=1
echo [5/5] Verifying installation...
"%INSTALL_DIR%\vyom.exe" --version
timeout /t 1 >nul

:: ================= DONE =================
cls
echo ============================================================
echo   Vyom v0.7 Installed Successfully!
echo ============================================================
echo.
echo   Open a NEW terminal and try:
echo     vyom --version
echo     vyom C:\Vyom\examples\main.vy
echo.
echo ============================================================
pause

:end
endlocal
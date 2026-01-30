@echo off
title Vyom Installer v0.7
color 0A
setlocal EnableDelayedExpansion

:: ================= CONFIG =================
set TOTAL_STEPS=5
set CURRENT_STEP=0

:: ================= ASCII BANNER =================
:banner
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
exit /b

:: ================= PROGRESS BAR =================
:progress
set /a CURRENT_STEP+=1
set BAR=
set /a FILLED=(CURRENT_STEP*20)/TOTAL_STEPS
set /a EMPTY=20-FILLED

for /L %%i in (1,1,!FILLED!) do set BAR=!BAR!█
for /L %%i in (1,1,!EMPTY!) do set BAR=!BAR!░

call :banner
echo   Progress: [!BAR!]  !CURRENT_STEP!/!TOTAL_STEPS!
echo.
exit /b

:: ================= START =================
call :banner
timeout /t 2 >nul

:: ================= ADMIN CHECK =================
net session >nul 2>&1
if %errorLevel% neq 0 (
    color 0E
    echo [WARNING] Not running as administrator
    echo PATH will be updated for USER only.
    choice /C YN /M "Continue installation?"
    if errorlevel 2 exit /b 1
    color 0A
)

:: ================= PATHS =================
set INSTALL_DIR=C:\Vyom
set EXAMPLES_DIR=%INSTALL_DIR%\examples

:: ================= STEP 1 =================
call :progress
echo Creating directories...
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
if not exist "%EXAMPLES_DIR%" mkdir "%EXAMPLES_DIR%"
timeout /t 1 >nul

:: ================= STEP 2 =================
call :progress
echo Installing Vyom executable...
if not exist "%~dp0vyom.exe" (
    color 0C
    echo ERROR: vyom.exe not found
    pause
    exit /b 1
)
copy /Y "%~dp0vyom.exe" "%INSTALL_DIR%\vyom.exe" >nul
timeout /t 1 >nul

:: ================= STEP 3 =================
call :progress
echo Installing examples...
if exist "%~dp0examples" (
    xcopy /E /I /Y "%~dp0examples" "%EXAMPLES_DIR%" >nul
)
timeout /t 1 >nul

:: ================= STEP 4 =================
call :progress
echo Adding Vyom to PATH...
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% neq 0 (
    powershell -Command ^
    "$p=[Environment]::GetEnvironmentVariable('Path','User'); ^
     if($p -notlike '*C:\Vyom*'){ ^
     [Environment]::SetEnvironmentVariable('Path',$p+';C:\Vyom','User')}"
)
timeout /t 1 >nul

:: ================= STEP 5 =================
call :progress
echo Verifying installation...
"%INSTALL_DIR%\vyom.exe" --version
timeout /t 1 >nul

:: ================= DONE =================
call :banner
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
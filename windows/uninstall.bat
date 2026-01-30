@echo off
title Vyom Uninstaller v0.7
color 0C
setlocal EnableDelayedExpansion

:: ================= CONFIG =================
set TOTAL_STEPS=4
set CURRENT_STEP=0
set INSTALL_DIR=C:\Vyom

:: ================= BANNER =================
cls
echo.
echo ============================================================
echo.
echo   VYOM PROGRAMMING LANGUAGE
echo.
echo   v0.7 — Uninstaller
echo   Clean • Explicit • No Leftovers
echo.
echo   Created by Sanket Bharadwaj
echo.
echo ============================================================
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

:: ================= CONFIRM =================
color 0E
choice /C YN /M "Are you sure you want to uninstall Vyom?"
if errorlevel 2 (
    echo.
    echo Uninstallation cancelled.
    pause
    goto :end
)
color 0C

:: ================= STEP 1 =================
set /a CURRENT_STEP+=1
echo [1/4] Checking installation...
if not exist "%INSTALL_DIR%" (
    echo Vyom directory not found. Nothing to remove.
    goto :cleanup_path
)
timeout /t 1 >nul

:: ================= STEP 2 =================
set /a CURRENT_STEP+=1
echo [2/4] Removing files...

if exist "%INSTALL_DIR%\vyom.exe" del /Q "%INSTALL_DIR%\vyom.exe" >nul 2>&1
if exist "%INSTALL_DIR%\examples" rmdir /S /Q "%INSTALL_DIR%\examples" >nul 2>&1
rmdir /S /Q "%INSTALL_DIR%" >nul 2>&1

timeout /t 1 >nul

:cleanup_path
:: ================= STEP 3 =================
set /a CURRENT_STEP+=1
echo [3/4] Cleaning PATH...

echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %errorlevel% equ 0 (
    powershell -Command ^
    "$p=[Environment]::GetEnvironmentVariable('Path','User'); ^
     $n=($p.Split(';') | Where-Object { $_ -ne 'C:\Vyom' }) -join ';'; ^
     [Environment]::SetEnvironmentVariable('Path',$n,'User')" >nul 2>&1
)

timeout /t 1 >nul

:: ================= STEP 4 =================
set /a CURRENT_STEP+=1
timeout /t 1 >nul

:: ================= DONE =================
cls
color 0A
echo ============================================================
echo   Vyom v0.7 Uninstalled Successfully
echo ============================================================
echo.
echo Removed:
echo   [x] C:\Vyom directory
echo   [x] vyom.exe
echo   [x] example programs
echo   [x] PATH entry
echo.
echo IMPORTANT:
echo   - Close this terminal
echo   - Open a NEW terminal
echo.
echo Verify:
echo   vyom --version
echo   (should say command not found)
echo.
echo ============================================================
pause

:end
endlocal
@echo off
setlocal

:: Configuration: Debug or Release
set CONFIG=%1
set SOLUTION_DIR=%~2
set SOLUTION_DIR=%SOLUTION_DIR:"=%

set DLL_PATH=%SOLUTION_DIR%includes\scintilla\bin\Scintilla.dll
set DEST_DIR=%SOLUTION_DIR%setup

:: Determine if DEBUG=1 is needed
set DEBUG_FLAG=
if /I %CONFIG%=="Debug" (
    set DEBUG_FLAG=DEBUG=1
)

:: Go to the directory with the make file
pushd %SOLUTION_DIR%includes\scintilla\win32

:: Build
echo === Building Scintilla (%CONFIG%)...
nmake -s %DEBUG_FLAG% -f scintilla.mak
if errorlevel 1 (
    echo !!! Build failed
    popd
    exit /b 1
)

popd

:: Copying DLL
if exist "%DLL_PATH%" (
    echo === Copying Scintilla.dll to %DEST_DIR%...
    copy /Y "%DLL_PATH%" "%DEST_DIR%\"
) else (
    echo !!! DLL not found at %DLL_PATH%
    exit /b 1
)

echo === Done.
endlocal
exit /b 0

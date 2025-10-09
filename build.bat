@echo off
setlocal enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"
set "BUILD_DIR=%SCRIPT_DIR%\build"
set "EXIT_CODE=0"
set "CONFIG=Release"
set "CONFIG_FOUND="

if "%~1"=="" goto CONFIG_READY

for %%C in (Debug Release RelWithDebInfo MinSizeRel) do (
    if /I "%~1"=="%%C" (
        set "CONFIG=%%C"
        set "CONFIG_FOUND=1"
    )
)

if not defined CONFIG_FOUND (
    echo [ERROR] Unknown configuration "%~1". Use Debug, Release, RelWithDebInfo or MinSizeRel.
    set "EXIT_CODE=1"
    goto END
)

:CONFIG_READY
echo [INFO] Target configuration: %CONFIG%

if not exist "%BUILD_DIR%" (
    echo [INFO] Creating build directory "%BUILD_DIR%"
    mkdir "%BUILD_DIR%" >nul 2>&1
)

echo [INFO] Running CMake configure...
cmake -S "%SCRIPT_DIR%" -B "%BUILD_DIR%"
if errorlevel 1 (
    echo [ERROR] CMake configure failed.
    set "EXIT_CODE=!ERRORLEVEL!"
    goto END
)

echo [INFO] Building targets...
cmake --build "%BUILD_DIR%" --config %CONFIG%
set "BUILD_RESULT=!ERRORLEVEL!"
if not "!BUILD_RESULT!"=="0" (
    echo [ERROR] Build failed.
    set "EXIT_CODE=!BUILD_RESULT!"
    goto END
)

set "CTEST_ARGS=--output-on-failure"
set "CACHE_FILE=%BUILD_DIR%\CMakeCache.txt"
if exist "%CACHE_FILE%" (
    findstr /B /C:"CMAKE_CONFIGURATION_TYPES:STRING=" "%CACHE_FILE%" >nul
    if not errorlevel 1 (
        set "CTEST_ARGS=!CTEST_ARGS! -C %CONFIG%"
    )

    echo [INFO] Running tests: ctest !CTEST_ARGS!
    pushd "%BUILD_DIR%" >nul
    ctest !CTEST_ARGS!
    set "TEST_RESULT=!ERRORLEVEL!"
    popd >nul

    if not "!TEST_RESULT!"=="0" (
        echo [ERROR] Tests failed.
        set "EXIT_CODE=!TEST_RESULT!"
        goto END
    )
) else (
    echo [WARN] CMakeCache.txt not found. Skipping tests.
)

echo [INFO] Build pipeline completed.

:END
echo.
echo [INFO] Exit code: %EXIT_CODE%
echo Press any key to close...
pause >nul
exit /b %EXIT_CODE%

@echo off
set "PATH=%W64DEVKIT_HOME%\bin;%PATH%"
title Compilador Proyecto Parqueadero

echo ========================================================
echo  COMPILADOR PROYECTO PARQUEADERO (C++ y DLL)
echo ========================================================
echo.

set "COMPILER="

:: 1. Comprobar si g++ esta directamente disponible en el PATH
where g++ >nul 2>&1
if not errorlevel 1 set "COMPILER=g++"
if defined COMPILER goto HACER_COMPILACION

:: 2. Buscar en rutas habituales (C:\MinGW, CodeBlocks, MSYS2)
if exist "C:\MinGW\bin\g++.exe" set "COMPILER=C:\MinGW\bin\g++.exe"
if defined COMPILER goto HACER_COMPILACION

if exist "C:\Program Files\CodeBlocks\MinGW\bin\g++.exe" set "COMPILER=C:\Program Files\CodeBlocks\MinGW\bin\g++.exe"
if defined COMPILER goto HACER_COMPILACION

if exist "C:\msys64\ucrt64\bin\g++.exe" set "COMPILER=C:\msys64\ucrt64\bin\g++.exe"
if defined COMPILER goto HACER_COMPILACION

if exist "C:\msys64\mingw64\bin\g++.exe" set "COMPILER=C:\msys64\mingw64\bin\g++.exe"
if defined COMPILER goto HACER_COMPILACION

:: 3. Si no se encontro ningun g++ instalado
echo [ALERTA] No se encontro el compilador g++ en tu equipo.
echo.
echo Para solucionarlo en 1 minuto de forma rapida y segura:
echo   1. Descarga el compilador portable:
echo      https://github.com/skeeto/w64devkit/releases (w64devkit.zip)
echo   2. Descomprime la carpeta en C:\MinGW
echo      (debe quedar el archivo C:\MinGW\bin\g++.exe)
echo   3. Vuelve a ejecutar este build.bat.
echo.
echo NOTA: Tambien puedes compilar usando Python ejecutando:
echo       python compilar.py
echo.
pause
exit /b 1

:HACER_COMPILACION
echo [DETECTADO] Usando compilador: %COMPILER%
echo.
echo [1/2] Compilando parqueadero_bridge.dll...
"%COMPILER%" -std=c++17 -shared -O2 parqueadero_bridge.cpp -o parqueadero_bridge.dll -lws2_32
if errorlevel 1 goto ERROR_DLL

echo [2/2] Compilando cliente_generador.exe...
"%COMPILER%" -std=c++17 cliente_generador.cpp -o cliente_generador.exe -lws2_32
if errorlevel 1 goto ERROR_CLIENTE

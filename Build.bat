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

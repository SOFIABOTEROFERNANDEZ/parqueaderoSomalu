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

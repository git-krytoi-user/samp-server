@ECHO off
SETLOCAL EnableExtensions
SETLOCAL EnableDelayedExpansion
CD /d "%~dp0"
SET DIR=%~dp0

REM
FOR /F "tokens=1,2,3 delims=/ " %%a IN ('DATE/T') DO SET CURRENT_DATE=%%c%%a%%b

REM
MKDIR releases >nul 2>&1

REM
"%cd%\compiler\pawncc.exe" -;+ -(+ -w=217 -v=2 -d=3 -i="%cd%\compiler\include" "%cd%\sources\new.pwn"
MKDIR "releases\%CURRENT_DATE% build" >nul 2>&1
MOVE /Y new.amx "releases\%CURRENT_DATE% compiler\new.amx" >nul 2>&1

timeout /t 2
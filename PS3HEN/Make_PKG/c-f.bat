@echo off
setlocal enabledelayedexpansion

rem C:\PS3HEN-master\Make_PKG\4.91\dev_rewrite\vsh\module\ftp.sprx

for /D %%A in (4.*) do (
set nm=%%A
set pp=%1
set pt=!pp:~0,26!!nm!!pp:~30,80!
xcopy /Y /I !pp! !pt!
)

@echo off
setlocal enabledelayedexpansion

FOR /L %%A IN (80,1,92) DO (
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\PS3HEN-en\Make_PKG\4.%%A\dev_hdd0\hen\m\l\HEN.BIN
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\PS3HEN-en\Make_PKG\4.%%Awm\dev_hdd0\hen\m\l\HEN.BIN
)

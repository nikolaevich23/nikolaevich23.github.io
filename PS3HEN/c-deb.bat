@echo off
setlocal enabledelayedexpansion

FOR /L %%A IN (80,1,93) DO (
xcopy /Y PS3HEN.BIN_CEX_4%%A_DEBUG C:\PS3HEN-en\Make_PKG\4.%%A\dev_hdd0\hen\m\d\HEN.BIN
xcopy /Y PS3HEN.BIN_CEX_4%%A_DEBUG C:\PS3HEN-en\Make_PKG\4.%%Awm\dev_hdd0\hen\m\d\HEN.BIN
)



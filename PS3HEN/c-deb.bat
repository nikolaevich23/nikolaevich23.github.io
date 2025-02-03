@echo off
setlocal enabledelayedexpansion

FOR /L %%A IN (80,1,91) DO (
xcopy /Y PS3HEN.BIN_CEX_4%%A_DEBUG C:\PS3HEN-master\Make_PKG\4.%%A\dev_hdd0\hen\mode\debug\PS3HEN.BIN
xcopy /Y PS3HEN.BIN_CEX_4%%A_DEBUG C:\PS3HEN-master\Make_PKG\4.%%Awm\dev_hdd0\hen\mode\debug\PS3HEN.BIN
)



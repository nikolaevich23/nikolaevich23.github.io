@echo off
setlocal enabledelayedexpansion

FOR /L %%A IN (80,1,91) DO (
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\PS3HEN-master\Make_PKG\4.%%A\dev_rewrite\hen\LITERU.BIN
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\PS3HEN-master\Make_PKG\4.%%A\dev_hdd0\hen\mode\lite\PS3HEN.BIN
)

@echo off
setlocal enabledelayedexpansion

FOR /L %%A IN (80,1,93) DO (
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\PS3HEN-en\Make_PKG\4.%%A\dev_rewrite\hen\PS3HEN.BIN
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\PS3HEN-en\Make_PKG\4.%%A\dev_hdd0\hen\m\r\HEN.BIN
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\Github\nikolaevich23.github.io\alt\4.%%A\PS3HEN.p3t
)

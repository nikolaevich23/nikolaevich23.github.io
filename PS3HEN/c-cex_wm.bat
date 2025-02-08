@echo off
setlocal enabledelayedexpansion

FOR /L %%A IN (80,1,92) DO (
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\PS3HEN-en\Make_PKG\4.%%Awm\dev_rewrite\hen\PS3HEN.BIN
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\PS3HEN-en\Make_PKG\4.%%Awm\dev_hdd0\hen\m\r\HEN.BIN
xcopy /Y PS3HEN.BIN_CEX_4%%A C:\Github\nikolaevich23.github.io\wm\4.%%A\PS3HEN.p3t
)


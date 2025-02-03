@echo off
setlocal EnabledelayedExpansion

:: ----------------------------------------------
:: Simple script to build a PKG (by CaptainCPS-X)
:: ----------------------------------------------

:: Change these for your application / manual...
set CID=CUSTOM-INSTALLER_00-0000000000000000
if exist for_4.91_latest_rus_sign.pkg del /Q for_4.91_latest_rus_sign.pkg

for /D %%A in (4.91) do (
set nm=%%A
make_package_custom.exe --contentid %CID% ./%%A/ for_!nm!_latest_rus.pkg
echo | ps3xploit_rifgen_edatresign for_!nm!_latest_rus.pkg ps3 >> NUL
del /q for_!nm!_latest_rus.pkg
ren for_!nm!_latest_rus.pkg_signed.pkg for_!nm!_latest_rus_sign.pkg
if exist "for_!nm!_latest_rus.pkg" echo !nm! PKG done
echo.
)

ftp -n -s:ftp.dat "10.5.1.45"

rem pause
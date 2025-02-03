@echo off
setlocal EnabledelayedExpansion

:: ----------------------------------------------
:: Simple script to build a PKG (by CaptainCPS-X)
:: ----------------------------------------------

:: Change these for your application / manual...
set CID=CUSTOM-INSTALLER_00-0000000000000000
if exist for_*.pkg del /Q for_*.pkg

for /D %%A in (4.*) do (
set nm=%%A
set wm=!nm:~0,4!
make_package_custom.exe --contentid %CID% ./%%A/ for_!nm!_latest_rus.pkg
echo | ps3xploit_rifgen_edatresign for_!nm!_latest_rus.pkg ps3 >> NUL
del /q for_!nm!_latest_rus.pkg
if !nm!==!wm!wm (
ren for_!nm!_latest_rus.pkg_signed.pkg for_!wm!_latest_rus_WMM_sign.pkg
) else ren for_!nm!_latest_rus.pkg_signed.pkg for_!nm!_latest_rus_sign.pkg
if exist "for_!nm!_latest_rus.pkg" echo !nm! PKG done
echo.
)
rem pause
@echo off
setlocal enabledelayedexpansion
rem задаем текущую прошивку. Тут для примера 4.92 отрбасываем 4. для приведения к единому формату
set /a l=92
rem задаем следующую прошивку
set /a n=l+1
if exist new-hen.done goto end

copy make_PS3HEN_rel_4!l!.bat make_PS3HEN_rel_4!n!.bat
copy make_PS3HEN_rel_4!l!_lite.bat make_PS3HEN_rel_4!n!_lite.bat
sfk replace make_PS3HEN_rel_4!n!.bat "/4!l!/4!n!/" -yes
sfk replace make_PS3HEN_rel_4!n!.bat "/*CEX_4!n!/*CEX_4!l! *CEX_4!n!/" -yes
sfk replace make_PS3HEN_rel_4!l!_lite.bat "/4!l!/4!n!/" -yes
sfk replace make_PS3HEN_rel_4!l!_lite.bat "/*CEX_4!n!/*CEX_4!l! *CEX_4!n!/" -yes

rem find string fw_version = L"4.92" in main.c;
for /f "usebackq" %%a in (`sfk echo !l! +encode -hex`) do set en=%%a
for /f "usebackq" %%k in (`sfk hexfind -binary /4c22342e!en!/ -firsthit -dir henplugin -file main.c +thead -lines=1 +getcol 6`) do set oset=%%k
set /a ofset=oset+0x24
sfk partcopy "./henplugin/main.c" 0x0 !ofset! "./main.c" -yes
echo		// 4.!n! CEX >>main.c
echo		// Kernel offset is off by 0x10 so we are checking this value instead of the timestamp>>main.c
echo		else if(val==0x323032362F30322FULL)>>main.c
echo		{                                  >>main.c
echo			fw_version = L"4.!n!";     >>main.c
echo			kernel_type = L"cex";      >>main.c
echo		}				>>main.c
sfk partcopy "./henplugin/main.c" -allfrom !ofset! "./main.c" -append -yes
move ./main.c ./henplugin/main.c

sfk replace lv2\include\lv2\symbols.h "/defined(FIRMWARE_4_!l!)/defined(FIRMWARE_4_!l!) || defined(FIRMWARE_4_!n!)/" -yes

rem for /f "usebackq" %%k in (`sfk hexfind -text "/Makefile_4!l!C_release/" -dir . -file Makefile +thead -lines=1 +getcol 6`) do set oset=%%k
copy .\payload\Mf .\payload\Makefile
sfk replace payload\Makefile "/4__/4!n!/" -yes

copy .\payload\Makefile_4!l!C_debug .\payload\Makefile_4!n!C_debug
sfk replace payload\Makefile_4!n!C_debug "/!l!/!n!/" -yes
copy .\payload\Makefile_4!l!C_release .\payload\Makefile_4!n!C_release
sfk replace payload\Makefile_4!n!C_release "/!l!/!n!/" -yes

sfk replace payload\storage_ext.c "/defined(FIRMWARE_4_!l!)/defined(FIRMWARE_4_!l!) || defined(FIRMWARE_4_!n!)/" -yes

for /f "usebackq" %%k in (`sfk hexfind -text "/0x04!l!/" -dir payload -file main.c +thead -lines=1 +getcol 6`) do set oset=%%k
set /a ofset=oset+0x8
sfk partcopy "./payload/main.c" 0x0 !ofset! "./main.c" -yes
echo #elif defined(FIRMWARE_4_!n!)>>main.c
echo 	#define FIRMWARE_VERSION	0x04!n!>>main.c
sfk partcopy "./payload/main.c" -allfrom !ofset! "./main.c" -append -yes
sfk replace main.c "/defined (FIRMWARE_4_!l!)/defined (FIRMWARE_4_!l!) || defined (FIRMWARE_4_!n!)/" -yes
move ./main.c ./payload/main.c

sfk replace payload\modulespatch.c "/defined(FIRMWARE_4_!l!)/defined(FIRMWARE_4_!l!) || defined(FIRMWARE_4_!n!)/" -yes
sfk replace payload\modulespatch.h "/defined(FIRMWARE_4_!l!)/defined(FIRMWARE_4_!l!) || defined(FIRMWARE_4_!n!)/" -yes

for /f "usebackq" %%k in (`sfk hexfind -text /defined(FIRMWARE_4_!l!/ -firsthit -dir payload -file ps3mapi_core.h +thead -lines=1 +getcol 6`) do set oset=%%k
set /a ofset=oset+0x159
sfk partcopy "./payload/ps3mapi_core.h" 0x0 !ofset! "./ps3mapi_core.h" -yes
echo f>>ps3mapi_core.h
echo.>>ps3mapi_core.h
echo #if defined(FIRMWARE_4_!n!)>>ps3mapi_core.h
echo 	#define PS3MAPI_FW_VERSION			 		0x04!n!>>ps3mapi_core.h
echo 	#if defined(IS_MAMBA)>>ps3mapi_core.h
echo 		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA">>ps3mapi_core.h
echo 	#else>>ps3mapi_core.h
echo 		#define PS3MAPI_FW_TYPE			 		"CEX COBRA">>ps3mapi_core.h
echo 	#endif>>ps3mapi_core.h
echo 	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL>>ps3mapi_core.h
echo 	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL>>ps3mapi_core.h
echo 	#define PS3MAPI_PSID						0x8000000000474B0CULL>>ps3mapi_core.h
echo #endif>>ps3mapi_core.h

set /a ofset=ofset+0x2
sfk partcopy "./payload/ps3mapi_core.h" -allfrom !ofset! "./ps3mapi_core.h" -append -yes
move ./ps3mapi_core.h ./payload/ps3mapi_core.h

copy .\stage0_file\Makefile_4!l!C .\stage0_file\Makefile_4!n!C
sfk replace .\stage0_file\Makefile_4!n!C "/!l!/!n!/" -yes

for /f "usebackq" %%k in (`sfk hexfind -text "/*.self *.4!l!/" -dir stage0_file -file Makefile +thead -lines=1 +getcol 6`) do set oset=%%k
set /a ofset=oset+0x48
sfk partcopy "./stage0_file/Makefile" 0x0 !ofset! "./Makefile" -yes
echo		rm -f *.o *.elf *.self *.4!n! *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile

for /f "usebackq" %%k in (`sfk hexfind -text "/*.elf *.4!l!/" -dir stage0_file -file Makefile +thead -lines=1 +getcol 6`) do set oset2=%%k
set /a ofset2=oset2+0x48
sfk partcopy "./stage0_file/Makefile" -fromto !ofset! !ofset2! "./Makefile" -append -yes
echo.>>Makefile
echo		rm -f *.o *.elf *.self *.4!n! *.bin *.4!n!C *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo		make -f Makefile_4!n!C --no-print-directory>>Makefile
echo		rm -f *.o *.bin *.elf *.4!n! *.4!n!c *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo.>>Makefile
sfk partcopy "./stage0_file/Makefile" -allfrom !ofset2! "./Makefile" -append -yes
for /f "usebackq" %%k in (`sfk hexfind -text "/4!l!:/" -dir stage0_file -file Makefile +thead -lines=1 +getcol 6`) do set oset3=%%k
set /a ofset3=oset3+0x5
echo.>>Makefile
echo 4!n!:>>Makefile
echo 	rm -f *.o *.elf *.self *.4!n! *.bin *.4!n!C *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo 	make -f Makefile_4!n!C --no-print-directory>>Makefile
echo 	rm -f *.o *.bin *.elf *.4!n! *.4!n!c *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
move ./Makefile ./stage0_file/Makefile

echo done > new-hen.done
:end
echo done

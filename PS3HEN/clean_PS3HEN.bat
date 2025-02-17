@echo off
cls
set PS3SDK=/c/PSDK3v2
set WIN_PS3SDK=C:/PSDK3v2
set PS3DEV=%PS3SDK%/ps3dev2
set PATH=%WIN_PS3SDK%/mingw/msys/1.0/bin;%WIN_PS3SDK%/mingw/bin;%PS3DEV%/ppu/bin;
set CYGWIN=C:\PSDK3v2\MinGW\msys\1.0\bin

mv *DEX_482_DEBUG *DEX_484_DEBUG *CEX_480_DEBUG *CEX_481_DEBUG *CEX_482_DEBUG *CEX_483_DEBUG *CEX_484_DEBUG *CEX_485_DEBUG *CEX_486_DEBUG *CEX_487_DEBUG *CEX_488_DEBUG *CEX_489_DEBUG *CEX_490_DEBUG *CEX_491_DEBUG *CEX_492_DEBUG PS3HEN_GEN
mv *DEX_484 *CEX_480 *CEX_481 *CEX_482 *CEX_483 *CEX_484 *CEX_485 *CEX_486 *CEX_487 *CEX_488 *CEX_489 *CEX_490 *CEX_491 *CEX_492 PS3HEN_GEN
cd henplugin
%CYGWIN%\bash -i -c 'make clean; rm -r objs; rm henplugin.prx; rm henplugin.sym; mv henplugin.sprx ../PS3HEN_GEN;'
cd ../stage0_file
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile clean
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ../payload
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile clean
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o


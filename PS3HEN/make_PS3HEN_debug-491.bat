@echo off
cls
set PS3SDK=/c/PSDK3v2
set WIN_PS3SDK=C:/PSDK3v2
set PS3DEV=%PS3SDK%/ps3dev2
set PATH=%WIN_PS3SDK%/mingw/msys/1.0/bin;%WIN_PS3SDK%/mingw/bin;%PS3DEV%/ppu/bin;
set CYGWIN=C:\PSDK3v2\MinGW\msys\1.0\bin

mv *DEX_482_DEBUG *DEX_484_DEBUG *CEX_480_DEBUG *CEX_481_DEBUG *CEX_482_DEBUG *CEX_483_DEBUG *CEX_484_DEBUG *CEX_485_DEBUG *CEX_486_DEBUG *CEX_487_DEBUG *CEX_488_DEBUG *CEX_489_DEBUG *CEX_490_DEBUG *CEX_491_DEBUG PS3HEN_GEN
cd henplugin
cp bildtype.d bildtype.h
%CYGWIN%\bash -i -c 'make debug; rm -r objs; rm henplugin.prx; rm henplugin.sym; mv henplugin.sprx ../PS3HEN_GEN;'
cd ../stage0_file
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile 491
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ../payload
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile debug_491
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ..
mv *491C PS3HEN_GEN
cd PS3HEN_GEN
gcc main.c -o HEN_GEN
HEN_GEN PS3HEN.BIN_CEX_491_DEBUG stage2.bin_debug_491C stage0.bin_491C HENplugin.sprx
rm stage* *.sprx *.exe
mv PS3HEN* ../
pause
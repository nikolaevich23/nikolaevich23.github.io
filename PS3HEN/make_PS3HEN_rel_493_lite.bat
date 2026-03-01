@echo off
cls
set PS3SDK=/c/PSDK3v2
set WIN_PS3SDK=C:/PSDK3v2
set PS3DEV=%PS3SDK%/ps3dev2
set MSYS=%PS3SDK%/mingw/msys/1.0;
set PATH=%WIN_PS3SDK%/mingw/msys/1.0/bin;%WIN_PS3SDK%/mingw/bin;%PS3DEV%/ppu/bin;%WIN_PS3SDK%/mingw/msys/1.0;
set CYGWIN=C:\PSDK3v2\MinGW\msys\1.0\bin

mv *CEX_480 *CEX_481 *CEX_482 *CEX_483 *CEX_484 *CEX_485 *CEX_486 *CEX_487 *CEX_488 *CEX_489 *CEX_490 *CEX_491 *CEX_492 PS3HEN_GEN
cd henplugin
cp bildtype.l bildtype.h
%CYGWIN%\bash -i -c 'make release; rm -r objs; rm henplugin.prx; rm henplugin.sym; mv henplugin.sprx ../PS3HEN_GEN;'
cd ../stage0_file
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile 492
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ../payload
cp lite.on lite.h
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile release_492
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ..
mv *492C PS3HEN_GEN
cd PS3HEN_GEN
gcc main.c -o HEN_GEN
HEN_GEN PS3HEN.BIN_CEX_492 stage2.bin_492C stage0.bin_492C HENplugin.sprx
rm stage* *.sprx *.exe
mv PS3HEN* ../

pause
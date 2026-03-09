@echo off
setlocal enabledelayedexpansion
set /a l=93

echo clean:>Makefile
FOR /L %%A IN (80,1,!l!) DO (
echo 	rm -f *.o *.elf *.self *.4%%A *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
)
echo all:>>Makefile
FOR /L %%A IN (80,1,!l!) DO (
echo 	rm -f *.o *.elf *.self *.4%%A *.bin *.4%%AC *.4%%AD *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo 	make -f Makefile_4%%AC --no-print-directory>>Makefile
echo 	rm -f *.o *.bin *.elf *.4%%A *.4%%Ac *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo.>>Makefile
if %%A==82 (
echo 	rm -f *.o *.elf *.self *.482 *.bin *.482C *.482D *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo 	make -f Makefile_482D --no-print-directory>>Makefile
echo 	rm -f *.o *.bin *.elf *.482 *.482d *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo.>>Makefile
)
if %%A==84 (
echo 	rm -f *.o *.elf *.self *.484 *.bin *.484D *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo 	make -f Makefile_484D --no-print-directory>>Makefile
echo 	rm -f *.o *.bin *.elf *.484 *.484d *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo.>>Makefile
)
)
FOR /L %%A IN (90,1,!l!) DO (
echo 4%%A:>>Makefile
echo 	rm -f *.o *.elf *.self *.4%%A *.bin *.4%%AC *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
echo 	make -f Makefile_4%%AC --no-print-directory>>Makefile
echo 	rm -f *.o *.bin *.elf *.4%%A *.4%%Ac *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o>>Makefile
)

move .\Makefile .\stage0_file\Makefile
:end
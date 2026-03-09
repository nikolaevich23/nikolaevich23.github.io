@echo off
setlocal enabledelayedexpansion
set /a l=93

echo clean:>Makefile
echo 	@rm -f *~ *.*~ *.o *.elf *.cex *.dex *.release *.debug *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo all:>>Makefile
FOR /L %%A IN (80,1,!l!) DO (
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AC_release --no-print-directory>>Makefile
echo.>>Makefile
)
FOR /L %%A IN (80,1,!l!) DO (
echo 	@rm -f *.o *.elf *.bin *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AC_debug --no-print-directory>>Makefile
echo.>>Makefile
if %%A==82 (
echo 	@rm -f *.o *.elf *.bin *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AD_debug --no-print-directory>>Makefile
echo.>>Makefile
)
if %%A==84 (
echo 	@rm -f *.o *.elf *.bin *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AD_debug --no-print-directory>>Makefile
echo.>>Makefile
)
)
echo release:>>Makefile
FOR /L %%A IN (80,1,!l!) DO (
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AC_release --no-print-directory>>Makefile
echo.>>Makefile
if %%A==82 (
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AD_release --no-print-directory>>Makefile
echo.>>Makefile
)
if %%A==84 (
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AD_release --no-print-directory>>Makefile
echo.>>Makefile
)
)
echo debug:>>Makefile
FOR /L %%A IN (80,1,!l!) DO (
echo 	@rm -f *.o *.elf *.bin *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AC_debug --no-print-directory>>Makefile
echo.>>Makefile
if %%A==82 (
echo 	@rm -f *.o *.elf *.bin *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AD_debug --no-print-directory>>Makefile
echo.>>Makefile
)
if %%A==84 (
echo 	@rm -f *.o *.elf *.bin *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AD_debug --no-print-directory>>Makefile
echo.>>Makefile
)
)
echo debug_480:>>Makefile
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_480C_debug --no-print-directory>>Makefile
echo.>>Makefile

FOR /L %%A IN (90,1,!l!) DO (
echo debug_4%%A:>>Makefile
echo 	@rm -f *.o *.elf *.bin *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_4%%AC_debug --no-print-directory>>Makefile
echo.>>Makefile
)

FOR /L %%A IN (90,1,!l!) DO (
echo release_4%%A:>>Makefile
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_481C_release --no-print-directory>>Makefile
echo.>>Makefile
)

echo debug_482dex:>>Makefile
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_482D_debug --no-print-directory>>Makefile
echo.>>Makefile
echo release_482dex:>>Makefile
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_482D_release --no-print-directory>>Makefile
echo.>>Makefile

echo debug_484dex:>>Makefile
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile
echo 	@make -f Makefile_484D_debug --no-print-directory>>Makefile
echo.>>Makefile	
echo release_484dex:>>Makefile 
echo 	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o>>Makefile 
echo 	@make -f Makefile_484D_release --no-print-directory>>Makefile 
echo.>>Makefile

move .\Makefile .\payload\Makefile
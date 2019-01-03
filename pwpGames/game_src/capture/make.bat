@echo off

if "%1" == "LCD"   goto pwplcd
if "%1" == "CRT"   goto pwpcrt
if "%1" == "CLEAN" goto clean
if "%1" == "clean" goto clean

goto  usage


:pwplcd
ECHO Tidying up...
if exist *.obj del *.obj
if exist *.src del *.src
if exist *.asm del *.asm
if exist *.bin del *.bin
if exist *.lcd del *.lcd
..\..\tools\nmake -f makefile.pwp LCD=
rename capture.bin capture.lcd
dir  capture.lcd
goto end


:pwpcrt
ECHO Tidying up...
if exist *.obj del *.obj
if exist *.src del *.src
if exist *.asm del *.asm
if exist *.bin del *.bin
if exist *.crt del  *.crt
..\..\tools\nmake -f makefile.pwp CRT=
rename capture.bin capture.crt
dir  capture.crt
goto end


:clean
ECHO Tidying up...
if exist *.obj del *.obj
if exist *.src del *.src
if exist *.pix del *.pix
if exist *.asm del *.asm
if exist *.bin del *.bin
if exist *.abs del *.abs
goto end


:usage
echo Usage: make {CRT, LCD, clean}


:end

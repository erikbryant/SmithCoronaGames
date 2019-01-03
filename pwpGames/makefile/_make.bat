@echo off

rem
rem BAT file to build any game.
rem

rem
rem See what the user wants to do.
rem
if "%1" == "CRT"   goto doit
if "%1" == "LCD"   goto doit
if "%1" == "CLEAN" goto clean

if "%1" == "clean" goto clean

goto :usage



:doit
rem
rem Make sure the image files are up to date.
rem
cd    bitmaps
echo  Making image files...
call  make
cd    ..

rem
rem Build the file that the user specified.
rem
echo  Making main program(s)...

rem
rem Clean up anything that could be left over from an aborted build.
rem
if exist *.obj del *.obj > NUL
if exist *.src del *.src > NUL
if exist *.bin del *.bin > NUL
if exist *.ab? del *.ab? > NUL
if exist *.sys del *.sys > NUL

rem
rem Copy the appropriate files up from the appropriate directory.
rem Make does not deal well with targets being in other directories.
rem
if exist %1_obj\*.obj copy %1_obj\*.obj > NUL
if exist %1_obj\*.src copy %1_obj\*.src > NUL
if exist %1_obj\*.bin copy %1_obj\*.bin > NUL
if exist %1_obj\*.ab? copy %1_obj\*.ab? > NUL
if exist %1_obj\*.sys copy %1_obj\*.sys > NUL

rem
rem Invoke Make, telling it what to build.
rem
..\..\tools\nmake /NOLOGO -f makefile. %3 %4 %5 %6 %7 %1=

rem
rem Copy the built files back into their directory.
rem
if exist *.obj copy *.obj %1_obj\*.obj > NUL
if exist *.src copy *.src %1_obj\*.src > NUL
if exist *.bin copy *.bin %1_obj\*.%1  > NUL
if exist *.ab? copy *.ab? %1_obj\*.ab? > NUL
if exist *.sys copy *.sys %1_obj\*.sys > NUL

rem
rem Clean up after the build.
rem
if exist *.obj del *.obj > NUL
if exist *.src del *.src > NUL
if exist *.exe del *.exe > NUL
if exist *.bin del *.bin > NUL
if exist *.ab? del *.ab? > NUL
if exist *.sys del *.sys > NUL

goto  :exit



rem
rem Clean up any files that might be left from an aborted build.
rem
:clean
if exist *.obj echo Deleting *.OBJ...
if exist *.obj del *.obj > NUL
if exist crt_obj\*.obj del crt_obj\*.obj > NUL
if exist lcd_obj\*.obj del lcd_obj\*.obj > NUL
if exist *.src echo Deleting *.SRC...
if exist *.src del *.src > NUL
if exist crt_obj\*.src del crt_obj\*.src > NUL
if exist lcd_obj\*.src del lcd_obj\*.src > NUL
if exist *.bin echo Deleting *.BIN...
if exist *.bin del *.bin > NUL
if exist *.pix echo Deleting *.PIX...
if exist *.pix del *.pix > NUL
if exist bitmaps\*.pix del bitmaps\*.pix > NUL
if exist *.map echo Deleting *.MAP...
if exist *.map del *.map > NUL
if exist crt_obj\*.map del crt_obj\*.map > NUL
if exist lcd_obj\*.map del lcd_obj\*.map > NUL
if exist *.ab? echo Deleting *.AB?...
if exist *.ab? del   *.ab? > NUL
if exist crt_obj\*.ab? del crt_obj\*.ab? > NUL
if exist lcd_obj\*.ab? del lcd_obj\*.ab? > NUL
if exist *.sys echo Deleting *.SYS...
if exist *.sys del   *.sys > NUL
if exist crt_obj\*.sys del crt_obj\*.sys > NUL
if exist lcd_obj\*.sys del lcd_obj\*.sys > NUL
goto :exit




:usage
echo Usage: %0 op
echo        where op = {CRT, LCD, or CLEAN}
goto :exit




:exit

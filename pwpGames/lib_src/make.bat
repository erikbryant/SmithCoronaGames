@echo off


rem
rem See if the user wants to delete all the old files or
rem update the old ones.
rem
if "%1" == ""      goto usage
if "%1" == "clean" goto clean
if "%1" == "CLEAN" goto clean


rem
rem If anything is left around from an aborted build then clean it up.
rem
if exist *.obj del  *.obj > NUL
if exist *.src del  *.src > NUL

rem
rem Copy the appropriate files up from their directory.  Make does
rem not deal well with files that are in other directories.
rem
if exist %1_obj\*.obj copy %1_obj\*.obj . > NUL
if exist %1_obj\*.src copy %1_obj\*.src . > NUL

rem
rem Invoke make.
rem
..\tools\nmake /NOLOGO %2 %3 %4 %5 %6 %7 %1=

rem
rem Copy the files back into their directory.
rem
if exist *.obj copy *.obj %1_obj\*.* > NUL
if exist *.src copy *.src %1_obj\*.* > NUL

rem
rem Clean up the files.
rem
if exist *.obj del  *.obj > NUL
if exist *.src del  *.src > NUL

goto exit



rem
rem Clean up all of the object files that could be lying around anywhere.
rem
:clean
if exist *.obj del *.obj > NUL
if exist *.src del *.src > NUL
if exist *.pix del *.pix > NUL
if exist crt_obj\*.obj del CRT_OBJ\*.obj > NUL
if exist crt_obj\*.src del CRT_OBJ\*.src > NUL
if exist lcd_obj\*.obj del LCD_OBJ\*.obj > NUL
if exist lcd_obj\*.src del LCD_OBJ\*.src > NUL
goto exit



:usage
echo Specify one of {CRT, LCD, CLEAN}
goto exit



:exit
exit

@echo off
if "%1"=="" goto USAGE
Echo test > %1\test.dat
if exist %1\test.dat goto INSTALL

        Echo Insure the installation directory is correct and try again.
        goto END

:INSTALL
        echo Unarchiving files...
        erase %1\test.dat > nul
        \pkunzip -d -o \pwpgames.zip %1
        goto END

:USAGE
        Echo install [drive letter]:[path to installation directory]

:END

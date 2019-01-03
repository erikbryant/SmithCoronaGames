#
# This is the name (without extension) of the project to be built.
#

PROJECT = phrase

#
# These are the names (with extensions) of the files that are included
# by the PROJECT, and that need to be built.
#

INCLUDES = phr_crt.hlp phr_lcd.hlp phrases.dat

#
# If there are any INCLUDES that have to be built with a special
# program, put those rules here, following the "all" rule.
#

all: $(PROJECT)

PHGEN    = ..\..\tools\phgen

phrases.dat: phrase.cnv
        $(PHGEN) phrase.cnv phrases.dat

!INCLUDE ..\..\makefile\makefile.inc

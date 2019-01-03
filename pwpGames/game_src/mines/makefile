#
# This is the name (without extension) of the project to be built.
#

PROJECT = mines

#
# These are the names (with extensions) of the files that are included
# by the PROJECT, and that need to be built.
#

INCLUDES = mine_crt.hlp mine_lcd.hlp boardlcd.h boardcrt.h

#
# If there are any INCLUDES that have to be built with a special
# program, put those rules here, following the "all" rule.
#

all: $(PROJECT)

BOARDLCD    = ..\..\tools\boardlcd
BOARDCRT    = ..\..\tools\boardcrt

boardlcd.h:  rectangl.lcd waffle.lcd   truck.lcd    ship.lcd     arrow.lcd   \
             castle.lcd   train.lcd
   $(BOARDLCD) boardlcd.lis boardlcd.h

boardcrt.h:  island.crt   robot.crt    pirate.crt   castle.crt   pillars.crt \
             pyramid.crt  yacht.crt    shades.crt   tree.crt     hills.crt   \
             tiles.crt    beacon.crt   plane.crt
   $(BOARDCRT) boardcrt.lis boardcrt.h

!INCLUDE ..\..\makefile\makefile.inc
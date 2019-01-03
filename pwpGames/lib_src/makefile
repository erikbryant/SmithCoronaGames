#
# MAKEFILE. for building the library files.
#

#
# Set up paths to needed executables.
#
BMPTOPIX  = ..\tools\bmptopix

#
# Set up the target files.
#
TARGETS   = DOWNHEAD.OBJ PWP.OBJ INP.OBJ GR.OBJ\
            FONTLIB.OBJ HELPLIB.OBJ SPLASH.OBJ WINDOW.OBJ

PIXMAPS   = SCLGCRT.PIX SCLGLCD.PIX ETILGCRT.PIX ETILGLCD.PIX

#
# Set up the compiler variables for the CRT.
#
!IFDEF CRT
CC        = mcc68k -B
OPTIMIZE  = -Oa -Ot
OUTPUT    = -S -nQ # -Fsm
CCOPT     = -DPWP -DCRT -DCRTD $(OPTIMIZE) $(OUTPUT)
ASM       = asm68k
!ENDIF

#
# Set up the compiler variables for the LCD.
#
!IFDEF LCD
CC        = mcc68k -B
OPTIMIZE  = -Oa -Ot
OUTPUT    = -Fsm -S
CCOPT     = -DPWP -DLCD $(OPTIMIZE) $(OUTPUT)
ASM       = asm68k
!ENDIF

#
#*****************************
#

all: $(PIXMAPS) $(TARGETS)

sclgcrt.pix: sclgcrt.bmp
    $(BMPTOPIX) -i $*.bmp $*.pix

#
# Rules.
#

.SUFFIXES: .C .A68 .BMP

.C.OBJ:
    $(CC) $(CCOPT) $*.c
    $(ASM) $(ASMOPT) $*.src > NUL

.SRC.OBJ:
    $(ASM) $(ASMOPT) $*.src > NUL

.A68.OBJ:
    $(ASM) $(ASMOPT) $*.a68 > NUL

.BMP.PIX:
    $(BMPTOPIX) $*.bmp $*.pix


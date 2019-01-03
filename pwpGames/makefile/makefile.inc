#
# Set up paths to needed executables.
#
BIN_PATH   = ..\..\tools

ABS2FIX    = $(BIN_PATH)\abs2fix
BMPTOPCK   = $(BIN_PATH)\bmptopck

#
# Defines for both machines.
#
C_INC      = -I ..\..\lib_src -I .\bitmaps

#
# Defines for the PWPs.
#
CC         = mcc68k -B -nQ
OPTIMIZE   = -Oa -Ot
OUTPUT     = -Fsm -S

ASM        = asm68k

LINK       = lnk68k
LINKOPT_0  = -c ..\..\lib_src\downzero.lnk -o $(PROJECT).ab0
LINKOPT_1  = -c ..\..\lib_src\downone.lnk  -o $(PROJECT).ab1

!IFDEF CRT
CCOPT      = -DPWP -DCRT $(OPTIMIZE) $(OUTPUT) $(C_INC)
LINKOPT_S  = -c ..\..\lib_src\downsys.crt  -o $(PROJECT).sys
!ENDIF

!IFDEF LCD
CCOPT      = -DPWP -DLCD $(OPTIMIZE) $(OUTPUT) $(C_INC)
LINKOPT_S  = -c ..\..\lib_src\downsys.lcd  -o $(PROJECT).sys
!ENDIF

#
# ****************** Dependencies ************************
#
#
# Build the final BIN file by creating the two ABS files and
# running them through the fix program...
#
$(PROJECT):      $(PROJECT).bin

$(PROJECT).bin:  $(PROJECT).ab1
   $(ABS2FIX) $(PROJECT).ab0 $(PROJECT).ab1 $(PROJECT).bin 65536 > NUL

$(PROJECT).ab1:  $(PROJECT).ab0
   $(LINK) $(LINKOPT_1) $(PROJECT).sys > NUL

$(PROJECT).ab0:  $(PROJECT).sys
   $(LINK) $(LINKOPT_0) $(PROJECT).sys > NUL

$(PROJECT).sys:  $(PROJECT).obj
   $(LINK) $(LINKOPT_S) $(PROJECT).obj > NUL

$(PROJECT).obj:  $(PROJECT).src
   $(ASM) $(ASMOPT) $*.src > NUL

$(PROJECT).src:  $(INCLUDES) $(PROJECT).c
   $(CC)  $(CCOPT) $*.c

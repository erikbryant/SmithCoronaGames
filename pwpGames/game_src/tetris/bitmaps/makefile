#
# Make all of the PIX files.
#
PIXMAPS = tetlc1.pix tetlc2.pix tetlc3.pix tetlc4.pix tetlc5.pix \
          tetlc6.pix tetlc7.pix tetpc1.pix tetpc2.pix tetpc3.pix \
          tetpc4.pix tetpc5.pix tetpc6.pix tetpc7.pix

#
# Make sure everything is up to date.
#
all: $(PIXMAPS)

#
# Paths.
#
BMPTOPIX = ..\..\..\tools\bmptopix

#
# Rules.
#
.SUFFIXES: .BMP
.BMP.PIX:
   @echo     Making: $*.PIX
   @$(BMPTOPIX) $*.bmp $*.pix

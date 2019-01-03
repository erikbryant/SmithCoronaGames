#
# Make all of the PIX files.
#
PIXMAPS = 100u.pix    100l.pix    200u.pix   200l.pix   300u.pix    300l.pix   \
          400u.pix    400l.pix    500u.pix   500l.pix   600u.pix    600l.pix   \
          700u.pix    700l.pix    800u.pix   800l.pix   900u.pix    900l.pix   \
          1000u.pix   1000l.pix   1500u.pix  1500l.pix  brokel.pix  brokeu.pix \
          freel.pix   freeu.pix   losel.pix  loseu.pix  arrow.pix              \
          d100u.pix   d100l.pix   d200u.pix  d200l.pix  d300u.pix   d300l.pix  \
          d400u.pix   d400l.pix   d500u.pix  d500l.pix  d600u.pix   d600l.pix  \
          d700u.pix   d700l.pix   d800u.pix  d800l.pix  d900u.pix   d900l.pix  \
          d1000u.pix  d1000l.pix  d1500u.pix d1500l.pix dbrokel.pix dbrokeu.pix\
          dfreel.pix  dfreeu.pix  dlosel.pix dloseu.pix darrow.pix

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
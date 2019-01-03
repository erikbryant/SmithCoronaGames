#
# Make all of the PIX files.
#
PIXMAPS =    0_lcd.pix    1_lcd.pix    2_lcd.pix    3_lcd.pix    \
             4_lcd.pix    5_lcd.pix    6_lcd.pix    7_lcd.pix    \
             8_lcd.pix    ptr_lcd.pix  mine_lcd.pix covr_lcd.pix \
             mark_lcd.pix test_lcd.pix wrng_lcd.pix              \
                                                                 \
             0.pix        1.pix        2.pix        3.pix        \
             4.pix        5.pix        6.pix        7.pix        \
             8.pix        pointer.pix  mine.pix                  \
                                                                 \
             cover_01.pix mark_01.pix  wrong_01.pix test_01.pix  \
             cover_02.pix mark_02.pix  wrong_02.pix test_02.pix  \
             cover_03.pix mark_03.pix  wrong_03.pix test_03.pix  \
             cover_04.pix mark_04.pix  wrong_04.pix test_04.pix  \
             cover_05.pix mark_05.pix  wrong_05.pix test_05.pix  \
             cover_06.pix mark_06.pix  wrong_06.pix test_06.pix  \
             cover_07.pix mark_07.pix  wrong_07.pix test_07.pix  \
             cover_08.pix mark_08.pix  wrong_08.pix test_08.pix  \
             cover_09.pix mark_09.pix  wrong_09.pix test_09.pix  \
             cover_10.pix mark_10.pix  wrong_10.pix test_10.pix  \
             cover_11.pix mark_11.pix  wrong_11.pix test_11.pix  \
             cover_12.pix mark_12.pix  wrong_12.pix test_12.pix  \
             cover_13.pix mark_13.pix  wrong_13.pix test_13.pix  \
             cover_14.pix mark_14.pix  wrong_14.pix test_14.pix  \
             cover_15.pix mark_15.pix  wrong_15.pix test_15.pix

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
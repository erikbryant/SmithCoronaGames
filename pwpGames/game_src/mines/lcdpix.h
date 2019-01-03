/*
** Define index values into the array of pixmaps.
*/
#define POINTER_PIXMAP   9
#define MINE_PIXMAP     10
#define COVERED_PIXMAP  11
#define MARKED_PIXMAP   12
#define TEST_PIXMAP     13
#define WRONG_PIXMAP    14

/*
** Include each of the pixmap files.
*/ 
UCHAR Pix_0[] = {
   #include "0_lcd.pix"
};

UCHAR Pix_1[] = {
   #include "1_lcd.pix"
};

UCHAR Pix_2[] = {
   #include "2_lcd.pix"
};

UCHAR Pix_3[] = {
   #include "3_lcd.pix"
};

UCHAR Pix_4[] = {
   #include "4_lcd.pix"
};

UCHAR Pix_5[] = {
   #include "5_lcd.pix"
};

UCHAR Pix_6[] = {
   #include "6_lcd.pix"
};

UCHAR Pix_7[] = {
   #include "7_lcd.pix"
};

UCHAR Pix_8[] = {
   #include "8_lcd.pix"
};

UCHAR Pix_9[] = {
   #include "ptr_lcd.pix"
};

UCHAR Pix_10[] = {
   #include "mine_lcd.pix"
};

UCHAR Pix_11[] = {
   #include "covr_lcd.pix"
};

UCHAR Pix_12[] = {
   #include "mark_lcd.pix"
};

UCHAR Pix_13[] = {
   #include "test_lcd.pix"
};

UCHAR Pix_14[] = {
   #include "wrng_lcd.pix"
};


/*
** Since this is the LCD model there is not a color screen.  This array
** therefore only has one dimension, the type of the pixmap you want
** to look up.
*/
UCHAR *Pixmaps[][1] = {
   { Pix_0  }, 
   { Pix_1  },
   { Pix_2  },
   { Pix_3  },
   { Pix_4  },
   { Pix_5  },
   { Pix_6  },
   { Pix_7  },
   { Pix_8  },
   { Pix_9  },
   { Pix_10 },
   { Pix_11 },
   { Pix_12 },
   { Pix_13 },
   { Pix_14 }
};

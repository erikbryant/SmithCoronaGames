/*****************************************************************************
**
**  Header Name:
**      gr.h
**
**  Description:
**      This is the header file for the PWP graphics module.
**
**  Notes:
**      none
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      01/28/92    Initial version
**
**      04/07/92    Several of the macro functions were moved from gr.c to gr.h
**                  This was to make them accessible to other apps
**
*****************************************************************************/

#ifndef GR_H_INCLUDED
#define GR_H_INCLUDED

/*
** Define the colors that may be used with the graphic functions.
*/

#define GR_NONE     -1     /* Does not affect background */
#define GR_BLACK     0     /* Solid black */
#define GR_PAT0      1
#define GR_PAT1      2
#define GR_PAT2      3
#define GR_PAT3      4
#define GR_PAT4      5
#define GR_PAT5      6
#define GR_PAT6      7
#define GR_GRAY      8     /* 50% gray */
#define GR_PAT7      9
#define GR_PAT8     10
#define GR_PAT9     11
#define GR_PAT10    12
#define GR_PAT11    13
#define GR_PAT12    14
#define GR_WHITE    16     /* Solid white */
#define GR_USER     17     /* User defined pattern */

/*
** Define a structure for that stores information about the current
** characteristics of the screen.
*/

typedef struct
{
   int screen_width;
   int screen_height;
} GR_INFO;

/*
** Define a structure that is used to store the simplified bitmap
** format called a pixmap.
*/

typedef struct
{
   short img_width;
   short img_height;
   short flags;
   char data[1];
} PIXMAP_INFO;

#define PIXMAP unsigned char

/*
** Define pixel operations
*/

#define GROP_SRC     1
#define GROP_OR      2
#define GROP_AND     4
#define GROP_NOT     8
#define GROP_XOR    16
#define GROP_NAND   (GROP_NOT | GROP_AND)
#define GROP_NOR    (GROP_NOT | GROP_OR)

/*
** Define a variable to be used for storing information that
** this module needs in order to operate.
*/

extern GR_INFO gr_info;

/*
** Define the sizes of the screen.  If you do not have to know the size
** of the screen at compile time then use the "real" macros.  The macros
** that map directly to integer values should be used if they are in
** complex #define statements to avoid the generation of a lot of code
** by the compiler.
*/
#define gr_real_scr_width()         (gr_info.screen_width)
#define gr_real_scr_height()        (gr_info.screen_height)

   #ifdef LCD
      #define gr_scr_width()		480
      #define gr_scr_height()		128
   #else
      #define gr_scr_width()		640
      #define gr_scr_height()		480
   #endif

/*
** Sizes of the screen.
*/
#define MIN_X  0
#define MIN_Y  0
#define MAX_X  (gr_scr_width()  - 1)
#define MAX_Y  (gr_scr_height() - 1)

/*
** All of the graphics routines view memory as being a 80x480
** byte array.  It really is not.  It is a 124x480 byte array
** that the ASIC reads a 80x480 chunk of.  Remap the 80x480
** addresses to the 124x480 address system.  This is only for
** the PWPs, though.  The PC works fine.
*/
   #ifdef CRT
      #define LEFT_VID_BUF       (14)
      #define RIGHT_VID_BUF      (30)
   #else
      #define LEFT_VID_BUF       (0)
      #define RIGHT_VID_BUF      (0)
   #endif

#define VID_BUF_WIDTH      (LEFT_VID_BUF + (gr_scr_width() >> 3) + RIGHT_VID_BUF)

#define VID_ADDR_ROW(row)  ((row)*(VID_BUF_WIDTH))
#define VID_ADDR_COL(col)  ((col) + (LEFT_VID_BUF))
#define VID_ADDR(x,y)      (VID_ADDR_ROW(y) + VID_ADDR_COL(((x) >> 3)))

/*
** This is a pointer to the start of video memory.
*/
   extern unsigned char *vidbuf;

/*
** The mask values must be different between the PC and the PWP because
** the order in which bits in a byte are displayed on the screen in
** opposite order.
*/
   extern unsigned char mask_and[];
   extern unsigned char mask_or[];
   extern unsigned char mask_right[];
   extern unsigned char mask_left[];

/*
** The number of bytes in each color pattern
*/
#define GR_PAT_SIZE	8

/*
** The number of color patterns
*/
#define GR_PAT_COUNT	18

/*
** A table of graphics bit patterns that are used to implement
** the concept of color on the monochrome display.  A color number
** is used is used to index into this table.
*/
extern unsigned char gr_color_pattern[GR_PAT_COUNT][GR_PAT_SIZE];

   #define BITREV(i) (i)

#define PAT_VAL(x,y,c)	(BITREV(gr_color_pattern[c][y & 7]))

/*
** The reason for this ifdef is that the pattern would be flipped
** on the PWP when PIX_VAL was called
*/
   #define PIX_VAL(x,y,c)	((PAT_VAL(x,y,c) >> (x & 7)) & 1)

/*
** Function prototypes.
*/
   #define gr_pixmap_width(pixmap)   (((PIXMAP_INFO *)(pixmap))->img_width)
   #define gr_pixmap_height(pixmap)  (((PIXMAP_INFO *)(pixmap))->img_height)

void gr_pixmap(int x, int y, PIXMAP *pixmap, int op);
void gr_def_pattern(unsigned char *pattern);
void gr_bevel_rect(int x, int y, int width, int height, int thick, int fc, int state);
#define BEVEL_IN	1
#define BEVEL_OUT	2
void gr_init  (void);
void gr_close (void);
void gr_point  (int x, int y, int color);
void gr_circle (int x, int y, int radius);
void gr_line   (int x1, int y1, int x2, int y2, int thick, int color);
void gr_rect   (int x, int y, int width, int height, int thick, int bc, int f);


#endif   /* ifndef GR_H_INCLUDED */

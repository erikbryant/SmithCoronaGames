/*****************************************************************************
**
**  Module Name:
**      gr.c
**
**  Description:
**      This module contains a number of graphical primitives for
**      use by PWP applications.
**
**  Exported Units:
**      gr_get_info
**      gr_init
**      gr_close
**      gr_scr_width
**      gr_scr_height
**      gr_point
**      gr_line
**      gr_rect
**      gr_clear_scr
**      gr_pixmap
**      gr_pixmap_width
**      gr_pixmap_height
**      gr_bevel_rect
**      gr_def_pattern
**
**  Local Units:
**
**  Exported Data:
**
**  Notes:
**      None
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      01/28/92    Initial version
**
**      03/13/92    gr_pixmap is working (mostly) for both PC and PWP
**   
**      04/08/92    Fixed case in gr_pixmap where on PWP very small bitmaps were
**                  displayed incorrectly (shift direction wrong - added ifdef)
**
*****************************************************************************/

/******************************  INCLUDE FILES  *****************************/
   #include "types.h"
   #include "const.h"
   #include "varstab.j"
   #include "sc_def.h"
   #include "windows.h"
   #include "limits.h"
   #include "winmacro.h"
   #include "macros.h"

#include "gr.h"
#include "fontlib.h"


/*********************************  GLOBALS  ********************************/

/*
** When the game comes up it is in its own window.
*/
   WINDOW Raw_window;


/*
** This structure stores information about the current vidth and
** height of the display screen.
*/
GR_INFO gr_info = { 0, 0 };

/*
** This is a pointer to the start of video memory.
*/
   unsigned char *vidbuf = 0;


/*
** A table of graphics bit patterns that are used to implement
** the concept of color on the monochrome display.  A color number
** is used is used to index into this table.
*/
unsigned char gr_color_pattern[GR_PAT_COUNT][GR_PAT_SIZE] =
{
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	/* black */
   0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xFF, 0xFF, 0xFF,	/*  4/64 */
   0xEE, 0xFF, 0xBB, 0xFF, 0xEE, 0xFF, 0xBB, 0xFF,	/*  8/64 */
   0xAA, 0xFF, 0xEE, 0xFF, 0xAA, 0xFF, 0xEE, 0xFF,	/* 12/64 */
   0xEE, 0xBB, 0xEE, 0xBB, 0xEE, 0xBB, 0xEE, 0xBB,	/* 16/64 */
   0xEE, 0xBB, 0x55, 0xBB, 0xEE, 0xBB, 0x55, 0xBB,	/* 20/64 */
   0xAA, 0x77, 0xAA, 0x77, 0xAA, 0x77, 0xAA, 0x77,	/* 24/64 */
   0xAA, 0x55, 0xAA, 0xDD, 0xAA, 0x55, 0xAA, 0xDD,	/* 28/64 */
   0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,	/* gray  */
   0xAA, 0x55, 0x22, 0x55, 0xAA, 0x55, 0x22, 0x55,	/* 36/64 */
   0x22, 0x55, 0xAA, 0x11, 0x22, 0x55, 0xAA, 0x11,	/* 40/64 */
   0x88, 0x55, 0x22, 0x44, 0x88, 0x55, 0x22, 0x44,	/* 44/64 */
   0x22, 0x44, 0x22, 0x11, 0x22, 0x44, 0x22, 0x11,	/* 48/64 */
   0x88, 0x11, 0x22, 0x00, 0x88, 0x11, 0x22, 0x00,	/* 52/64 */
   0x00, 0x22, 0x00, 0x11, 0x00, 0x22, 0x00, 0x11,	/* 56/64 */
   0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11,	/* 60/64 */
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/* white */
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x11, 0xFF, 0xFF	/* user defined */
};

/*
** The mask values must be different between the PC and the PWP because
** the order in which bits in a byte are displayed on the screen in
** opposite order.
*/
   unsigned char mask_and[]   = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };
   unsigned char mask_or[]    = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
   unsigned char mask_left[]  = { 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
   unsigned char mask_right[] = { 0x00, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 };


/**********************************  UNITS  *********************************/
/*
** Because bits are displayed in inversed, reversed order when displayed
** on the PWP display screen.
*/

/*****************************************************************************
**
**  Unit Name:
**      gr_get_info
**
**  Description:
**      Determines the screen width and height and loads the values into
**      a global variable accessable through the macros gr_real_scr_width ()
**      and gr_real_scr_height ().
**
**  Calling convention:
**      gr_get_info ()
**
**  Parameters:
**      None
**
**  Return Value:
**      None
**
**  Notes:
**      None
**
*****************************************************************************/

void gr_get_info (void)
{
   gr_info.screen_height = *((unsigned long *)vtab[VT_dsplen]);
   gr_info.screen_width = (*((unsigned long *)vtab[VT_dspwdt])) * 8;

   return;
}

/*****************************************************************************
**
**  Unit Name:
**      gr_init
**
**  Description:
**      This function configures the video to be used for a
**      PWP application environment.
**
**  Calling convention:
**      gr_init ()
**
**  Parameters:
**      None
**
**  Return Value:
**      None
**
**  Notes:
**      None
**
*****************************************************************************/

void gr_init(void)
{

   gr_get_info ();

   vidbuf = ((unsigned char *)vtab[VT_scrnmem]);

   /*
   ** Suspend all other tasks on the system.
   */
   execTStatSusp();

   /*
   ** Open a full screen window for the game.
   */
   rawTinit();
   rawCsrToggle ();

   return;
}



/*****************************************************************************
**
**  Unit Name:
**      gr_close
**
**  Description:
**      This function performs the necessary steps to restore the
**      video state.
**
**  Calling convention:
**      gr_close ()
**
**  Parameters:
**      None
**
**  Return Value:
**      None
**
**  Notes:
**      None
**
*****************************************************************************/

void gr_close(void)
{

   rawClrEow();

   rawWinClose();

   execTRest ();

   #ifdef LCD
      /*
      ** The LCD does not have hardware windows.  To get the icons
      ** to redraw you have to reboot.
      */
      warm_boot ();
   #endif

}



/*****************************************************************************
**
**  Unit Name:
**      gr_scr_width
**
**  Description:
**      This function returns the width (in pixels) of the
**      video screen.
**
**  Calling convention:
**      gr_scr_width ()
**
**  Parameters:
**      None
**
**  Return Value:
**      width of the screen in pixels
**
**  Notes:
**      This function is implemented as a macro for efficiency
**      reasons.
**
*****************************************************************************/

                
                
/*****************************************************************************
**
**  Unit Name:
**      gr_scr_height
**
**  Description:
**      This function returns the height (in pixels) of the
**      video screen.
**
**  Calling convention:
**      gr_get_info ()
**
**  Parameters:
**      None
**
**  Return Value:
**      height of the screen in pixels
**
**  Notes:
**      This function is implemented as a macro for efficiency
**      reasons.
**
*****************************************************************************/



/*****************************************************************************
**
**  Unit Name:
**      gr_point
**
**  Description:
**      This function plots a single point to the screen using
**      the currently defined color.
**
**  Calling convention:
**      gr_point ()
**
**  Parameters:
**
**      Name         Description
**      ----         -----------
**      x            X coordinate of the point
**      y            Y coordinate of the point
**      color        color of the point
**
**  Return Value:
**      None
**
**  Notes:
**      No error checking is performed on the parameters.  If illegal
**      coordinates or color indexes are specified, the resulting
**      behavior is unspecified.
**
*****************************************************************************/

void gr_point(int x, int y, int color)
{
   unsigned int addr;
 
   /*
   ** Do nothing if the color specified is GR_NONE.
   */
   if (color == GR_NONE)
   {
      return;
   }

   /*
   ** Plot the point using the specified color
   */
   addr = VID_ADDR(x,y);

   if (PIX_VAL(x,y,color) == 1) {
      vidbuf[addr] |= mask_or[x & 7];
   }
   else {
      vidbuf[addr] &= mask_and[x & 7];
   }
}



/*
** gr_line_h
**
** This function draws a horizontal line.
*/
static void gr_line_h(int x, int y, int length, int color)
{
   int bits_left;
   int bits_right;
   int whole_bytes;
   int mask = 0;
   unsigned int addr;
   int pattern;
   unsigned char byte;

   bits_left = (8 - (x & 7)) & 7;
   bits_right = (x + length) & 7;
   addr = VID_ADDR (x, y);
   pattern = PAT_VAL(x,y,color);

   /*
   ** Check to see if the entire line fits into one byte, in
   ** which case special processing has to be performed.
   */
   if (length < bits_left)
   {
      mask = mask_left[bits_left] | mask_right[bits_right];
      byte = vidbuf[addr] & mask;
      vidbuf[addr] = byte | (pattern & ~mask);
      return;
   }
   
   /*
   ** Update the left-most byte of the line
   */
   if (bits_left > 0)
   {
      mask = mask_left[bits_left];
      byte = vidbuf[addr] & mask;
      vidbuf[addr++] = byte | (pattern & ~mask);
   }
   
   /*
   ** Update the middle section of the line, which is composed
   ** entirely of complete bytes.
   */
   whole_bytes = (length - bits_left) >> 3;
   if (whole_bytes > 0)
   {
      memset(&vidbuf[addr],pattern,whole_bytes);
      addr += whole_bytes;
   }

   /*
   ** Update the right-most byte of the line
   */
   if (bits_right > 0)
   {
      mask = mask_right[bits_right];
      byte = vidbuf[addr] & mask;
      vidbuf[addr] = byte | (pattern & ~mask);
   }

}



/*
** gr_line_v
**
** This function draws a vertical line.
*/
static void gr_line_v(int x, int y, int length, int color)
{
   while (length-- > 0) {
      gr_point(x, y++, color);
   }
}


/*****************************************************************************
**
**  Unit Name:
**      gr_line
**
**  Description:
**      This function draws a line.  A thickness parameter can be
**      specified in order to draw a thicker line.
**
**  Calling convention:
**      gr_line ()
**
**  Parameters:
**
**      Name         Description
**      ----         -----------
**      x1           X coordinate of first point on line        
**      y1           Y coordinate of first point on line
**      x2           X coordinate of other endpoint on line
**      y2           Y coordinate of other endpoint on line
**      thick        Width of the line
**      color        Color of the line
**
**  Return Value:
**      None
**
**  Notes:
**      No error checking is performed on the parameters.  If illegal
**      coordinates or color indexes are specified, the resulting
**      behavior is unspecified.
**
*****************************************************************************/

void gr_line(int x1, int y1, int x2, int y2, int thick, int color)
{
   int s1 = 0;
   int s2 = 1;
   int a1 = 1;
   int a2 = 0;
   int dx;
   int dy;
   int d3;
   int n;

   /*
   ** Do nothing if the color specified is GR_NONE.
   */
   if (color == GR_NONE)
   {
      return;
   }

   /*
   ** Check for the case when the line is horizontal
   */
   if (y1 == y2)
   {
      while (thick-- > 0)
      {
         gr_line_h(x1, y1++, (x2-x1+1), color);
      }
      return;
   }
   
   /*
   ** Check for the case when the line is vertical
   */
   if (x1 == x2)
   {
      while (thick-- > 0)
      {
         gr_line_v(x1++, y1, (y2-y1+1), color);
      }
      return;
   }
   
   gr_point(x1, y1, color);
   
   dx = x2 - x1;
   if (dx < 0)
   {
      a1 = -1;
      dx = -dx;
   }
   dy = y2 - y1;
   if (dy < 0)
   {
      s2 = -1;
      dy = -dy;
   }
  if (dx < dy)
  { 
     n = dx;
     dx = dy;
     dy = n;
     s1 = a1;
     a1 = 0;
     a2 = s2;
     s2 = 0;
   }
   d3 = dx / 2;
   n = 1;
   
   do{
      x1 = x1 + a1;
      y1 = y1 + a2;
      d3 = d3 + dy;
      n = n + 1;
      if (d3 > dx)
      {
         d3 = d3 - dx;
         x1 = x1 + s1;
         y1 = y1 +s2;
      }
      gr_point(x1, y1, color);
   } while(n <= dx);
  
   gr_point(x2, y2, color);
}



/*
** gr_filled_rect
**
** Draw a plain, filled rectangle without a border.
*/
static void gr_filled_rect(int x, int y, int width, int height, int color)
{
   if (color != GR_NONE)
   {
      while (height-- > 0)
      {
         gr_line_h (x, y++, width, color);
      }
   }
}



/*****************************************************************************
**
**  Unit Name:
**      gr_rect
**
**  Description:
**      This function draws a rectangle with a border.
**
**  Calling convention:
**      gr_rect ()
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      x           The left coordinate of the rectangle
**      y           The upper coordinate of the rectangle
**      width       The width of the rectangle
**      height      The height of the rectangle
**      thick       The thickness of the border to place around the
**                  rectangle.  If 0, no border is drawn.
**      bc          The color of the border.
**      fc          The color of the inner region of the rectangle.
**
**  Return Value:
**      None
**
**  Notes:
**      No error checking is performed on the parameters.  If illegal
**      coordinates or color indexes are specified, the resulting
**      behavior is unspecified.
**
*****************************************************************************/

void gr_rect(int x, int y, int width, int height, int thick, int bc, int fc)
{
   /*
   ** Draw the border
   */
   if ((thick > 0) && (bc != GR_NONE))
   {
      gr_filled_rect (x, y, width, thick, bc);
      gr_filled_rect (x, y+height-thick, width, thick, bc);

      gr_filled_rect (x, y, thick, height, bc);
      gr_filled_rect (x+width-thick, y, thick, height, bc);
   }
   
   /*
   ** Draw the solid center portion
   */
   gr_filled_rect(x+thick, y+thick, width-(2*thick), height-(2*thick), fc);
}



/*****************************************************************************
**
**  Unit Name:
**      gr_clear_scr
**
**  Description:
**      This function clears the screen to the specified color.
**
**  Calling convention:
**      gr_clear_scr (int color)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      color       The color with which to clear the screen 
**
**  Return Value:
**      None
**
**  Notes:
**      This function is implemented as a macro on the PC for efficiency
**      reasons.
**
*****************************************************************************/
void gr_clear_scr (int color)
{
   register INT2  y;
   register UCHAR *ptr;

   ptr = &vidbuf[VID_ADDR (0, 0)];

   for (y = 0; y < gr_scr_height (); y++) {
      memset (ptr, PAT_VAL (0, y, color), gr_scr_width() >> 3);
      ptr += VID_ADDR (0, 1) - VID_ADDR (0, 0);
   }

   return;
}

/*
** gr_pixmap_row
**
*/
void gr_pixmap_row(int x, int y, int width, PIXMAP *data, int op)
{
   int bits_left;
   int bits_right;
   int whole_bytes;
   int mask = 0;
   unsigned int addr;
   unsigned char byte;

   bits_left = (8 - (x & 7)) & 7;
   bits_right = (x + width) & 7;
   
   addr = VID_ADDR(x,y);

   /*
   ** Pixmap is smaller than 8 bits
   */

   if (width < bits_left)
   {
      mask = mask_left[bits_left] | mask_right[bits_right];
      byte = vidbuf[addr] & mask;

      mask = mask_right[bits_left];
      byte = byte | ((BITREV(*data) & ~mask) << (8 - bits_left));

      vidbuf[addr] = byte;
      return;                      
   }
   
   /*
   ** Update the left-most byte of the line
   */

   if (bits_left > 0)
   {
      mask = mask_left[bits_left];
      byte = vidbuf[addr] & mask;
      mask = mask_right[bits_left];
      byte = byte | ((BITREV(*data) & ~mask) << (8 - bits_left));
      vidbuf[addr++] = byte;
   }

   /*
   ** Update the middle section of the line, which is composed
   ** entirely of complete bytes.
   */

   whole_bytes = (width - bits_left) >> 3;
   if (whole_bytes > 0)
   {
        while (whole_bytes-- > 0)
        {
            if (bits_left==0)
            {
                vidbuf[addr++] = BITREV(*data++);
            }
            else
            {
                /*
                **  The reason for this is when the data byte boundaries do 
                **  not match the video byte boundaries. The cause of this is
                **  when there are less then 8 bits to the left of a video byte.
                **  This means the data bytes and the video bytes are not in sync. 
                **  In this case it is neccessary to make the data byte
                **  boundaries match the video byte boundaries
                */

                /*
                **  Get right half of byte from data
                **  and increment data
                */

                byte  = (BITREV(*data++) & mask) >> bits_left;

                /*
                **  Get left half of byte from data and combine with right
                */

                byte |= ((BITREV(*data) & ~mask) << (8 - bits_left));

                /*
                **  Write to video and increment video
                */

                vidbuf[addr++] = byte;
            }
        }
   }

   /*
   ** Update the right-most byte of the line
   */

   if (bits_right > 0)
   {
      mask = mask_right[bits_right];
      byte = vidbuf[addr] & mask;

      /*
      ** Handled differently depending on whether the byte
      ** boundaries of the video and data are in sync
      */

      if (bits_left==0)
      {
        vidbuf[addr] = byte | (BITREV(*data) & ~mask);
      }
      else if ((bits_left+bits_right) > 8)
      {
        byte = byte | (BITREV(*data++) >> bits_left);
        byte = byte | (BITREV(*data) << (8-bits_left));
        vidbuf[addr] = byte;
      }
      else
      {
        byte = byte | (BITREV(*data) >> bits_left);
        vidbuf[addr] = byte;
      }
   }

   return;
}


/*****************************************************************************
**
**  Unit Name:
**      gr_pixmap
**
**  Description:
**      This function displays a bitmap to the screen.
**
**  Calling convention:
**      gr_pixmap (int x, int y, PIXMAP *pixmap, int op)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      x           X coordinate of the upper left corner of the pixmap
**      y           Y coordinate of the upper left corner of the pixmap
**      pixmap      Pointer to the pixmap
**      op          How the pixmap is to be drawn (GROP_SRC, GROP_XOR, etc.)
**
**  Return Value:
**      None
**
**  Notes:
**      None
**
*****************************************************************************/

void gr_pixmap(int x, int y, PIXMAP *pixmap, int op)
{
   int width;
   int height;
   int flags;
   int bytes_per_row;

   /*
   ** Read the header information from the beginning
   ** of the pixmap.
   */
   width   = *pixmap++ << 8;
   width  |= *pixmap++;
   height  = *pixmap++ << 8;
   height |= *pixmap++;
   flags   = *pixmap++ << 8;
   flags  |= *pixmap++;

   /*
   ** Display the pixmap one row at a time.
   */
   bytes_per_row = (width + 7) >> 3;

   if (op == GROP_SRC) {
      while (height-- > 0)
      {
         gr_pixmap_row(x, y++, width, pixmap, op);
         pixmap += bytes_per_row;
      }
   }
}



/*****************************************************************************
**
**  Unit Name:
**      gr_pixmap_width
**
**  Description:
**      This function returns the width of the specified pixmap.
**
**  Calling convention:
**      gr_pixmap_width (PIXMAP *pixmap)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      pixmap      A pointer to a pixmap
**
**  Return Value:
**      The width (in pixels) of the pixmap
**
**  Notes:
**      This function is implemented as a macro for the PWP.
**
*****************************************************************************/




/*****************************************************************************
**
**  Unit Name:
**      gr_pixmap_height
**
**  Description:
**      This function returns the height of the specified pixmap.
**
**  Calling convention:
**      gr_pixmap_height (PIXMAP *pixmap)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      pixmap      A pointer to a pixmap
**
**  Return Value:
**      The height (in pixels) of the pixmap
**
**  Notes:
**      This function is implemented as a macro for the PWP.
**
*****************************************************************************/




/*****************************************************************************
**
**  Unit Name:
**      gr_bevel_rect
**
**  Description:
**      This function displays a bordered rectangle to the screen, in
**      which the border is 'beveled' in order to give it a 3-D
**      appearence.
**
**  Calling convention:
**      gr_bevel_rect ()
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      x           X coordinate of upper left corner of rectangle
**      y           Y coordinate of upper left corner of rectangle
**      width       Width of rectangle
**      height      Height of rectangle
**      thick       Thickness of the bevel
**      fc          Color of the area inside the bevel
**      op          Which direction the bevel goes (BEVEL_IN or BEVEL_OUT)
**
**  Return Value:
**      None
**
**  Notes:
**      None
**
*****************************************************************************/

void gr_bevel_rect (int x, int y, int width, int height, int thick, int fc, int op)
{
   int i;
   
   switch (op) {
      case BEVEL_OUT:

         for (i=0; i<height; i++)
         {
            gr_line_h(x,            y+i,       thick,         GR_WHITE);
            gr_line_h(x+width-thick,y+i,       thick,         GR_BLACK);
         }
         
         for (i=0; i<thick; i++)
         {
            gr_line_h(x+i,       y+i,          width-(i << 1), GR_WHITE);
            gr_line_h(x+i,     y+height-1-i,   width-i-thick , GR_BLACK);
         }
         break;
      case BEVEL_IN:
         for (i=0; i<height; i++)
         {
            gr_line_h(x,            y+i,       thick,        GR_BLACK);
            gr_line_h(x+width-thick,y+i,       thick,        GR_WHITE);
         }
         for (i=0; i<thick; i++)
         {
            gr_line_h(x+i,       y+i,          width-(i << 1),  GR_BLACK);
            gr_line_h(x+i+1,     y+height-1-i,  width-i-thick,  GR_WHITE);
         }
         break;
   }

   gr_filled_rect(x+thick, y+thick, width-2*thick, height-2*thick, fc);

}



#if 0
/*****************************************************************************
**
**  Unit Name:
**      gr_def_pattern
**
**  Description:
**      This function allows the caller to define a custom color.
**
**  Calling convention:
**      gr_def_pattern (unsigned char *pattern)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**
**  Return Value:
**      None
**
**  Notes:
**      None
**
*****************************************************************************/

void gr_def_pattern(unsigned char *pattern)
{
   int i;
   
   for (i=0; i<GR_PAT_SIZE; i++)
   {
      gr_color_pattern[GR_USER][i] = BITREV(*pattern++);
   }
}
#endif

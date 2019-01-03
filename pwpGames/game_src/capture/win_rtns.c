/*************************************************************************
** Module Name:
**    win_rtns
**
** Description:
**    Local window handling routines.
**
** Notes:
**    CRT   -  Draws a beveled rectangle for a window
**    LCD   -  Draws a shadowed rectangle for a window
**
** Change Log:
**
**     Date           Description
**     ----           -----------
**
**     05/14/92       Creation date
**
**************************************************************************/

/*
*************************  INCLUDED FILES  *******************************
*/

#include "sc_def.h"
#include "gr.h"
#include "win_rtns.h"

/*
************************    Globals    ***********************************
*/

/***************************************************************************
** Unit Name:
**    display_window
**
** Description:
**    Draws a window given an x,y location, width, height and a fill color.
**    On the CRT it displays a window with a beveled frame. On the LCD
**    it displays a shadowed window.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**      x               x position of inner upper left corner of window
**      y               y position of inner upper left corner of window
**      width           inner width of window
**      height          inner height of window
**      fill_color      color to fill window with (usually GR_WHITE
**                                                         or GR_NONE)
**
** Return Value:
**    NONE
**
** Notes:
**    The parameters passed are the inner values for the window
**    the window frame is then built outside(around) those values
**    makes it easier to manipulate items within the window without
**    a lot of extra calculations
**
***************************************************************************/

void display_window(INT2 x,INT2 y,INT2 width, INT2 height,INT4 fill_color)
{
#ifdef LCD

   /*
   ** Window frame
   */
   gr_rect( x,
            y,
            width,
            height,
            1,
            GR_BLACK,
            fill_color);

   /*
   ** window shadow
   */

   gr_line( x + SHADOW_WIDTH,
            y + height,
            x + width  + SHADOW_WIDTH - 1,
            y + height,
            SHADOW_WIDTH,
            GR_BLACK);

   gr_line( x + width,
            y + SHADOW_WIDTH,
            x + width,
            y + height + SHADOW_WIDTH - 1,
            SHADOW_WIDTH,
            GR_BLACK);
#else
   /*
   ** OUTSIDE BEVEL SURROUNDING BOARD
   */

   gr_bevel_rect(x - (WIN_BORDER_THICKNESS << 1) - WIN_FRAME_WIDTH,
                 y - (WIN_BORDER_THICKNESS << 1) - WIN_FRAME_WIDTH,
                 width  + ((WIN_BORDER_THICKNESS << 2) + (WIN_FRAME_WIDTH << 1)),
                 height + ((WIN_BORDER_THICKNESS << 2) + (WIN_FRAME_WIDTH << 1)),
                 WIN_BORDER_THICKNESS,
                 GR_NONE,
                 BEVEL_OUT);

   /*
   ** INSIDE BEVEL SURROUNDING BOARD
   */
   
   gr_bevel_rect(x - WIN_BORDER_THICKNESS,
                 y - WIN_BORDER_THICKNESS,
                 width  + (WIN_BORDER_THICKNESS << 1),
                 height + (WIN_BORDER_THICKNESS << 1),
                 WIN_BORDER_THICKNESS,
                 GR_NONE,
                 BEVEL_IN);

   /*
   ** draws black border to define inner bevel 
   ** and fills the window with specified fill_color
   */

   gr_rect( x,
            y,
            width,
            height,
            1,
            GR_BLACK,
            fill_color);
#endif
}

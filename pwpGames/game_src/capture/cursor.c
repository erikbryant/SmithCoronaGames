/*************************************************************************
** Module Name:
**
**    cursor
**
** Description:
**
**    Routines to manipulate the cursor. Display it, erase it and move it.
**    Also contains the routines to display and erase the "legal move
**    marker."
**
** Notes:
**
**
** Change Log:
**
**     Date           Description
**     ----           -----------
**
**     04/06/92       Creation date
**
**************************************************************************/

/*
*************************  INCLUDED FILES  *******************************
*/

#include "sc_def.h"
#include "gr.h"
#include "keys.h"
#include "win_rtns.h"
#include "cursor.h"
#include "board.h"
#include "capture.h"

/***************************************************************************
** Unit Name:
**    move_cursor
**
** Description:
**    
**    Moves the selection box given a direction by eraseing the old cursor,
**    moving the x or y axis and drawing the cursor in the new position.
**    The direction is simply a key from the key board.
**    It then moves the x or y pointer in that direction.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    dir               keypress that represents directional movement
**    x                 current x axis of cursor
**    y                 current y axis of cursor
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  move_cursor(UINT2 dir,INT1 *x,INT1 *y)
{
      erase_cursor(*x,*y);
      switch(dir)
      {
         case LEFT_ARROW_KEY:
         case LEFT_NUM_KEY:
                     *x=*x-1;
                     if (*x<0)
                        *x=BOARD_X-1;
                     break;
         case UP_ARROW_KEY:
         case UP_NUM_KEY:
                     *y=*y-1;
                     if (*y<0)
                        *y=BOARD_Y-1;
                     break;
         case RIGHT_ARROW_KEY:
         case RIGHT_NUM_KEY:
                     *x=*x+1;
                     if (*x>(BOARD_X-1))
                        *x=0;
                     break;
         case DOWN_ARROW_KEY:
         case DOWN_NUM_KEY:
                     *y=*y+1;
                     if (*y>(BOARD_Y-1))
                        *y=0;
                     break;
      }
      draw_cursor(*x,*y);
}

/***************************************************************************
** Unit Name:
**    draw_cursor
**
** Description:
**    
**    Given a grid location on the board draws the cursor (selection box).
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    x                 current x axis of the cursor
**    y                 current y axis of the cursor
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  draw_cursor(INT1 x, INT1 y)
{
   if ((x < 0) || (y < 0) || (x >= BOARD_X) || (y >= BOARD_Y))
   {
      x = 0;
      y = 0;
   }
#ifdef LCD
   gr_rect(       (BRD_START_X+SQUARE_X*x),
                  (BRD_START_Y+SQUARE_Y*y),
                  SQUARE_X-1,
                  SQUARE_Y-1,
                  1,
                  GR_BLACK,
                  GR_NONE);
#else
   gr_bevel_rect( (BRD_START_X+SQUARE_X*x),
                  (BRD_START_Y+SQUARE_Y*y),
                  SQUARE_X,
                  SQUARE_Y,
                  WIN_BORDER_THICKNESS,
                  GR_NONE,
                  BEVEL_IN);
#endif
}

/***************************************************************************
** Unit Name:
**    erase_cursor
**
** Description:
**    
**    This function given a grid location on the board (for CaptureIt 0-7)
**    overdraws the existing cursor with a just the opposite colors 
**    at the given location effectively erasing the cursor (selection box).
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    x                 current x axis of the cursor
**    y                 current y axis of the cursor
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  erase_cursor(INT1 x, INT1 y)
{

   /* the cursor is different between the two machines (LCD and CRT) */ 
#ifdef LCD
   gr_rect(       (BRD_START_X+SQUARE_X*x),
                  (BRD_START_Y+SQUARE_Y*y),
                  SQUARE_X-1,
                  SQUARE_Y-1,
                  1,
                  GR_WHITE,
                  GR_NONE);
#else
   gr_bevel_rect( (BRD_START_X+SQUARE_X*x),
                  (BRD_START_Y+SQUARE_Y*y),
                  SQUARE_X,
                  SQUARE_Y,
                  WIN_BORDER_THICKNESS,
                  GR_NONE,
                  BEVEL_OUT);
#endif
}

/***************************************************************************
** Unit Name:
**    drawmarker
**
** Description:
**    Draws the plus symbol at the indicated x,y location on the board.
**    The plus symbol represents a valid move in CaptureIt.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    x                 current x location of the cursor
**    y                 current y location of the cursor
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  drawmarker(INT2 x,INT2 y)
{
#ifdef LCD
   gr_line( BRD_START_X+(SQUARE_X*x+2),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y>>1)-1),
            BRD_START_X+(SQUARE_X*x+(SQUARE_X-4)),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y>>1)-1),
            1,
            GR_BLACK);
   gr_line( BRD_START_X+(SQUARE_X*x+(SQUARE_X>>1)-1),
            BRD_START_Y+(SQUARE_Y*y+2),
            BRD_START_X+(SQUARE_X*x+(SQUARE_X>>1)-1),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y-4)),
            1,
            GR_BLACK);
#else
   gr_line( BRD_START_X+(SQUARE_X*x+8),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y>>1)),
            BRD_START_X+(SQUARE_X*x+(SQUARE_X-8)),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y>>1)),
            2,
            GR_BLACK);
   gr_line( BRD_START_X+(SQUARE_X*x+(SQUARE_X>>1)),
            BRD_START_Y+(SQUARE_Y*y+8),
            BRD_START_X+(SQUARE_X*x+(SQUARE_X>>1)),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y-8)),
            2,
            GR_BLACK);
#endif
}

/***************************************************************************
** Unit Name:
**    erasemarker
**
** Description:
**    Erase the plus symbol that indicates a legal move by drawing over it
**    with the BACKGROUND color.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    x                 current x location of the cursor
**    y                 current y location of the cursor
**                      
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  erasemarker(INT2 x,INT2 y)
{
#ifdef LCD
   gr_line( BRD_START_X+(SQUARE_X*x+2),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y>>1)-1),
            BRD_START_X+(SQUARE_X*x+(SQUARE_X-4)),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y>>1)-1),
            1,
            GR_WHITE);
   gr_line( BRD_START_X+(SQUARE_X*x+(SQUARE_X>>1)-1),
            BRD_START_Y+(SQUARE_Y*y+2),
            BRD_START_X+(SQUARE_X*x+(SQUARE_X>>1)-1),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y-4)),
            1,
            GR_WHITE);
#else
   gr_line( BRD_START_X+(SQUARE_X*x+8),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y>>1)),
            BRD_START_X+(SQUARE_X*x+(SQUARE_X-8)),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y>>1)),
            2,
            BACKGROUND);
   gr_line( BRD_START_X+(SQUARE_X*x+(SQUARE_X>>1)),
            BRD_START_Y+(SQUARE_Y*y+8),      
            BRD_START_X+(SQUARE_X*x+(SQUARE_X>>1)),
            BRD_START_Y+(SQUARE_Y*y+(SQUARE_Y-8)),
            2,
            BACKGROUND);
#endif
}

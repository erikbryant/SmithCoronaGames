/*************************************************************************
** Module Name:
**     board
**
** Description:
**
**     routines to manipulate the board
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
#include "win_rtns.h"
#include "capture.h"
#include "board.h"

/*
************************    Globals    ***********************************
*/

/*
** These PIXMAP's represent the three possible types of squares
** that can occur on the game board
**
** Pixmaps would (are) be much faster than computing a circle and filling it!
*/

PIXMAP Free_square[]=
{
#include FREE_SQUARE
};
   
PIXMAP Black_square[]=
{
#include BLACK_SQUARE
};

PIXMAP White_square[]=
{
#include WHITE_SQUARE
};

/***************************************************************************
** Unit Name:
**    drawboard
**
** Description:
**    Draws the playing board with PIXMAPs and a window around it.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    x                 where to place the upper left corner of the game
**                      board on the x axis
**    y                 where to place the upper left corner of the game
**                      board on the y axis
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  drawboard(INT2 x,INT2 y)
{
   INT2 tmpx;
   INT2 tmpy;

   for(tmpx=0;tmpx<BOARD_X;tmpx++)
   {
      for(tmpy=0;tmpy<BOARD_Y;tmpy++)
      {
         gr_pixmap(  x + tmpx * SQUARE_X,
                     y + tmpy * SQUARE_Y,
                     Free_square,
                     GROP_SRC);
      }
   }
#ifdef LCD
   display_window(x-1,y-1,BOARD_X * SQUARE_X + 1,BOARD_Y * SQUARE_Y + 1,GR_NONE);
#else
   display_window(x,y,BOARD_X * SQUARE_X,BOARD_Y * SQUARE_Y,GR_NONE);
#endif
}

/***************************************************************************
** Unit Name:
**    drawtokens
**
** Description:
**    Draws circles of occupied squares on the game board
**    in the correct locations and the correct color using PIXMAPs for the 
**    circles.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             the current game board
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  drawtokens(struct BOARD *board)
{
   INT2 x;
   INT2 y;

   /* step through each location on the board */
   /* if it is an occupied square, splat up the corresponing PIXMAP */
   for(y=0;y<BOARD_Y;y++)
   {
      for(x=0;x<BOARD_X;x++)
      {
         if (board->loc[y][x] == PLAYER1)
         {
            gr_pixmap ( BRD_START_X + (x * SQUARE_X),
                        BRD_START_Y + (y * SQUARE_Y),
                        Black_square,
                        GROP_SRC);
         }
         else if(board->loc[y][x] == PLAYER2)
         {
            gr_pixmap ( BRD_START_X + (x * SQUARE_X),
                        BRD_START_Y + (y * SQUARE_Y),
                        White_square,
                        GROP_SRC);
         }

         /*
         ** else if Free square, do nothing 
         ** nothing needs to be done
         */
      }
   }
}

/***************************************************************************
** Unit Name:
**    initboard
**
** Description:
**    Initializes the playing board.
**    Sets all of the board locations to FREE
**    and places occupants on center four squares.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to the current game board
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  initboard( struct BOARD *board)
{
   
   INT2 x;
   INT2 y;
   
   /*
   ** Set All locations on board to free
   */

   memset(board,FREE,BOARD_X*BOARD_Y);

   /* 
   ** Setup 4 starting squares
   */

   board->loc[3][3] = PLAYER1;   
   board->loc[4][4] = PLAYER1;   
   board->loc[4][3] = PLAYER2;
   board->loc[3][4] = PLAYER2;
}


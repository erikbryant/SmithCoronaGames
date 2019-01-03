/*************************************************************************
** Module Name:
**    makemove
**
** Description:
**
**    Routines for making a move in capture.  Includes tests if a move is
**    legal, if there are any available moves, and suggests a move.
**
** Notes:
**
**    I had a lot of trouble using INT2 and UINT2 for some of my directional
**    variables for some reason so I went to INT4's and the problems
**    stopped, so I left it that way, but it does cause warnings with 
**    Microsoft C's complier.
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    02/20/92       Creation date
**
**************************************************************************/

/*
*************************  INCLUDED FILES  *******************************
*/

#include "sc_def.h"
#include "gr.h"
#include "makemove.h"
#include "minimax.h"
#include "board.h"
#include "capture.h"

/*
*************************  LOCAL FUNCTIONS  ******************************
*/

BOOL  testcapture  (struct BOARD *board,INT1 color,INT4 x,INT4 y,INT1 dirx,INT1 diry);
void  makecapture  (struct BOARD *board,INT1 color,INT4 x,INT4 y,INT1 dirx,INT1 diry);


/***************************************************************************
** Unit Name:
**    legalmove
**
** Description:
**    Checks if a location would be a legal move for the color.
**    From a given location first looks for a oppositely colored piece
**    then test if there are any pieces to complete the capture
**    in the same direction.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to a game board
**    x                 x location of attemped move
**    y                 y location of attemped move
**    color             color of current player
**
** Return Value:
**    returns TRUE if it is a legal move and FALSE if it is not
**
** Notes:
**    from current location look in 1 of 8 directions for a
**    opposite color then check if next further is
**    opposite color or same color. If same color then return true.
**    If opposite color check next further. If blank or a wall go
**    back to current location and try next direction.
**    If complete all eight directions without TRUE then return FALSE
**
***************************************************************************/

BOOL  legalmove(struct BOARD *board, INT1 x, INT1 y, INT1 color)
{
   INT1 tmpx = x-1; /* x value used to check the surrounding 8 squares from */
                    /* the current location given */
   INT1 tmpy;       /* y value used to check the surrounding 8 squares from */
                    /* the current location given */

   if (board->loc[y][x]==FREE)
   {
      while(tmpx < (x + 2))
      {
         if (tmpx < 0) tmpx++;

         tmpy = y-1;
       
         while (tmpy < (y + 2))
         {
            if (tmpy < 0) tmpy++;
         
            if ((tmpx == 0) && (tmpy == 0))  /* can skip this location */
            {                                /* since it is concerned only with */
               tmpy++;                       /* the surrounding 8 squares */
               continue;                     /* not the on it is on */
            }

            
            /* test for a capture */

            if (testcapture(board,color,tmpx,tmpy,(tmpx-x),(tmpy-y)))
            {
               return TRUE;
            }
            
         tmpy++;
         }
      tmpx++;
      }
   }
   return FALSE;
}

/***************************************************************************
** Unit Name:
**    makemove
**
** Description:
**    Makes a move at the given x,y location on the board passed.
**    Tests all 8 directions for a capture and makes the captures.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to a game board
**    x                 x location of attemped move
**    y                 y location of attemped move
**    color             color of current player
**
** Return Value:
**
** Notes:
**
**    Assumes that the move is a valid move
**
***************************************************************************/

void  makemove(struct BOARD *board,INT1 x,INT1 y,INT1 color)
{
   INT1 dirx = x-1;
   INT1 diry = y-1;

   if (board->loc[y][x] == FREE)
   {
      board->loc[y][x] = color;

      while (dirx < (x + 2))
      {
         diry = y-1;
         while (diry < (y + 2))
         {
            if (!((dirx == 0) && (diry == 0)))
            {
               if(testcapture(board,color,dirx,diry,(dirx-x),(diry-y)))
               {
                  makecapture(board,color,dirx,diry,(dirx-x),(diry-y));
               }
            }
            diry++;
         }
         dirx++;
      }
   }
}

/***************************************************************************
** Unit Name:
**    testcapture
**
** Description:
**    Checks for an ending piece that will complete a capture
**    and make a location a valid move.
**    Returns TRUE if and ending capture is found in the given direction
**    and FALSE if it is not found.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to a game board
**    x                 x location of attemped move
**    y                 y location of attemped move
**    color             color of current player
**    dirx              direction of capture in the x axis (-1,0,1)
**    diry              direction of capture in the y axis (-1,0,1)
**
** Return Value:
**
** Notes:
**
***************************************************************************/

BOOL  testcapture(struct BOARD *board,INT1 color,INT4 x,INT4 y,INT1 dirx,INT1 diry)
{
   BOOL done  = FALSE;
   BOOL found = FALSE;

   if((x >= 0) && (x < BOARD_X) && (y >= 0) && (y < BOARD_Y))
   {
      if (board->loc[y][x] == OTHER_PLAYER(color))
      {
         while(!done)
         {
            x += dirx;
            y += diry;

            if((x>=0) && (x<BOARD_X) && (y>=0) && (y<BOARD_Y))
            {
               if (board->loc[y][x] == color)
               {
                  found = TRUE;
                  done  = TRUE;
               }
               else if (board->loc[y][x] == FREE)
               {
                  done = TRUE;
               }
            }
            else
            {
               done = TRUE;
            }
         }
      }
   }
   return found;
}

/***************************************************************************
** Unit Name:
**    makecapture
**
** Description:
**    Changes the color of the captured squares (flips captured square)
**    colors.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to a game board
**    x                 x location of attemped move
**    y                 y location of attemped move
**    color             color of current player
**    dirx              direction of capture in the x axis (-1,0,1)
**    diry              direction of capture in the y axis (-1,0,1)
**
** Return Value:
**
** Notes:
**
***************************************************************************/

void makecapture(struct BOARD *board, INT1 color, INT4 x, INT4 y, INT1 dirx, INT1 diry)
{
   BOOL done = FALSE;

   while(!done)
   {
      if (board->loc[y][x] == (OTHER_PLAYER(color)))
      {
         board->loc[y][x] = color;
      }
      else
      {
         done=TRUE;
      }

      x += dirx;
      y += diry;
   }
}

/***************************************************************************
** Unit Name:
**    moveavailable
**
** Description:
**    checks for any available moves
**    goes through all the board locations and tests if it is a legalmove
**    once one is found no need to continue and returns TRUE
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to a game board
**    color             color of current player
**
** Return Value:
**
** Notes:
**
***************************************************************************/

BOOL  moveavailable(struct BOARD *board,INT1 color)
{
   INT1 x = 0;
   INT1 y = 0;
   BOOL found = FALSE;

   while ((!found) && (x < BOARD_X))
   {
      y = 0;
      while((!found) && (y < BOARD_Y))
      {
         if(legalmove(board,x,y,color) > 0)
         {
            found = TRUE;
         }
         y++;
      }
      x++;
   }
   return found;
}

/***************************************************************************
** Unit Name:
**    findmove
**
** Description:
**    Finds the best move for the specified player.
**    X and y will contain the suggested location to move to if a move 
**    was found.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to a game board
**    x                 x location of attemped move
**    y                 y location of attemped move
**    color             color of current player
**    lvl               current skill level
**
** Return Value:
**    returns TRUE if a move was found else FALSE
**
** Notes:
**
***************************************************************************/

BOOL findmove(struct BOARD *board,INT1 *x,INT1 *y,INT1 color,UINT1 lvl)
{

   struct COORD coord;

   coord = minimax(board,color,lvl);

   if (coord.x == -1)
   {
      return FALSE;
   }
   else
   {
      *x = coord.x;
      *y = coord.y;
      return TRUE;    
   }
}

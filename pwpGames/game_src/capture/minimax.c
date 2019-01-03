/***************************************************************************
** Module Name:
**    minimax
**
** Description:
**    The MINIMAX, Artificial Intelligence, look-ahead, search algorithm
**    for searching a game tree and choosing a move based on the
**    current board
**
**    The search algorithm uses the MIN MAX method to search the "tree" of 
**    possible moves and assigns a weight at the leaf nodes.
**    The branch returning the highest weight is the best move and returns
**    its parent coordinants as the best move.  If two or more
**    child nodes of a branch in a tree have equal weight, one is randomly
**    chosen.
**
** Notes:
**    Should start at a maximizing level for CaptureIt
**    (in other words, the level should be an odd number)
**
**    Algorithm currently statically evaluates a move. It would be better
**    if it made a dynamic evaluation that changed as the board changed.
**
** Change Log:
**
**    Date          Description
**    ----          -----------
**
**    03/24/92      Creation date
**
**    04/03/92      min portion was returning wrong weight
**                  was returning last value from the loop rather than
**                  the minimum weight
**
**    05/15/92      Randomly chooses between moves of equal weight
**
***************************************************************************/

#include "sc_def.h"
#include "capture.h"
#include "minimax.h"
#include "gr.h"
#include "board.h"

/*
** For debugging information
** prints out what is returned from each level
** If the depth is very deep (3 levels or more), it can be a lot of 
** information to wade through
**
** #define MINIMAX_DEBUG
*/

/*
** Static weight table used in evaluating a board
** by determining a weight
*/
char  weight_table_normal[BOARD_Y][BOARD_X]=
{
    100,-30, 20, 20, 20, 20,-30,100,
    -30,-50,-10,-10,-10,-10,-50,-30,
     20,-10,  1,  1,  1,  1,-10, 20,
     20,-10,  1,  1,  1,  1,-10, 20,
     20,-10,  1,  1,  1,  1,-10, 20,
     20,-10,  1,  1,  1,  1,-10, 20,
    -30,-50,-10,-10,-10,-10,-50,-30,
    100,-30, 20, 20, 20, 20,-30,100
};

char  weight_table_expert[BOARD_Y][BOARD_X]=
{
    127,-25, 35, 35, 35, 35,-25,127,
    -25,-50,-20,-20,-20,-20,-50,-25,
     35,-20,  1,  1,  1,  1,-20, 35,
     35,-20,  1, 10, 10,  1,-20, 35,
     35,-20,  1, 10, 10,  1,-20, 35,
     35,-20,  1,  1,  1,  1,-20, 35,
    -25,-50,-20,-20,-20,-20,-50,-25,
    127,-25, 35, 35, 35, 35,-25,127
};

static UINT1 Skill_lvl = 0;

/***************************************************************************
** Unit Name:
**    minimax
**
** Description:
**    The MINIMAX, Artificial Intelligence, look-ahead, search algorithm
**    for searching a game tree and choosing a move based on the
**    current board.  This function is recursive and level 0 is the level
**    at which the final evaluation of the board is made.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             A pointer to the gameboard being searched
**
**    player            the current player being evaluated
**
**    lvl               The level of search
**                      An even number is a maximizing level
**                      An odd number is a minimizing level
**                      Zero is the limit of search
**
** Return Value:
**    struct COORD   returns the weight and x,y location of best move
**
** Notes:
**    Should start at a maximizing level for capture
**    (in other words, the level should be an even number)
**    Should only return a value to findmove (MODULE makemove) from an
**    even number level. That means from the Maximize and zero levels, not
**    from the Minimize level. This is so only the top level x,y move
**    gets returned.  We are not concerned with what any of the lower
**    recursion levels x,y value are.  This is also the reason for the
**    tmp_coord structure variable.
**
***************************************************************************/

struct COORD minimax(struct BOARD *board, INT1 player, UINT1 lvl)
{

   struct BOARD tmp_board;    /* to do look ahead it is necessary to make a */
                              /* move on the board and evaluate its weight  */
                              /* rather than kill the current board we make */
                              /* a copy of it and do the evaluations on it  */
   INT2         weight;       /* current weight of the board                */
   INT1         x;            /* x location of the move being tested        */
   INT1         y;            /* y location of the move being tested        */
   struct COORD tmp_coord;    /* temporary coordination structure - used    */
                              /* because we don't want to save the x,y from */
                              /* the minimize level just the weight         */
   struct COORD coord;        /* coordinate structure of the move we think  */
                              /* is best                                    */

   coord.x=-1;
   coord.y=-1;

   if (lvl == LVL_EXPERT)
   {
      lvl = LVL_NOVICE;
      Skill_lvl = LVL_EXPERT;
   }

   /*
   ** If the limit of the search has been reached
   ** you must return the weight of the location.
   ** 
   ** Note:
   **  If you want to include any other weight on the locations
   **  other than the static weight table, this is the place to 
   **  do it.
   */
   
   /* 
   ** Statically evaluates a move and returns a weight
   */

   if (lvl==0)
   {
      coord.weight=0;
      
      /* step through every location on the board */
      /* if it is occupied add the weight or subtract based on who the */
      /* current player is */

      if (Skill_lvl == LVL_EXPERT)
      {
         for (y=0;y<BOARD_Y;y++)
         {
            for (x=0;x<BOARD_X;x++)
            {
            
               if (board->loc[y][x] == OTHER_PLAYER(player))
                  coord.weight += weight_table_expert[y][x];
               else if (board->loc[y][x] == player)
                  coord.weight -= weight_table_expert[y][x];
            }
         }
      }
      else
      {
         for (y=0;y<BOARD_Y;y++)
         {
            for (x=0;x<BOARD_X;x++)
            {
            
               if (board->loc[y][x] == OTHER_PLAYER(player))
                  coord.weight += weight_table_normal[y][x];
               else if (board->loc[y][x] == player)
                  coord.weight -= weight_table_normal[y][x];
            }
         }
      }
   }

   /*
   ** Minimize level
   ** If their move, you want worst possible!
   */
           
   else if (EVEN(lvl)) /* odd numbered level */
   {
      /* if there is a legal move available find its weight */
      if (moveavailable(board,player))
      {
         weight = MAXINT;
   
         /* step through every location on the board testind for a legal */
         /* move */
         for (y=0;y<BOARD_Y;y++)
         {
            for (x=0;x<BOARD_X;x++)
            {
               if (legalmove(board,x,y,player))
               {
                  /* once a legal move is found test it out */
                  memcpy(&tmp_board,board,sizeof(struct BOARD));

                  makemove(&tmp_board,x,y,player);

                  /* find the weight of the move */
                  coord = minimax(&tmp_board,OTHER_PLAYER(player),(UINT1)(lvl-1));

                  /*
                  ** At this level we do not care what the coordinates are
                  ** actually we don't want to know
                  */
                  if (coord.weight < weight)
                  {
                     weight = coord.weight;
                  }

#ifdef MINIMAX_DEBUG                    
                  switch(lvl)
                  {
                     case 2: printf("  MIN -   %3d %3d %3d %3d\n",lvl,weight,x,y);
                              break;
                     case 4: printf("MIN -     %3d %3d %3d %3d\n",lvl,weight,x,y);
                              break;
                  }
#endif                    
               }
            }
         }
         coord.weight = weight;
      }
      else
      {
         /* if no legal move available we return a weight saying that */
         /* this could be a good move for one player and not the other*/
         coord.weight = -(150);
      }
   }

   /*
   ** Maximize level
   ** If your move, you want the best possible!
   */
           
   else if (ODD(lvl)) /* even numbered level */
   {
      if (moveavailable(board,player))
      {
         weight = -(MAXINT);
   
         /* step through every location on the board testind for a legal */
         /* move */
         for (y=0;y<BOARD_Y;y++)
         {
            for (x=0;x<BOARD_X;x++)
            {
       
               if (legalmove(board,x,y,player))
               {
                  /* once a legal move is found test it out */
                  memcpy(&tmp_board,board,sizeof(struct BOARD));
   
                  makemove(&tmp_board,x,y,player);
               
                  /* find the weight of the move */
                  tmp_coord = minimax(&tmp_board,OTHER_PLAYER(player),(UINT1)(lvl-1));

                  if (tmp_coord.weight > weight)
                  {
                     /*
                     ** Since the weight was greater than the last
                     ** this we assume is a better move so we record
                     ** the weight and x,y location of the move
                     */
                     weight = tmp_coord.weight;
                     coord.weight = weight;
                     coord.x = x;
                     coord.y = y;
                  }
                  else if((weight == tmp_coord.weight) && ((rand() % 2) == 1)) 
                  {
                     /*
                     ** Randomizes the moves somewhat
                     ** Each time if weight is equal will randomize move
                     ** Choosing a different sub tree of possible moves 
                     ** rather than always choosing the same set of moves
                     */
                  
                     weight = tmp_coord.weight;
                     coord.weight = weight;
                     coord.x = x;
                     coord.y = y;
                  }

#ifdef MINIMAX_DEBUG                    
                  switch(lvl)
                  {
                     case 1: printf("   MAX -  %3d %3d %3d %3d\n",lvl,weight,x,y);
                           break;
                     case 3: printf(" MAX -    %3d %3d %3d %3d\n",lvl,weight,x,y);
                           break;
                  }
#endif                 
               }
            }
         }
      }
      else
      {
         coord.weight = +(150);
      }
   }
    
#ifdef MINIMAX_DEBUG                    
   switch(lvl)
   {
      case 0: printf("    RET - %3d %3d\n",lvl,coord.weight);
              break;
      case 1: printf("   RET -  %3d %3d %3d %3d\n",lvl,coord.weight,coord.x,coord.y);
              break;
      case 2: printf("  RET -   %3d %3d %3d %3d\n",lvl,coord.weight,coord.x,coord.y);
              break;
      case 3: printf(" RET -    %3d %3d %3d %3d\n",lvl,coord.weight,coord.x,coord.y);
              break;
      case 4: printf("RET -     %3d %3d %3d %3d\n",lvl,coord.weight,coord.x,coord.y);
              break;
   }
#endif    
     
   return coord;
}    
    

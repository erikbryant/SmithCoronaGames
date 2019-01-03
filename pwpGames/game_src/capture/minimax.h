/***************************************************************************
** Module Name:
**    minimax
**
** Description:
**    The Header file for the minimax module
**
** Change Log:
**
**     Date           Description
**     ----           -----------
**
**     03/24/92       Creation date
**
***************************************************************************/

#define  MAXINT         32000       /* largest value of an INT (approx.) */

#define  ODD(num)       ((num) % 2) /* returns TRUE if a number is ODD   */
#define  EVEN(num)      (!ODD(num)) /* returns TRUE if a number is EVEN  */

struct COORD { 
    INT1 x;
    INT1 y;
    INT2 weight;
};

struct COORD minimax(struct BOARD *board,INT1 player,UINT1 skill_level);

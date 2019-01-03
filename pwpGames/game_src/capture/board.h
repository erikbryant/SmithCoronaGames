/*************************************************************************
** Module Name:
**
**    board
**
** Description:
**
**    This is the header file for the board module
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    04/06/92       Creation date
**
**************************************************************************/
#ifdef LCD

/**** board ****/

#define  BRD_START_X     ((gr_scr_width()  - (BOARD_X*SQUARE_X))>>1)
#define  BRD_START_Y     ((gr_scr_height() - (BOARD_Y*SQUARE_Y))>>1)
#define  BOARD_X          8
#define  BOARD_Y          8
#define  SQUARE_X        14
#define  SQUARE_Y        14

/**** Square pixmap names ****/

#define  FREE_SQUARE     "freelcd.pix"
#define  WHITE_SQUARE    "whitelcd.pix"
#define  BLACK_SQUARE    "blacklcd.pix"

#else

/**** board ****/
#define  BRD_START_X     80
#define  BRD_START_Y     60
#define  BOARD_X          8
#define  BOARD_Y          8
#define  SQUARE_X        40
#define  SQUARE_Y        40


/**** Square pixmap names ****/

#define  FREE_SQUARE     "freecrt.pix"
#define  WHITE_SQUARE    "whitecrt.pix"
#define  BLACK_SQUARE    "blackcrt.pix"
#endif


/***** the game board structure *****/

struct BOARD
{
   char  loc[BOARD_Y][BOARD_X];
};


void  drawtokens     (struct BOARD *board);
void  initboard      (struct BOARD *board);
void  drawboard      (INT2 x,INT2 y);


/*************************************************************************
** Module Name:
**    makemove
**
** Description:
**
**    This is the header file for the makemove module
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    02/20/92       Creation date
**
**************************************************************************/

BOOL  legalmove      (struct BOARD *board,INT1 x, INT1 y, INT1 color);
void  makemove       (struct BOARD *board,INT1 x, INT1 y, INT1 color);
BOOL  findmove       (struct BOARD *board,INT1 *x,INT1 *y,INT1 color,UINT1 lvl);
BOOL  moveavailable  (struct BOARD *board,INT1 color);


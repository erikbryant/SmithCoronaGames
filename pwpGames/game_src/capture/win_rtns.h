/*************************************************************************
** Module Name:
**
**    win_rtns
**
** Description:
**
**    This is the header file for the win_rtns module
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    05/15/92       Creation date
**
**************************************************************************/


/*
** window frame defines
*/

#ifdef LCD
#define WIN_BORDER_THICKNESS    3
#define WIN_FRAME_WIDTH         0
#define SHADOW_WIDTH            3
#else
#define WIN_BORDER_THICKNESS    3
#define WIN_FRAME_WIDTH         5
#endif

void  display_window (INT2 x,INT2 y,INT2 width, INT2 height,INT4 fill_color);


/*************************************************************************
** Module Name:
**
**    menu
**
** Description:
**
**    This is the header file for the menu module
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    04/06/92       Creation date
**
**************************************************************************/

/**** menu ****/
#ifdef LCD

#define  MENU_CENTER_X    (gr_scr_width() - ((BRD_START_X + BANNER_WIDTH) >> 1))
#define  MENU_CENTER_Y    (gr_scr_height() >> 1)
#define  MENU_FONT        "Helvetica 16"
#define  MENU_TEXT_OFFSET  10

#else

#define  MENU_START_X     (BRD_START_X + (SQUARE_X << 3) + 50)
#define  MENU_START_Y     (BRD_START_Y + (SQUARE_Y << 2) - ((ascend + descend) >> 1))
#define  MENU_FONT        "Helvetica 30"
#define  MENU_TEXT_OFFSET  10
#define  MENU_LEADING      12
#endif

void  display_menu(char *menustr);

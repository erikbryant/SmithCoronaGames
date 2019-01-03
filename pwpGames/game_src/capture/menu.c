/***************************************************************************
** Module Name:
**    menu
**
** Description:
**
**    Variable size menu routines
**
** Notes:
**    Size of menu depends on text
**    No error checking is done
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    04/06/92       Creation date
**
**************************************************************************/

#include "sc_def.h"
#include "gr.h"
#include "fontlib.h"
#include "win_rtns.h"
#include "capture.h"
#include "board.h"
#include "menu.h"

/***************************   Globals   **********************************/

/* records current size and location of the menu */
/* It is also used be the thinking window to match the X location and width */
/* of the menu on the CRT */

INT2 Menu_x        = -1;
INT2 Menu_y        = -1;
INT2 Menu_width    = -1;
INT2 Menu_height   = -1;


/***************************************************************************
** Unit Name:
**    display_menu
**
** Description:
**    Displays a resizable menu depending on the length of longest
**    newline terminated string.
**
**    If a menu is already being displayed it will draw over it
**    with a rectangle in the BACKGROUND color.
**
**    Entries are separated by newlines.
**    Underlines are used to distinguish menu hot keys.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    menu_str          pointer to menu string
**
** Return Value:
**    NONE
**
** Notes:
**    No error checking is performed!
**
***************************************************************************/

void  display_menu(char *menu_str)
{
   FONT_ATTR   f_attr;
   INT2        max_length;
   INT2        ascend;
   INT2        descend;
#ifndef LCD
   INT1  tmp_leading;
#endif

   if (font_select(MENU_FONT) < 0)
   {
      return;
   }

   font_get_attributes(&f_attr);

#ifdef LCD

   /* if there is a menu already there draw over it with the BACKGROUND color */
   
   if ((Menu_width > 0)&&(Menu_height > 0))
   {
      /* different than CRT because there is a border only on one side */
      /* the shadow is only on the right and bottom sides */

      gr_rect( Menu_x,
               Menu_y,
               Menu_width  + (WIN_BORDER_THICKNESS),
               Menu_height + (WIN_BORDER_THICKNESS),
               0,
               GR_NONE,
               BACKGROUND);
   }

   /* change the offset of the underlining */

   f_attr.underline_offset -= 2;

   font_set_attributes(&f_attr);

   /* find the attributes of the string so we can size the menu accordingly */
   font_string_size(menu_str,&max_length,&ascend,&descend);

   /* Adjust the last line of text in the menu to fit it inside the border */
   descend += 4;

   /* record menu size and location */
   Menu_x       = MENU_CENTER_X - ((MENU_TEXT_OFFSET + max_length)>>1);
   Menu_y       = MENU_CENTER_Y - ((ascend + descend)>>1);

   Menu_width   = MENU_TEXT_OFFSET + max_length;
   Menu_height  = ascend + descend;

   /* draw the window the menu is going to be displayed in */
   display_window( Menu_x,
                   Menu_y,
                   Menu_width,
                   Menu_height,
                   GR_WHITE);

   /* print the menu string centered in the window we made */
   font_printf_centered(menu_str,
                        MENU_CENTER_X,
                        MENU_CENTER_Y,
                        MENU_CENTER_X,
                        MENU_CENTER_Y);

   /* reset the underline_offset to what it was before */
   f_attr.underline_offset += 2;
#else
   
   /* if there is a menu already there draw over it with the BACKGROUND color */

   if ((Menu_x > 0)&&(Menu_y > 0))
   {
      gr_rect( Menu_x - (WIN_BORDER_THICKNESS<<1) - WIN_FRAME_WIDTH,
               Menu_y - (WIN_BORDER_THICKNESS<<1) - WIN_FRAME_WIDTH,
               Menu_width  + ((WIN_FRAME_WIDTH<<1) + (WIN_BORDER_THICKNESS<<2)),
               Menu_height + ((WIN_FRAME_WIDTH<<1) + (WIN_BORDER_THICKNESS<<2)),
               0,
               GR_NONE,
               BACKGROUND);
   }
 
   /* record the current value of leading so we can change it back later */
   tmp_leading = f_attr.leading;

   /* change the spacing between lines in the menu */
   f_attr.leading = MENU_LEADING;
   font_set_attributes(&f_attr);

   /* find the attributes of the string so we can size the menu accordingly */
   font_string_size(menu_str,&max_length,&ascend,&descend);

   /* Adjust the last line of text in the menu to fit it inside the border */
   descend += 4;

   /* record menu size and location */
   Menu_x      = MENU_START_X;
   Menu_y      = MENU_START_Y;

   Menu_width  = max_length + (MENU_TEXT_OFFSET<<1);
   Menu_height = ascend + descend;

   /* draw the window the menu is going to be displayed in */
   display_window(Menu_x,
                  Menu_y,
                  Menu_width,
                  Menu_height,
                  GR_WHITE);

   /* print the menu string centered in the window we made */
   font_printf_centered(menu_str,
                        Menu_x + MENU_TEXT_OFFSET + (max_length >> 1),
                        Menu_y + ((ascend + descend) >> 1),
                        Menu_x + MENU_TEXT_OFFSET + (max_length >> 1),
                        Menu_y + ((ascend + descend) >> 1));

   /* reset the leading to what it was before we changed it */
   f_attr.leading = tmp_leading;
#endif
   
   font_set_attributes(&f_attr);
}


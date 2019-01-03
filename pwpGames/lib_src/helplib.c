/*****************************************************************************
**
**  Module Name:
**      helplib
**
**  Description:
**      This module contains the functions which implement the help library.
**
**  Exported Units:
**      help_display
**
**  Local Units:
**      help_menu
**      help_display_topic
**      help_display_page
**
**  Exported Data:
**      none
**
**  Notes:
**      none
**
**  Change Log:
**      Date            Description of Change
**      ----            ---------------------
**      04/11/92        Version 1.0
**
**
**
**
**
**
**
*****************************************************************************/


/*
** Included files for module.
*/
#include "helplib.h"                /* Constants, macros, types */
#include "fontlib.h"                /* Font definitions */
#include "keys.h"                   /* Definition of key codes */
#include "inp.h"                    /* Keyboard access, etc. */
#include "gr.h"                     /* Graphics definitions */
#include "window.h"

/*
** The width of the borders for any windows that are put up.
*/
#ifdef LCD
   #define BORDER_WIDTH       1     /* Width of beveled borders */
   #define FLAT_WIDTH         0     /* Space between beveled borders */
   #define OUTER_SPACE        2     /* Space between the edge of the screen and
                                       the outer border */
   #define INNER_SPACE        4     /* Space between the inner border and the
                                       field of text inside */
   #define HELP_MENU_LEADING  8     /* Space between lines of help text */
#else
   #define BORDER_WIDTH       3     /* Width of beveled border */
   #define FLAT_WIDTH         5     /* Space between beveled borders */
   #define OUTER_SPACE        8     /* Space between the edge of the screen and
                                       the outer border */
   #define INNER_SPACE        8     /* Space between the inner border and the
                                       field of text inside */
   #define HELP_MENU_LEADING 12     /* Space between lines of help text */
#endif
/*
** The dimensions/location of the title bar for the help pages.
*/
#define TITLE_MIN_X  (MIN_X + OUTER_SPACE + FLAT_WIDTH + (BORDER_WIDTH << 1))
#define TITLE_MIN_Y  (MIN_Y + OUTER_SPACE + FLAT_WIDTH + (BORDER_WIDTH << 1))
#undef  TITLE_WIDTH
#define TITLE_WIDTH  (MAX_X - TITLE_MIN_X - (TITLE_MIN_X - MIN_X))

#undef TITLE_HEIGHT
#ifdef LCD
   /*
   ** title needs a bit more room on the LCD
   */
   #define TITLE_HEIGHT ((MAX_Y - MIN_Y) / 9)
#else
   #define TITLE_HEIGHT ((MAX_Y - MIN_Y) / 10)
#endif

/*
** The dimensions/location of the text for the help pages.
*/
#define TEXT_MIN_X   TITLE_MIN_X
#define TEXT_MIN_Y   (TITLE_HEIGHT + TITLE_MIN_Y)
#define TEXT_WIDTH   TITLE_WIDTH
#define TEXT_HEIGHT  (MAX_Y - TEXT_MIN_Y - OUTER_SPACE - FLAT_WIDTH - (BORDER_WIDTH << 1))

/*
** The dimensions/location of the page number message for the help pages.
**
*/
#define PAGE_MIN_X   (((TITLE_MIN_X + TITLE_WIDTH) * 3) >> 2)
#define PAGE_MIN_Y   TITLE_MIN_Y
#define PAGE_WIDTH   (TITLE_MIN_X + TITLE_WIDTH - PAGE_MIN_X)
#define PAGE_HEIGHT  TITLE_HEIGHT

#ifdef LCD
   #define PAGE_NUMBER_FONT   "Helvetica 12"
#else
   #define PAGE_NUMBER_FONT   "Helvetica 20"
#endif

#define PAGE_NUMBER_SWIDTH 7
#define PAGE_NUMBER_KERN   2


/*
** Define the function prototypes for the internal routines
*/
INT1 help_display_topic (HELP_INFO *game_help, INT1 topic);
INT1 help_display_page  (HELP_INFO *game_help, INT1 topic, INT1 page);


/****************************************************************************
**
**  Unit Name:
**      help_display
**
**  Description:
**      This function is the main help function.  To display its
**      help text all a program has to do is call this function.
**      This function is an interface to the help routines.
**
**  Calling Convention:
**      help_display (HELP_INFO *game_help)
**
**  Parameters:
**      Name            Description
**      ----            -----------
**      game_help       pointer to a structure containing all of the
**                      help information for the particular program
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/* CALL HELP_MENU */
/* IF THE RETURN_VALUE IS EXIT */
/* .  RETURN */
/* ELSE */
/* .  SEARCH FOR REQUESTED TOPIC IN HELP STRUCTURE */
/* .  CALL HELP_DISPLAY_TOPIC WITH THAT TOPIC */
/* END IF */
/* RETURN */

INT1 help_display (HELP_INFO *game_help)
{
   register INT1 x;
   register INT2 choice;
   register char char_choice;
   register BOOL valid;

   INT2 bounding_x1;
   INT2 bounding_x2;
   INT2 bounding_y1;
   INT2 bounding_y2;

   INT2 title_x1;
   INT2 title_x2;
   INT2 title_y1;
   INT2 title_y2;

   INT2 choice_x1;
   INT2 choice_x2;
   INT2 choice_y1;
   INT2 choice_y2;

   INT2 width;
   INT2 title_width;
   INT2 total_height;
   INT2 ascender;
   INT2 descender;
   INT2 title_ascender;
   INT2 title_descender;
   char menu_title[40];
   char menu[MAX_ENTRIES * (MAX_TOPIC_LEN + 5)];
   FONT_ATTR attr;

   /*
   ** Set the font to the default help font.
   */
   if (font_select (game_help->font) < 0) {
      return (FONT_ERROR);
   }

   /*
   ** Get the current font attributes and change the leading.
   */
   if (font_get_attributes (&attr)) {
      return (HELP_ERROR);
   }
   attr.leading = HELP_MENU_LEADING;
   if (font_set_attributes (&attr)) {
      return (HELP_ERROR);
   }

   /*
   ** Build the string to be printed for the menu.
   */
   menu[0] = '\0';
   for (x = 0; x < (game_help->topic_count - 1); x++) {
      strcat (menu, game_help->topics[x].topic_name);
      strcat (menu, "\\n");
   }
   strcat (menu, game_help->topics[game_help->topic_count - 1].topic_name);

   /*
   ** Get the dimensions of the built string.
   */
   if (font_string_size (menu, &width, &ascender, &descender)) {
      return (HELP_ERROR);
   }

   /*
   ** Build the title of the menu and adjust the coordinates of the
   ** choices window to account for this window.
   ** Italics don't look so well on LCD so it is turned off
   */
#ifdef LCD
   sprintf (menu_title, "%s Help Menu", game_help->app_name);
#else
   sprintf (menu_title, "\\i%s\\i Help Menu", game_help->app_name);
#endif

   if (font_string_size (menu_title, &title_width,
                         &title_ascender, &title_descender)) {
      return (HELP_ERROR);
   }

   /*
   ** Based on the size of the text, determine the coordinates
   ** and size of the bounding rectangle.
   */
   total_height = ascender + descender + title_ascender +
                  title_descender + (INNER_SPACE << 2);
   bounding_y1 = ((MAX_Y - MIN_Y - total_height) >> 1) + MIN_Y -
                 FLAT_WIDTH - (BORDER_WIDTH << 1);
   bounding_y2 = bounding_y1 + total_height;

   title_y1 = bounding_y1;
   title_y2 = title_y1 + title_ascender + title_descender + (INNER_SPACE << 1);

   choice_y1 = title_y2 + 1;
   choice_y2 = choice_y1 + ascender + descender + (INNER_SPACE << 1) - 1;

   if (title_width > width) {
      bounding_x1 = ((MAX_X - MIN_X - title_width) >> 1) + MIN_X;
      bounding_x2 = bounding_x1 + title_width;
   }
   else {
      bounding_x1 = ((MAX_X - MIN_X - width) >> 1) + MIN_X;
      bounding_x2 = bounding_x1 + width;
   }

   bounding_x1 -= INNER_SPACE;
   bounding_x2 += INNER_SPACE;

   title_x1 = bounding_x1;
   title_x2 = bounding_x2;

   choice_x1 = bounding_x1;
   choice_x2 = bounding_x2;

   /*
   ** Keep letting the user select different help topics
   ** until they press the EXIT key.
   */
   do {
      /*
      ** Clear the screen and print the menu.
      */
      gr_clear_scr (GR_GRAY);

#ifdef LCD
      gr_rect (title_x1,
               title_y1,
               title_x2 - title_x1 + 1,
               title_y2 - title_y1 + 1,
               BORDER_WIDTH,
               GR_BLACK,
               GR_WHITE);

      gr_rect (choice_x1,
               choice_y1- BORDER_WIDTH,
               choice_x2 - choice_x1 + 1,
               choice_y2 - choice_y1 + 1,
               BORDER_WIDTH,
               GR_BLACK,
               GR_WHITE);
#else
      gr_bevel_rect (bounding_x1 - FLAT_WIDTH - (BORDER_WIDTH << 1),
                     bounding_y1 - FLAT_WIDTH - (BORDER_WIDTH << 1),
                     bounding_x2 - bounding_x1 + 1 + (FLAT_WIDTH << 1) + (BORDER_WIDTH << 2),
                     bounding_y2 - bounding_y1 + 1 + (FLAT_WIDTH << 1) + (BORDER_WIDTH << 2),
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_OUT);

      gr_bevel_rect (bounding_x1 - BORDER_WIDTH,
                     bounding_y1 - BORDER_WIDTH,
                     bounding_x2 - bounding_x1 + 1 + (BORDER_WIDTH << 1),
                     bounding_y2 - bounding_y1 + 1 + (BORDER_WIDTH << 1),
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_IN);

      gr_rect (title_x1,
               title_y1,
               title_x2 - title_x1 + 1,
               title_y2 - title_y1 + 1,
               1,
               GR_BLACK,
               GR_WHITE);

      gr_rect (choice_x1,
               choice_y1,
               choice_x2 - choice_x1 + 1,
               choice_y2 - choice_y1 + 1,
               1,
               GR_BLACK,
               GR_WHITE);
#endif

      if (font_printf_centered (menu_title, title_x1, title_y1,
                                title_x2, title_y2)) {
         return (HELP_ERROR);
      }

      if (font_printf_centered (menu, choice_x1, choice_y1,
                                choice_x2, choice_y2)) {
         return (HELP_ERROR);
      }

      /*
      ** Keep looping until the user presses either the EXIT key
      ** or a valid SELECT-letter combination.
      */
      valid = FALSE;
      do {
         choice = inp_getch ();

         if ((choice == EXIT_KEY) || (choice == RETURN_KEY)){
            valid = TRUE;
         }
         else {
            /*
            ** If this is a SELECT-letter combination then see
            ** if this is one of the topic letters.  If it is,
            ** then wipe the menu off and display the topic.
            */
            if (((choice & 0xFF00) == SELECT_KEY) || ((choice & 0xFF00) == 0)) {
               char_choice = toupper ((char) (choice & 0x00FF));
               for (x = 0; x < game_help->topic_count; x++) {
                  if (game_help->topics[x].topic_char == char_choice) {
                     gr_rect (bounding_x1 - FLAT_WIDTH - (BORDER_WIDTH << 1),
                              bounding_y1 - FLAT_WIDTH - (BORDER_WIDTH << 1),
                              bounding_x2 - bounding_x1 + 1 + (FLAT_WIDTH << 1) + (BORDER_WIDTH << 2),
                              bounding_y2 - bounding_y1 + 1 + (FLAT_WIDTH << 1) + (BORDER_WIDTH << 2),
                              0,
                              GR_NONE,
                              GR_GRAY);
                     if (help_display_topic (game_help, x)) {
                        return (HELP_ERROR);
                     }
                     valid = TRUE;
                     break;
                  }
               }
            }
         }
      } while (!valid);
   } while ((choice != EXIT_KEY) && (choice != RETURN_KEY));

   return (HELP_SUCCESS);
}


/****************************************************************************
**
**  Unit Name:
**      help_display_topic
**
**  Description:
**      Displays a page of help text to the screen and prompts
**      the user to move up, up/down, or down depending on where
**      in the help text the user is.
**
**  Calling Convention:
**      help_display_topic (HELP_INFO *game_help, INT1 topic)
**
**  Parameters:
**      Name            Description
**      ----            -----------
**      game_help       pointer to the help structure
**      topic_num       the number of the topic to display
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/* WHILE (USER HAS NOT PRESSED EXIT) */
/* .  IF (USER PRESSES PAGE_UP) THEN */
/* .  .  DISPLAY PREVIOUS PAGE IF THERE IS ONE */
/* .  ELSE IF (USER PRESSES PAGE_DOWN) THEN */
/* .  .  DISPLAY NEXT PAGE IF THERE IS ONE */
/* .  ELSE IF (USER PRESSES TOP) THEN */
/* .  .  GO TO THE FIRST PAGE IF NOT ALREADY THERE */
/* .  ELSE IF (USER PRESSES BOTTOM) THEN */
/* .  .  GO TO THE LAST PAGE IF NOT ALREADY THERE */
/* .  ELSE */
/* .  .  DO NOTHING */
/* .  END IF */
/* END WHILE */
/* RETURN */

INT1 help_display_topic (HELP_INFO *game_help, INT1 topic)
{
   BOOL exit = FALSE;
   BOOL display_page = FALSE;
   INT1 page;
   UINT2 key;

   /*
   ** Display the first page of the help text.
   */
   page = game_help->topics[topic].first_page;
   if (help_display_page (game_help, topic, page)) {
      return (HELP_ERROR);
   }

   /*
   ** Let the user move up and down through the help text
   ** until they decide to exit.
   */
   while (!exit) {
      key = inp_getch ();
      display_page = FALSE;
      switch (key) {
         case '7':
         case SCRN_UP_KEY:
         case CODE_KEY|CURSUP_KEY:
            if (page - 1 >= game_help->topics[topic].first_page) {
               page--;
               display_page = TRUE;
            }
            break;
         case '1':
         case SCRN_DOWN_KEY:
         case CODE_KEY|CURSDOWN_KEY:
            if (page + 1 <= game_help->topics[topic].first_page +
                            game_help->topics[topic].page_count - 1) {
               page++;
               display_page = TRUE;
            }
            break;
         case '5':
         case ADVANCE_KEY:
            key = inp_getch ();
            switch (key) {
               case '8':
               case CURSUP_KEY:
                  if (page != game_help->topics[topic].first_page) {
                     page = game_help->topics[topic].first_page;
                     display_page = TRUE;
                  }
                  break;
               case '2':
               case CURSDOWN_KEY:
                  if (page != game_help->topics[topic].first_page +
                              game_help->topics[topic].page_count - 1) {
                     page = game_help->topics[topic].first_page +
                            game_help->topics[topic].page_count - 1;
                     display_page = TRUE;
                  }
                  break;
               default:
                  break;
            }
            break;
         case RETURN_KEY:
         case EXIT_KEY:
            exit = TRUE;
            break;
         default:
            break;
      }
      /*
      ** Only display the page if the page number actually changed.
      ** This keeps the screen from flickering if the user hits
      ** PAGE_UP too many times, for example.
      */
      if (display_page) {
         if (help_display_page (game_help, topic, page)) {
            return (HELP_ERROR);
         }
      }
   }

   return (HELP_SUCCESS);
}






/****************************************************************************
**
**  Unit Name:
**      help_display_page
**
**  Description:
**      Displays a page of help text to the screen and prompts
**      the user to move up, up/down, or down depending on where
**      in the help text the user is.
**
**  Calling Convention:
**      help_display_page (HELP_INFO *game_help, INT1 page)
**
**  Parameters:
**      Name            Description
**      ----            -----------
**      game_help       pointer to the help structure
**      page            the number of the page to display
**
**  Return Value:
**      HELP_SUCCESS if successful, HELP_ERROR otherwise
**
**  Notes:
**      none
**
*****************************************************************************/
/* SAVE THE FONT ATTRIBUTES */
/* DRAW A WINDOW ON THE SCREEN */
/* MAKE A BOX AT THE TOP FOR A TITLE */
/* IN THE TITLE BOX PUT THE NAME OF THE APPLICATION AND THE HELP TOPIC */
/* PRINT THE SELECTED PAGE OF HELP IN THE WINDOW */
/* RESTORE THE FONT ATTRIBUTES */
/* RETURN */
INT1 help_display_page (HELP_INFO *game_help, INT1 topic, INT1 page)
{
   char text[MAX_TEXT_LEN];
   INT1 old_underline_thickness;
   INT2 x;
   FONT_COORD pos;
   FONT_ATTR attr;
   FONT_ATTR old_attr;

   /*
   ** Save the current attributes so they can be reset at the end
   ** of this routine.  This will also reset the attributes to
   ** the default so that nothing unpredicted will happen with the
   ** text.
   */
   if (font_reset_attributes (&attr, &old_attr)) {
      pop_up("default attr error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }

   /*
   ** Since the topic name is going to be printed and it has
   ** underlining control characters in it they will have to
   ** be rendered impotent.  Change the underline thickness
   ** to zero so that no underline will print.
   */
   if (font_get_attributes (&attr)) {
      pop_up("underline get attr error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }
   old_underline_thickness = attr.underline_thickness;
   attr.underline_thickness = 0;
   if (font_set_attributes (&attr)) {
      pop_up("underline set attr error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }

   /*
   ** If this is running on a CRT machine then put a bevel around the
   ** windows.
   */
#ifndef LCD
   gr_bevel_rect (TITLE_MIN_X - FLAT_WIDTH - (BORDER_WIDTH << 1),
                  TITLE_MIN_Y - FLAT_WIDTH - (BORDER_WIDTH << 1),
                  TITLE_WIDTH + (FLAT_WIDTH << 1) + (BORDER_WIDTH << 2),
                  TITLE_HEIGHT + TEXT_HEIGHT + (FLAT_WIDTH << 1) + (BORDER_WIDTH << 2),
                  BORDER_WIDTH,
                  GR_NONE,
                  BEVEL_OUT);

   gr_bevel_rect (TITLE_MIN_X - BORDER_WIDTH,
                  TITLE_MIN_Y - BORDER_WIDTH,
                  TITLE_WIDTH + (BORDER_WIDTH << 1),
                  TITLE_HEIGHT + TEXT_HEIGHT + (BORDER_WIDTH << 1),
                  BORDER_WIDTH,
                  GR_NONE,
                  BEVEL_IN);

#endif

   /*
   ** Draw a title rectangle on the screen.
   */
#ifdef LCD
   gr_rect (TITLE_MIN_X,
            TITLE_MIN_Y,
            TITLE_WIDTH,
            TITLE_HEIGHT,
            BORDER_WIDTH,
            GR_BLACK,
            GR_WHITE);
#else
   gr_rect (TITLE_MIN_X,
            TITLE_MIN_Y,
            TITLE_WIDTH,
            TITLE_HEIGHT,
            1,
            GR_BLACK,
            GR_WHITE);
#endif

   /*
   ** Print the text in the window at the top of the screen.
   ** Italics does not look good on LCD
   */
#ifdef LCD
   sprintf (text, "%s - %s", game_help->app_name,
            game_help->topics[topic].topic_name);
#else
   sprintf (text, "\\i%s\\i - %s", game_help->app_name,
            game_help->topics[topic].topic_name);
#endif
   font_printf_centered (text,
                         TITLE_MIN_X,
                         TITLE_MIN_Y,
                         TITLE_WIDTH + TITLE_MIN_X,
                         TITLE_HEIGHT + TITLE_MIN_Y - 2);

                         /* The (-2) is an adjustment to center the string */
                         /* in the window */

   /*
   ** Set underlining back to whatever it was before.
   */
   attr.underline_thickness = old_underline_thickness;
   if (font_set_attributes (&attr)) {
      pop_up("reset underline attr error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }

   /*
   ** Print the page number, right justified, in the title window.
   ** Make it a smaller font.
   */
   if (font_select (PAGE_NUMBER_FONT) < 0) {
      pop_up("page font select error",0,0,gr_scr_width(),gr_scr_height());
      return (FONT_ERROR);
   }
   attr.space_width = PAGE_NUMBER_SWIDTH;
   attr.kerning     = PAGE_NUMBER_KERN;
   if (font_set_attributes (&attr)) {
      pop_up("page num  set attr error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }
   sprintf (text, "Page %d of %d",
            page - game_help->topics[topic].first_page + 1,
            game_help->topics[topic].page_count);
   if (font_printf_centered (text,
                             PAGE_MIN_X,
                             PAGE_MIN_Y,
                             PAGE_MIN_X + PAGE_WIDTH,
                             PAGE_MIN_Y + PAGE_HEIGHT - 2)) {
                         /* The (-2) is an adjustment to center the string */
                         /* in the window */

      pop_up("page num printf centered error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }

   /*
   ** Draw a text rectangle on the screen.
   */
#ifdef LCD
   gr_rect (TEXT_MIN_X,
            TEXT_MIN_Y - BORDER_WIDTH, /* Overlap the borders */
            TEXT_WIDTH,
            TEXT_HEIGHT,
            BORDER_WIDTH,
            GR_BLACK,
            GR_WHITE);
#else
   gr_rect (TEXT_MIN_X,
            TEXT_MIN_Y, /* Butt the rectangles together */
            TEXT_WIDTH,
            TEXT_HEIGHT,
            1,
            GR_BLACK,
            GR_WHITE);
#endif

   /*
   ** Move to the first line on the help screen.
   */
   pos.x = TEXT_MIN_X + INNER_SPACE;
   pos.y = TEXT_MIN_Y + INNER_SPACE + attr.height + attr.leading;
   if (font_set_pos (&pos)) {
      pop_up("help text set pos error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }

   /*
   ** Print out each of the lines of text.  Be sure to prepend the
   ** text with a '\n' so that the font printing routine will take
   ** care of positioning the focus at the start of the next line
   ** for us.  Before each line of text is printed, set the font
   ** and set the line's attributes.  To set a font only knowing the
   ** ID you have to call font_printf with the '\f' notation.
   */
   for (x = game_help->pages[page].first_record;
        x < game_help->pages[page].first_record +
            game_help->pages[page].record_count;
        x++) {
      attr.space_width         = game_help->records[x].space_width;
      attr.tab_width           = game_help->records[x].tab_width;
      attr.kerning             = game_help->records[x].kerning;
      attr.leading             = game_help->records[x].leading;
      attr.underline_thickness = game_help->records[x].underline_thickness;
      attr.underline_offset    = game_help->records[x].underline_offset;
      if (font_set_attributes (&attr)) {
         pop_up("help text set attr error",0,0,gr_scr_width(),gr_scr_height());
         return (HELP_ERROR);
      }
      sprintf (text, "\\f%d%s",
               game_help->records[x].font_id,
               game_help->records[x].text);
      if (x < game_help->pages[page].first_record +
              game_help->pages[page].record_count - 1) {
         strcat (text, "\\n");
      }
      if (font_printf (text)) {
         pop_up("Help text printf error",0,0,gr_scr_width(),gr_scr_height());
         return (HELP_ERROR);
      }
   }

   /*
   ** Set the font back to what it was before this routine was
   ** called.
   */
   if (font_select (game_help->font) < 0) {
      pop_up("reset font select error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }

   /*
   ** Set the font attributes back to what they were before this
   ** routine was called.
   */
   if (font_set_attributes (&old_attr)) {
      pop_up("reset attr error",0,0,gr_scr_width(),gr_scr_height());
      return (HELP_ERROR);
   }
   return (HELP_SUCCESS);
}

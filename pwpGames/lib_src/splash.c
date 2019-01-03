/*****************************************************************************
**
**  Module Name:
**      splash
**
**  Description:
**      Display the PWP Entertainment pack opening banner screen
**
**  Exported Units:
**      display_splash_scr
**      display_splash_scr_license
**      window_title
**
**  Local Units:
**
**  Exported Data:
**      none
**
**  Notes:
**      none
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      04/13/92    Initial version
**
**
*****************************************************************************/

#include "sc_def.h"
#include "gr.h"
#include "inp.h"
#include "fontlib.h"
#include "splash.h"

   extern void waitsecs();

/*
** The version number that the games are at.
*/
#define VERSION "1.0"

/*
** Set up the fonts that the text is to be printed in.
*/
#ifdef LCD
   #define  TITLE_FONT              2 /* "Helvetica 20" */
   #define  TEXT_FONT               0 /* "Helvetica 12" */
#else
      #define  TITLE_FONT           2 /* "Helvetica 30" */
      #define  TEXT_FONT            0 /* "Helvetica 20" */
#endif

#define TETRIS_LINE_1 "@1987 v/o Electronorgtechnica (Elorg).  All Rights Reserved.  TETRIS is a\\n"
#define TETRIS_LINE_2 "registered trademark of Elorg.  TETRIS copyright and trademark licensed\\n"
#define TETRIS_LINE_3 "to Sphere, Inc and sublicensed to Smith Corona.  Original concept by\\n"
#define TETRIS_LINE_4 "Alexey Pajitnov.  Smith Corona Version @1992, Smith Corona Corporation.\\n"
#define TETRIS_LINE_5 "Developed by Eclat Technologies for Smith Corona."


/*
** How long to wait before pulling down the splash screen.
*/
#define  SPLASH_DELAY               5  /* In seconds */

/*
** The center of the screen horizontally.
*/
#define MID_X             (((MAX_X - MIN_X) >> 1) + MIN_X)

/*
** The center of the screen vertically.
*/
#define MID_Y             (((MAX_Y - MIN_Y) >> 1) + MIN_Y)

/*
** The outer left and right columns.
*/
#define LFT_X             (((MAX_X - MIN_X) / 6) + MIN_X)
#define RGT_X             (LFT_X * 5)

/*
** The different types of data this porgram can handle.
*/
typedef enum image_type { LOGO, STRING, END } IMAGE_TYPE;

/*
** A structure that holds all the necessary information about any image
** listed in IMAGE_TYPE.
*/
typedef struct image {
   IMAGE_TYPE type;
   INT1       font;
   INT1       style;
   INT2       x;
   INT2       y;
   PIXMAP     *picture;
   char       *text;
} IMAGE;

/*
*************************   GLOBAL DATA    *******************************
*/

/*
** The Smith Corona logo.
*/
PIXMAP sclogo[]=
{
   #ifdef LCD
      #include "sclgLCD.pix"
   #else
      #include "sclgCRT.pix"
   #endif
};

/*
** The ETI logo.
*/
#ifndef LCD
   PIXMAP etilogo[]=
   {
      #include "etilgCRT.pix"
   };
#endif

/***************************************************************************
**
** Unit Name:
**    display_splash_scr
**
** Description:
**    displays a banner page before an application actually starts
**    (credits and copyrights)
**
**  Calling convention:
**      display_splash_scr (char *game_title)
**
** Parameters:
**    title -   a char pointer to a string containing the title of the
**              calling game
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void display_splash_scr(char *game_title)
{
   FONT_ATTR   attr;
   UINT1       i;
   char        text[10];

#ifdef LCD
   IMAGE credits[] = {
      {  STRING, TITLE_FONT, FONT_STYLE_ITALIC, MID_X,  25,       0, ""},
      {    LOGO,          0,                 0, LFT_X,  40,  sclogo, ""},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, LFT_X,  80,       0, "PWP Game Pack\\nVersion " VERSION},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, RGT_X,  70,       0, "Developed by:\\nEclat Technologies, Inc.\\nSyracuse, New York"},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 100,       0, "Copyright @ 1992\\nby Smith Corona Corporation, U.S.A.\\nAll rights reserved"},
      {     END,          0,                 0,     0,   0,       0, ""}
   };

   credits[0].text = game_title;
#else
   IMAGE credits[] = {
      {    LOGO,          0,                 0, MID_X,  20,  sclogo, ""},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 115,       0, "PWP Game Pack\\nVersion " VERSION},
      {  STRING, TITLE_FONT, FONT_STYLE_ITALIC, MID_X, 180,       0, ""},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 270,       0, "Copyright @ 1992\\nby Smith Corona Corporation, U.S.A.\\nAll rights reserved"},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 370,       0, "Developed by:"},
      {    LOGO,          0,                 0, MID_X, 385, etilogo, ""},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 440,       0, "Eclat Technologies, Inc.\\nSyracuse, New York"},
      {     END,          0,                 0,     0,   0,       0, ""}
   };

   credits[2].text = game_title;
#endif

   gr_clear_scr(GR_WHITE);

   /*
   ** You have to call font_select to initialize the font routines
   ** before you can use the \f notation.  It does not really matter
   ** what font you select, as long as it is valid.
   */
   font_select ("Helvetica 20");

   for (i = 0; credits[i].type != END; i++) {
      switch (credits[i].type) {
         case LOGO:
            gr_pixmap (credits[i].x - (gr_pixmap_width (credits[i].picture) >> 1),
                       credits[i].y,
                       credits[i].picture,
                       GROP_SRC);
            break;
         case STRING:
            font_get_attributes (&attr);
            attr.style ^= credits[i].style;
            font_set_attributes (&attr);

            sprintf (text, "\\F%d", credits[i].font);
            font_printf (text);
            
            font_printf_centered_each (credits[i].text,
                                       credits[i].x,
                                       credits[i].y,
                                       credits[i].x,
                                       credits[i].y);

            attr.style ^= credits[i].style;
            font_set_attributes (&attr);
            break;
         default:
            break;
      }
   }

   i = 0;
   while (i < SPLASH_DELAY)
   {
      waitsecs(1);
      i++;
      if (inp_kbhit())
      {
         inp_getch();
         break;
      }
   }

   return;
}


/***************************************************************************
**
** Unit Name:
**    display_splash_scr_license
**
** Description:
**    displays a banner page before an application actually starts
**    (credits and copyrights) and also displays the Tetris license
**    information
**
**  Calling convention:
**      display_splash_scr_license (char *game_title)
**
** Parameters:
**    title -   a char pointer to a string containing the title of the
**              calling game
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void display_splash_scr_license (char *game_title)
{
   FONT_ATTR   attr;
   UINT1       i;
   char        text[10];

#ifdef LCD
   IMAGE credits[] = {
      {  STRING, TITLE_FONT, FONT_STYLE_ITALIC, MID_X,  35,       0, ""},
      {    LOGO,          0,                 0, LFT_X,  10,  sclogo, ""},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, LFT_X,  50,       0, "PWP Game Pack\\nVersion " VERSION},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, RGT_X,  40,       0, "Developed by:\\nEclat Technologies, Inc.\\nSyracuse, New York"},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 110,       0, TETRIS_LINE_1 TETRIS_LINE_2 TETRIS_LINE_3 TETRIS_LINE_4 TETRIS_LINE_5},
      {     END,          0,                 0,     0,   0,       0, ""}
   };

   credits[0].text = game_title;
#else
   IMAGE credits[] = {
      {    LOGO,          0,                 0, MID_X,  20,  sclogo, ""},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 115,       0, "PWP Game Pack\\nVersion " VERSION},
      {  STRING, TITLE_FONT, FONT_STYLE_ITALIC, MID_X, 180,       0, ""},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 274,       0, TETRIS_LINE_1 TETRIS_LINE_2 TETRIS_LINE_3 TETRIS_LINE_4 TETRIS_LINE_5},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 370,       0, "Developed by:"},
      {    LOGO,          0,                 0, MID_X, 385, etilogo, ""},
      {  STRING,  TEXT_FONT, FONT_STYLE_NORMAL, MID_X, 440,       0, "Eclat Technologies, Inc.\\nSyracuse, New York"},
      {     END,          0,                 0,     0,   0,       0, ""}
   };

   credits[2].text = game_title;
#endif

   gr_clear_scr(GR_WHITE);

   /*
   ** You have to call font_select to initialize the font routines
   ** before you can use the \f notation.  It does not really matter
   ** what font you select, as long as it is valid.
   */
   font_select ("Helvetica 20");

   for (i = 0; credits[i].type != END; i++) {
      switch (credits[i].type) {
         case LOGO:
            gr_pixmap (credits[i].x - (gr_pixmap_width (credits[i].picture) >> 1),
                       credits[i].y,
                       credits[i].picture,
                       GROP_SRC);
            break;
         case STRING:
            font_get_attributes (&attr);
            attr.style ^= credits[i].style;
#ifdef LCD
            attr.leading -= 2;
#endif
            font_set_attributes (&attr);

            sprintf (text, "\\F%d", credits[i].font);
            font_printf (text);
            
            font_printf_centered_each (credits[i].text,
                                       credits[i].x,
                                       credits[i].y,
                                       credits[i].x,
                                       credits[i].y);

            attr.style ^= credits[i].style;
#ifdef LCD
            attr.leading += 2;
#endif
            font_set_attributes (&attr);
            break;
         default:
            break;
      }
   }

   i = 0;
   while (i < SPLASH_DELAY)
   {
      waitsecs(1);
      i++;
      if (inp_kbhit())
      {
         inp_getch();
         break;
      }
   }

   return;
}





/***************************************************************************
**
** Unit Name:
**    window_title
**
** Description:
**    displays a bar along the top or sides of the screen naming the
**    game and Smith Corona.
**
**  Calling convention:
**      (INT1) status = window_title (char *app_name)
**
** Parameters:
**      app_name - Pointer to a string containing the name of the game.
**                 On the LCD machines the name must be formatted so
**                 that it will display vertically.
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

/*
** Given an application name draw a title on the screen
** naming the application and Smith Corona.
*/
INT1 window_title (char *app_name)
{
   char text[100];
   INT1 i;
   INT1 j;

   /*
   ** Get a font to print the text in.  The smaller the better.
   */
   if (font_select (SPLASH_FONT) < 0) {
      return (SPLASH_ERROR);
   }

#ifdef LCD
   strcpy (text, app_name);

   gr_rect (MIN_X,
            MIN_Y,
            LEFT_ADJUSTMENT,
            MAX_Y - MIN_Y + 1,
            0, 
            GR_NONE,
            GR_WHITE);

   gr_line (MIN_X + LEFT_ADJUSTMENT,
            MIN_Y,
            MIN_X + LEFT_ADJUSTMENT,
            MAX_Y,
            1,
            GR_BLACK);

   font_printf_centered_each (MESSAGE,
                              MIN_X,
                              MIN_Y,
                              MIN_X + LEFT_ADJUSTMENT,
                              MAX_Y);

   gr_rect (MAX_X - RIGHT_ADJUSTMENT + 1,
            MIN_Y,
            RIGHT_ADJUSTMENT,
            MAX_Y - MIN_Y + 1,
            0, 
            GR_NONE,
            GR_WHITE);

   gr_line (MAX_X - RIGHT_ADJUSTMENT,
            MIN_Y,
            MAX_X - RIGHT_ADJUSTMENT,
            MAX_Y,
            1,
            GR_BLACK);

   font_printf_centered_each (text,
                              MAX_X - RIGHT_ADJUSTMENT,
                              MIN_Y,
                              MAX_X,
                              MAX_Y);

#else
   /*
   ** Draw the title bar.
   */
   gr_rect (TITLE_X, 
            TITLE_Y,
            TITLE_WIDTH, 
            TITLE_HEIGHT,
            TITLE_BORDER_WIDTH,
            TITLE_BORDER,
            TITLE_FOREGROUND);

   /*
   ** Draw a line under the title bar to offset it from
   ** the rest of the background.
   */
   gr_line (TITLE_X, 
            TITLE_HEIGHT, 
            TITLE_X + TITLE_WIDTH, 
            TITLE_Y + TITLE_HEIGHT, 
            LINE_WIDTH, 
            LINE_COLOR);

   /*
   ** Build the message string and print it.
   */
   sprintf (text, MESSAGE, app_name);
   if (font_printf_centered (text, 
                             TITLE_X, 
                             TITLE_Y, 
                             TITLE_X + TITLE_WIDTH, 
                             TITLE_Y + TITLE_HEIGHT)) {
      return (SPLASH_ERROR);
   }
#endif

   /*
   ** If this was reached no errors were reported.  Return
   ** a success.
   */
   return (SPLASH_SUCCESS);
}





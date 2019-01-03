/*****************************************************************************
**
**  Header Name:
**      splash.h
**
**  Description:
**      This header has the defintions for the splash screen and the title bar.
**
**  Notes:
**      none
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      02/22/92    Initial version
**
*****************************************************************************/

#ifndef splash_h
#define splash_h

#include "sc_def.h"

/*
** The size and location of the title.
*/
#ifdef LCD
   #define LEFT_ADJUSTMENT    34
   #define RIGHT_ADJUSTMENT   LEFT_ADJUSTMENT

   #define MESSAGE            "PWP\\n\\nG\\nA\\nM\\nE\\n\\nPack"
   #define SPLASH_FONT        "Helvetica 12"
#else
   #define TITLE_X            0
   #define TITLE_Y            0
   #define TITLE_HEIGHT       30
   #define TITLE_WIDTH        (gr_scr_width ())

   #define LINE_WIDTH         2
   #define LINE_COLOR         GR_BLACK
   
   #define TITLE_ADJUSTMENT   (TITLE_HEIGHT + LINE_WIDTH)

   #define TITLE_BORDER_WIDTH 0
   #define TITLE_BORDER       GR_WHITE
   #define TITLE_FOREGROUND   GR_WHITE

   #define MESSAGE            "Smith Corona Game Pack - \\i%s\\i"
   #define SPLASH_FONT        "Helvetica 30"
#endif

/*
** Status values.
*/
#define SPLASH_SUCCESS        0
#define SPLASH_ERROR         -1

/*
** Function prototypes.
*/
void display_splash_scr (char *game_title);
void display_splash_scr_license (char *game_title);
INT1 window_title (char *app_name);


#endif

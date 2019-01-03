/*************************************************************************
** Module Name:
**
**    capture
**
** Description:
**
**    A two player CaptureIt (Othello) game. It includes a computer player
**    as an opponent with several levels of difficulty.
**
** Notes:
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    02/06/92       Creation date
**                   legalmove - created and tested
**                   testcapture - created and tested
**
**    02/10/92       Added the weighting for the computer player
**                   Added the computer as one of the possible players
**
**    03/24/92       The minimax algorithm for finding moves was implemented
**                   and included
**
**    04/06/92       The move algorithm is in its final stages (it works
**                   but needs to be speeded up)
**
**************************************************************************/

/*
*************************  INCLUDED FILES  *******************************
*/

#include "sc_def.h"     /* type defines                     */
#include "gr.h"         /* graphics routines                */
#include "inp.h"        /* keyboard routines                */
#include "splash.h"     /* opening banner screen            */
#include "keys.h"       /* PWP keyboard key defines         */
#include "fontlib.h"    /* Font routines                    */
#include "window.h"     /* windowing functions              */
#include "win_rtns.h"   /* local windowing functions        */
#include "capture.h"    /* CaptureIt header file            */
#include "makemove.h"   /* Move routines                    */
#include "board.h"      /* board routines                   */
#include "cursor.h"     /* cursor (selection box) routines  */
#include "menu.h"       /* menu routines                    */
#include "help.h"       /* local help routines              */


/*
*************************   GLOBAL DATA    *******************************
*/

char  Game_title[] = "CaptureIt";

#ifdef LCD
char  Game_banner_title[] = "C\\nA\\nP\\nT\\nU\\nR\\nE\\nI\\nT";
#else
char  Game_banner_title[] = "CaptureIt";
#endif

/*****************
**              **
** Menu strings **
**              **
*****************/

char  Mainmenu[]  =
  "\\uN\\uew\\n\\uC\\ulue\\n\\uO\\uptions\\n\\uH\\uelp\\n\\uQ\\uuit";

/*
** LCD is different because of smaller screen size was unable to
** put an extra NEWLINE in middle of options menu string
*/

#ifdef LCD
   char  Optmenu[]  =
     "\\uB\\ueginner\\n\\uN\\uovice\\n\\uE\\uxpert\\n\\uO\\une Player\\n\\uT\\uwo Player\\n\\uD\\uemo";
#else
   char  Optmenu[]  =
      "\\uB\\ueginner\\n\\uN\\uovice\\n\\uE\\uxpert\\n\\n\\uO\\une Player\\n\\uT\\uwo Player\\n\\uD\\uemo";
#endif


UINT1 Skill_level=LVL_BEGIN;  /* 1 = beginner */
                              /* 3 = novice */
                              /* 5 = expert */

UINT1 Num_players=1;          /* 0 = demo */
                              /* 1 = one player */
                              /* 2 = two players */

BOOL   No_move_left = FALSE;     /* TRUE when no move available for either player */

BOOL   Options_changed;       /* TRUE if the options have been changed */

/*
** Score structure returned from countscore()
*/
struct SCORE
{
   UINT1 player1;
   UINT1 player2;
};

/*
** current sizes of the menu window
** used only for the thinking window on the CRT
** Window is in a different place on the LCD
*/

#ifndef LCD
extern INT2 Menu_x;
extern INT2 Menu_y;
extern INT2 Menu_width;
extern INT2 Menu_height;
#endif

/*
*************************  LOCAL PROCEDURES  *******************************
*/

UINT2          get_next_event          (void);
void           option_menu             (struct BOARD *game_board,
                                          INT1 *x,
                                          INT1 *y,
                                          INT1 *player,
                                          BOOL *marker);
void           newgame                 (struct BOARD *game_board,
                                        struct SCORE *score,
                                          INT1 *player,
                                          INT1 *x,
                                          INT1 *y);
struct SCORE   countscore              (struct BOARD *game_board);
void           draw_status_win         (void);
void           display_status_message  (char *message);
void           print_status_score      (struct SCORE *current_score);
void           print_status_player     (INT1  player);
void           print_status_level      (UINT1 lvl);
void           game_over_message       (struct SCORE *current_score);
void           display_thinking        (void);
void           erase_thinking          (void);

/***************************************************************************
**                                                                        **
** main - capture                                                         **
**                                                                        **
***************************************************************************/

void   app_main(void)
{
   INT1   x = 0;         /* The current x location relative to the game board */
   INT1   y = 0;         /* The current y location relative to the game board */

   INT1   current_player = PLAYER1;  /* current player */
   
   BOOL   quit_game = FALSE;  /* BOOLEAN - TRUE when user wants to quit game */
   BOOL   move_marker;        /* BOOLEAN - TRUE if a space is a legal move */
   
   struct BOARD    gameboard; /* The game board */
   struct SCORE    score;

   UINT2 menu_event;

   srand(ANY_NUMBER);         /* seed the random number for MINIMAX.C */

   /*
   ** display opening logo screen
   */
   
   display_splash_scr(Game_title);
   gr_clear_scr(BACKGROUND);

   /*
   ** display the title banner
   */

   window_title(Game_banner_title);

   /*
   ** display the main menu
   */
   display_menu(Mainmenu);    /* draw the menu */

   /*
   ** Setup a new game
   */
   draw_status_win ();
   newgame(&gameboard,&score,&current_player,&x,&y);

   /*
   ** draw the game board
   */
   
   drawboard(  BRD_START_X,
               BRD_START_Y);
   /*
   ** draw the starting tokens (circles)
   */
   drawtokens(&gameboard);

   /*
   ** draw the cursor
   */
   draw_cursor(x,y);


   /***********************************
   **                                **
   ** MAIN Keyboard input event loop **
   **                                **
   ***********************************/
   
   while(!quit_game)
   {
      menu_event = get_next_event();

      switch(menu_event)
      {
         /*
         ** Cursor movement events
         */
         
         case UP_ARROW_KEY:
         case DOWN_ARROW_KEY:
         case RIGHT_ARROW_KEY:
         case LEFT_ARROW_KEY:
         case UP_NUM_KEY:         
         case DOWN_NUM_KEY:         
         case RIGHT_NUM_KEY:         
         case LEFT_NUM_KEY:         
            if (move_marker)
            {
               erasemarker(x,y);
               move_marker = FALSE;
            }
            move_cursor(menu_event,&x,&y);  /* move cursor */
            if (legalmove(&gameboard,x,y,current_player) > 0)
            {
               drawmarker(x,y);
               move_marker = TRUE;
            }
            break;

         /*
         ** New Game event
         */
         
         case NEW_MAIN:
            move_marker = FALSE;
            draw_status_win ();
            newgame(&gameboard,&score,&current_player,&x,&y);
            drawboard(  BRD_START_X,      /* draw the game board */
                        BRD_START_Y);
            
            drawtokens(&gameboard);            /* draw the starting tokens (circles) */

            draw_cursor(x,y);
            No_move_left = FALSE;
            break;
         
         /*
         ** Hint event (Suggest)
         */

         case CLUE_MAIN:
            if (Num_players == 1)
            {
               erase_cursor(x,y);
               if (move_marker == TRUE)
               {
               erasemarker(x,y);
               }
               
               /*
               ** display thinking window
               */

               if (Skill_level > LVL_BEGIN)
               {
               display_thinking();
               }
         
               if (findmove(&gameboard,&x,&y,current_player,Skill_level))
               {
               move_marker = TRUE;
               drawmarker(x,y);
               }

               /*
               ** erase thinking window
               */
               if (Skill_level > LVL_BEGIN)
               {
               erase_thinking();
               }
         
               draw_cursor(x,y);
                
            }
            break;
         
         /*
         ** Option menu event
         */
   
         case OPTIONS_MAIN:
            Options_changed = FALSE;
            option_menu(&gameboard,&x,&y,&current_player,&move_marker);
            if (Options_changed)
            {
               move_marker = FALSE;
               draw_status_win ();
               newgame(&gameboard,&score,&current_player,&x,&y);
               drawboard(  BRD_START_X,      /* draw the game board */
                           BRD_START_Y);
            
               drawtokens(&gameboard);            /* draw the starting tokens (circles) */

               draw_cursor(x,y);
               No_move_left = FALSE;
            }
            break;
         
         /*
         ** Help menu event
         */

         case HELP_MAIN:
         case PWP_HELP_KEY:
#ifndef LCD
         case HELP_NUM_KEY:
#endif
            /* Run help */
            
            help();

            /*
            ** restore game screen
            */

            gr_clear_scr(BACKGROUND);
            window_title(Game_banner_title);
            drawboard(  BRD_START_X,         /* draw the game board */
                        BRD_START_Y);

            drawtokens(&gameboard);          /* draw the tokens */
            
            draw_cursor(x,y);                /* draw cursor */

            if (move_marker == TRUE)         /* draw valid move marker */
               drawmarker(x,y);

            display_menu(Mainmenu);          /* draw the main menu */

            draw_status_win ();              /* draw status window */

            print_status_player (current_player); /* show current status info */
            print_status_level  (Skill_level);
            print_status_score  (&score);
            break;

         /*
         ** make move event
         */

         case MAKE_MOVE_SPACE:
         case MAKE_MOVE_RETURN:
            if (legalmove(&gameboard,x,y,current_player)>0)
            {
               makemove(&gameboard,x,y,current_player);
               drawtokens(&gameboard);

               /* No need to erase marker because it will be over written */
               
               move_marker = FALSE;

               current_player = OTHER_PLAYER(current_player);
               print_status_player (current_player);
               
               score = countscore(&gameboard);
               print_status_score (&score);

               /* since cursor was erased in the process it needs to*/
               /* be redrawn */

               draw_cursor(x,y);

            }
            /*
            ** if not a legal move, presently nothing is done
            ** didn't seem neccessary
            ** could popup a message box, but thought it would tend to be
            ** annoying
            */
            break;
         
         /*
         ** quit game event
         */

         case QUIT_MAIN:
         case PWP_EXIT_KEY:
   
            /* Pop up quit confirmation window */

            if (font_select(MENU_FONT) < 0)
            {
               return;
            }

            menu_event = prompt_exit(BRD_START_X + (BOARD_X * SQUARE_X >>1),
                                     BRD_START_Y + (BOARD_Y * SQUARE_Y >>1));
            
            if ((menu_event == 'Y') ||
                (menu_event == 'y') ||
                (menu_event == (SELECT_KEY|'y')))
            {
               quit_game = TRUE;
            }
            break;
      }
      
      /*
      ** Game over conditional - test if game over
      */
      
      if ((((score.player1+score.player2) == 64)||
          (score.player1 == 0) || (score.player2 == 0) ||
          (!moveavailable(&gameboard,current_player) &&
          !(moveavailable(&gameboard,OTHER_PLAYER(current_player))))) &&
          (!No_move_left))
      {
         game_over_message(&score);
         No_move_left = TRUE;
      }

      /*
      ** Conditional - no move available for current player
      */

      if (!moveavailable(&gameboard,current_player) && (!No_move_left))
      {

         display_status_message("No move available\\nPassing turn");

         /*
         ** wait for key press
         */

         inp_getch();
         
         /*
         ** Switch players
         */
         current_player = OTHER_PLAYER(current_player);

         /*
         ** Redraw status information
         ** since the status window is used for PASSING TURN message 
         ** all the info must be redrawn
         */
         draw_status_win ();
         print_status_player (current_player);
         print_status_level (Skill_level);
         print_status_score (&score);
      }

      /*
      ** Conditional for computer move
      ** Could probably make it another event
      */

      if ((Num_players == 1) && (current_player == PLAYER2) && (!No_move_left))
      {
         if (Skill_level > LVL_BEGIN)
         {
            display_thinking();
         }
         if(findmove(&gameboard,&x,&y,current_player,Skill_level))
         {
            erase_cursor(x,y);
            makemove(&gameboard,x,y,current_player);
            drawtokens(&gameboard);
           
            move_marker = FALSE;
         
            score = countscore(&gameboard);
            print_status_score (&score);
         
            draw_cursor(x,y);
         }
         
         /*
         ** erase thinking window
         */
         if (Skill_level > LVL_BEGIN)
         {
            erase_thinking();
         }
         
         current_player = OTHER_PLAYER(current_player);
         print_status_player (current_player);
      }

   }
   return;
}

/***************************************************************************
** Unit Name:
**    newgame
**
** Description:
**    initializes the game
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to the new gameboard
**    score             initialize the score
**    player            initialize beginning player
**    x                 initialize cursor x location
**    y                 initialize cursor y location
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  newgame( struct BOARD *board, struct SCORE *score, INT1 *player, INT1 *x,INT1 *y)
{
   /* Initialize the game board */

   initboard(board);

   /*
   ** resets the cursor position to the upper left corner
   */

   *x = 0;
   *y = 0;

   /*
   ** Reset player to PLAYER1 (player1 always starts first)
   */

   *player = PLAYER1;

   /*
   ** get current score 
   ** should be BLACK: 2 and WHITE: 2 at the beginning of a game
   */

   *score = countscore(board);
   
   /*
   ** redraw status window with new information
   */
   print_status_player(*player);
   print_status_level (Skill_level);
   print_status_score (score);
}

/***************************************************************************
** Unit Name:
**    countscore
**
** Description:
**    Returns a structure containing the number of black
**    and number of white circles on the board.
**
**    Scans the game board by stepping through each location and checking
**    if it is black or white.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    board             pointer to the current game board
**
** Return Value:
**    score     structure containing the current score
**
** Notes:
**
***************************************************************************/

struct SCORE countscore(struct BOARD *board)
{
   INT1   x;
   INT1   y;
   struct SCORE score;

   score.player1 = 0;
   score.player2 = 0;

   /* examine each square in the board and count the */
   /* number of WHITE and BLACK squares */

   for(x=0;x<BOARD_X;x++)
   {
      for(y=0;y<BOARD_Y;y++)
      {
         if(board->loc[y][x]==PLAYER1)
            score.player1++;
         else if (board->loc[y][x]==PLAYER2)
            score.player2++;
      }
   }
   
   return (score);
}

/***************************************************************************
** Unit Name:
**    get_next_event
**
** Description:
**    Returns keyboard input if there was any else a NULL (0x00).
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    NONE
**
** Return Value:
**    key     key pressed
**
** Notes:
**
***************************************************************************/

UINT2 get_next_event(void)
{
   UINT2 key;

   if (inp_kbhit())
   {
      key = inp_getch();
   }else{
      key = 0x00;
   }
   return (key);
}

/***************************************************************************
** Unit Name:
**    print_status_score
**
** Description:
**    Prints the score in the status message box,
**    erases the previous score and displays the new score.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    score             a pointer to the current score structure to print
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void print_status_score(struct SCORE *score)
{
   FONT_COORD  f_pos;
   FONT_ATTR   f_attr;

#ifdef LCD
   char        player1_score_str[5];
   char        player2_score_str[5];
#else
   char score_str[40];
#endif

   /* Select a font */
   if (font_select(STATUS_WIN_FONT) < 0)
   {
      return;
   }

   /* get height attributes of font */
   font_get_attributes(&f_attr);


#ifdef LCD
   /* erase old score */
   gr_rect( STATUS_WIN_X + STATUS_WIN_SCORE_COLUMN,
            STATUS_WIN_Y + (STATUS_REGION_HEIGHT<<1),
            STATUS_WIN_WIDTH - STATUS_WIN_SCORE_COLUMN - WIN_BORDER_THICKNESS,
            (STATUS_REGION_HEIGHT<<1) - WIN_BORDER_THICKNESS,
            0, GR_NONE,
            GR_WHITE);

   /* build string to print */
   sprintf( player1_score_str,
            "%d",
            score->player1);

   sprintf( player2_score_str,
            "%d",
            score->player2);
   
   /* set position to display string */
   f_pos.x = STATUS_WIN_X + STATUS_WIN_SCORE_COLUMN,
   f_pos.y = STATUS_WIN_Y + (STATUS_REGION_HEIGHT * 3) - 3;
   font_set_pos(&f_pos);

   /* print score strings */
   font_printf(player1_score_str);

   /* set position to display string */
   f_pos.x = STATUS_WIN_X + STATUS_WIN_SCORE_COLUMN,
   f_pos.y = STATUS_WIN_Y + (STATUS_REGION_HEIGHT << 2) - 3;
   font_set_pos(&f_pos);

   /* print score strings */
   font_printf(player2_score_str);

#else

   /* set position to print score string */

   f_pos.x = STATUS_WIN_X + STATUS_WIN_TEXT_OFFSET + WIN_FRAME_WIDTH + STATUS_REGION1_WIDTH + STATUS_REGION2_WIDTH;
   f_pos.y = STATUS_WIN_Y + WIN_BORDER_THICKNESS + f_attr.height + (STATUS_WIN_TEXT_OFFSET<<1);
   
   font_set_pos(&f_pos);

   /* erase old score */
   
   gr_rect( f_pos.x + 50,
            f_pos.y - f_attr.height + 4,
            40,
            f_attr.height,0,GR_NONE,
            GR_WHITE);

   gr_rect( f_pos.x + 55 + 100,
            f_pos.y - f_attr.height + 4,
            40,
            f_attr.height,0,GR_NONE,
            GR_WHITE);

   /* build score string */
   sprintf( score_str,
            "\\t\\t\\t%d\\t\\t\\t\\t\\t%d",
            score->player1, score->player2);
  
   
   /*
   ** print score strings
   */
   font_printf(score_str);
 
#endif
}

/***************************************************************************
** Unit Name:
**    print_status_player
**
** Description:
**    Prints the current player in the status message box,
**    erases the old player and displays the new player.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    player            the current player
**
** Return Value:
**    NONE
**
** Notes:                
**
***************************************************************************/

void print_status_player(INT1 player)
{
   FONT_ATTR   f_attr;
   FONT_COORD  f_pos;
   char        color_str[15];
   char        black_str[] = "Black";
   char        white_str[] = "White";
   char        err_str[]   = "ERR%d";

   /* select font */
   if (font_select(STATUS_WIN_FONT) < 0)
   {
      return;
   }

   /* get font height attributes */
   font_get_attributes(&f_attr);


#ifdef LCD
   /* set up position to display player text at */
   f_pos.x = STATUS_WIN_TEXT_OFFSET + STATUS_WIN_X + STATUS_WIN_OUTPUT_COLUMN;
   f_pos.y = STATUS_WIN_Y + STATUS_REGION_HEIGHT - 3;

   /* erase old player */
   gr_rect( STATUS_WIN_X + STATUS_WIN_OUTPUT_COLUMN,
            STATUS_WIN_Y + WIN_BORDER_THICKNESS,
            STATUS_WIN_WIDTH - STATUS_WIN_OUTPUT_COLUMN - WIN_BORDER_THICKNESS,
            STATUS_REGION_HEIGHT - WIN_BORDER_THICKNESS,
            0, GR_NONE,
            GR_WHITE);
#else
   /* set up position to display player text at */
   f_pos.x = STATUS_WIN_X + WIN_FRAME_WIDTH +STATUS_WIN_TEXT_OFFSET;
   f_pos.y = STATUS_WIN_Y + WIN_BORDER_THICKNESS + f_attr.height + (STATUS_WIN_TEXT_OFFSET<<1);

   /* erase old player */
   gr_rect( f_pos.x,
            f_pos.y - f_attr.height + 4,
            STATUS_REGION1_WIDTH,
            STATUS_REGION_HEIGHT,0,GR_NONE,
            GR_WHITE);
#endif
   /* set up string to print */
   switch(player)
   {
      case PLAYER1:
         strcpy (color_str,black_str);
         break;
      case PLAYER2:
         strcpy (color_str,white_str);
         break;
      default:
         sprintf(color_str,err_str,player);
         break;
   }

   /* set position */
   font_set_pos(&f_pos);

   /* display the new player */
   font_printf(color_str);
}

/***************************************************************************
** Unit Name:
**    print_status_level
**
** Description:
**    Prints the current skill level in the status message box,
**    erases the old skill level and displays the new value.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    level             the current skill level
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void print_status_level(UINT1 level)
{
   FONT_COORD  f_pos;
   FONT_ATTR   f_attr;
   char        level_str[18];
   char        beginner_str[] = "Beginner";
   char        novice_str[]   = "Novice";
   char        expert_str[]   = "Expert";
   char        err_str[]      = "ERR-%d";


   /* select font */
   if (font_select(STATUS_WIN_FONT) < 0)
   {
      return;
   }

   /* get height attribute of font */
   font_get_attributes(&f_attr);

#ifdef LCD

   /* setup position to print level information at */
   
   f_pos.x = STATUS_WIN_TEXT_OFFSET + STATUS_WIN_X + STATUS_WIN_OUTPUT_COLUMN;
   f_pos.y = STATUS_WIN_Y + (STATUS_REGION_HEIGHT<<1) - 3;

   /* erase old level value */
   
   gr_rect( STATUS_WIN_X + STATUS_WIN_OUTPUT_COLUMN,
            STATUS_WIN_Y + STATUS_REGION_HEIGHT,
            STATUS_WIN_WIDTH - STATUS_WIN_OUTPUT_COLUMN - WIN_BORDER_THICKNESS,
            STATUS_REGION_HEIGHT,
            0, GR_NONE,
            GR_WHITE);
#else
   
   /* setup position to print level information at */

   f_pos.x = STATUS_WIN_X + WIN_FRAME_WIDTH + STATUS_WIN_TEXT_OFFSET + STATUS_REGION1_WIDTH;
   f_pos.y = STATUS_WIN_Y + WIN_BORDER_THICKNESS + f_attr.height + (STATUS_WIN_TEXT_OFFSET<<1);

   /* Clear(white out) region */
   /* erase old level value */

   gr_rect( f_pos.x,
            f_pos.y - f_attr.height + 4,
            STATUS_REGION2_WIDTH,
            f_attr.height,0,GR_NONE,
            GR_WHITE);

#endif
   /* build string to print */
   switch(level)
   {
      case LVL_BEGIN:
         strcpy (level_str,beginner_str);
         break;
      case LVL_NOVICE:
         strcpy (level_str,novice_str);
         break;
      case LVL_EXPERT:
         strcpy (level_str,expert_str);
         break;
      default:
         sprintf(level_str,err_str,level);
         break;
   }

   /* set position to print at */
   font_set_pos(&f_pos);

   /* print new level */
   font_printf (level_str);
}

/***************************************************************************
** Unit Name:
**    draw_status_win
**
** Description:
**    displays the status window with all of the text that does not change
**    such as Level, Score, Player, Black and White. The reset of the information
**    then has to be filled in by the functions: display_status_score(),
**    display_status_player(),and display_status_level().
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    NONE
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void draw_status_win(void)
{
   FONT_COORD  f_pos;
   FONT_ATTR   f_attr;

#ifdef LCD
   char        player_str[]      = "Player:";
   char        level_str[]       = "Level:";
   char        score_str[]       = "Score:";
   char        score_black_str[] = "Black:";
   char        score_white_str[] = "White:";
#else
   char        player_str[]      = "Player";
   char        level_str[]       = "Level";
   char        score_str[]       = "Score";
   char        score_color_str[] = "Black: \\t\\t\\tWhite:";
#endif

   /* select font */
   if (font_select(STATUS_WIN_FONT) < 0)
   {
      return;
   }

   /* get height attributes of font */
   font_get_attributes(&f_attr);

   /* display status window */
   display_window(STATUS_WIN_X,
                  STATUS_WIN_Y,
                  STATUS_WIN_WIDTH,
                  STATUS_WIN_HEIGHT,
                  GR_WHITE);

#ifdef LCD
   f_pos.x = STATUS_WIN_X + STATUS_WIN_TEXT_OFFSET;
   f_pos.y = STATUS_WIN_Y + (STATUS_REGION_HEIGHT) - 3;
   font_set_pos(&f_pos);
   font_printf(player_str);
   
   f_pos.y = STATUS_WIN_Y + (STATUS_REGION_HEIGHT<<1) - 3;
   font_set_pos(&f_pos);
   font_printf(level_str);
   
   f_pos.y = STATUS_WIN_Y + (STATUS_REGION_HEIGHT*3)  - 3;
   font_set_pos(&f_pos);
   font_printf(score_str);

   f_pos.x = STATUS_WIN_TEXT_OFFSET + STATUS_WIN_X + STATUS_WIN_OUTPUT_COLUMN;
   font_set_pos(&f_pos);
   font_printf(score_black_str);

   f_pos.x = STATUS_WIN_TEXT_OFFSET + STATUS_WIN_X + STATUS_WIN_OUTPUT_COLUMN;
   f_pos.y = STATUS_WIN_Y + (STATUS_REGION_HEIGHT<<2) - 3;
   font_set_pos(&f_pos);
   font_printf(score_white_str);
#else
   f_pos.x = STATUS_WIN_X + WIN_FRAME_WIDTH + STATUS_WIN_TEXT_OFFSET;
   f_pos.y = STATUS_WIN_Y + (STATUS_WIN_TEXT_OFFSET<<1);
   font_set_pos(&f_pos);
   font_printf(player_str);

   f_pos.x = STATUS_WIN_X + WIN_FRAME_WIDTH + STATUS_WIN_TEXT_OFFSET + STATUS_REGION1_WIDTH;
   font_set_pos(&f_pos);
   font_printf(level_str);
   
   f_pos.x = STATUS_WIN_X + WIN_FRAME_WIDTH + STATUS_WIN_TEXT_OFFSET + STATUS_REGION1_WIDTH + STATUS_REGION2_WIDTH;
   font_set_pos(&f_pos);
   font_printf(score_str);

   f_pos.x = STATUS_WIN_X + STATUS_WIN_TEXT_OFFSET + WIN_FRAME_WIDTH + STATUS_REGION1_WIDTH + STATUS_REGION2_WIDTH;
   f_pos.y = STATUS_WIN_Y + WIN_BORDER_THICKNESS + f_attr.height + (STATUS_WIN_TEXT_OFFSET<<1);
   font_set_pos(&f_pos);
   font_printf(score_color_str);
#endif
}

/***************************************************************************
** Unit Name:
**    display_status_message
**
** Description:
**    Prints a message in the status window erasing the information that
**    was in there. The string passed can be one or two lines separated
**    by a newline in the string. It will then print the string centered
**    in the window.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    message           character pointer to message to print
**
** Return Value:
**    NONE
**
** Notes:
**    Message can be either 1 line with no ending newline or
**    2 lines with one newline in the middle and again cannot
**    have an ending newline or it will not be centered
**
**    There is no error checking if message goes out of bounds of the window
**
***************************************************************************/

void  display_status_message(char *message)
{
   FONT_ATTR   f_attr;

   /* select font */
   if (font_select(STATUS_WIN_FONT) < 0)
   {
      return;
   }

   /* get height attribute of font */
   font_get_attributes(&f_attr);
   
   /* clear status window */
   gr_rect( STATUS_WIN_X + 1,
            STATUS_WIN_Y + 1,
            STATUS_WIN_WIDTH  - 2,
            STATUS_WIN_HEIGHT - 2,
            0,
            GR_NONE,
            GR_WHITE);

   /* print message centered in window */
   font_printf_centered(message,
                        STATUS_WIN_X,
                        STATUS_WIN_Y,
                        STATUS_WIN_X+STATUS_WIN_WIDTH,
                        STATUS_WIN_Y+STATUS_WIN_HEIGHT);
}

/***************************************************************************
** Unit Name:
**    option_menu
**
** Description:
**    Handles the options menu events.
**    Displays the options menu and waits for keyboard input.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    gameboard         The current game board
**    x                 Current location of the cursor on the x axis
**    y                 Current location of the cursor on the y axis
**    player            Current player
**    marker            TRUE if current location of cursor is a legal move
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void  option_menu(struct BOARD *gameboard,INT1 *x, INT1 *y,INT1 *player,BOOL *marker)
{
   UINT2          menu_event;
   struct SCORE   score;
   BOOL           exit_opt_menu = FALSE;
   char           demo_message[] = "Press any\\nkey to\\nstop demo . . .";

   /* display option menu */
   display_menu(Optmenu);

   while(exit_opt_menu == FALSE)
   {
      menu_event = get_next_event();

      switch(menu_event)
      {
         /*
         ** One player option - One human player and one computer player
         */
         case ONE_OPT:
            /* set number of players to one */
            Num_players = 1;
            Options_changed = TRUE;
            break;

         /*
         ** Two player option - Two human players
         */
         case TWO_OPT:
            
            /* set number of players to one */
            Num_players = 2;
            Options_changed = TRUE;
            break;

         /*
         ** Demo option - computer plays itself
         */
         case DEMO_OPT:
            if (*marker == TRUE)
               erasemarker(*x,*y);

            /* display the "Press any key to stop demo..." message in */
            /* menu window */
            display_menu(demo_message);
            draw_status_win ();

            /* erase the cursor - computer player doesn't need a cursor */
            erase_cursor(*x,*y);
            newgame(gameboard,&score,player,x,y);
            drawboard(  BRD_START_X,      /* draw the game board */
                        BRD_START_Y);
            drawtokens(gameboard);        /* draw the starting tokens (circles) */

            /* on PWP SELECT_KEY returns a key press */
            /* and would stop demo, so we need to capture it */
            /* Demo won't work until some key has been pressed, but there */
            /* isn't much I can do */
#ifdef PWP
            inp_getch();  /* get select key release */
#endif
            /* LOOP through demo until a keypress is made */
            while(!inp_kbhit())
            {  
               /* Put up thinking window */
               if (Skill_level > LVL_BEGIN)
               {
                  display_thinking();
               }

               /* Find next move for the computer */
               if (findmove(gameboard,x,y,*player,Skill_level))
               {
                  /* if a move is found make it */
                  makemove(gameboard,*x,*y,*player);

                  /* redraw the board with the changed circles */
                  drawtokens(gameboard);
               }

               /* if there was no move available for the first player */
               /* check for move available for the other player */
               
               else if (!moveavailable(gameboard,OTHER_PLAYER(*player)))
               {
                  /* if no move available for either player */
                  /* GAME is OVER so erase thinking window and wait a few */
                  /* seconds for the viewer to see who won and the status */
                  /* then start a new game */
                  if (Skill_level > LVL_BEGIN)
                  {
                     /*
                     ** erase thinking window
                     */
                     if (Skill_level > LVL_BEGIN)
                     {
                        erase_thinking();
                     }
                  }
                  waitsecs(DEMO_DELAY);
                  newgame(gameboard,&score,player,x,y);
                  drawboard(  BRD_START_X,      /* draw the game board */
                              BRD_START_Y);
                  drawtokens(gameboard);        /* draw the starting tokens (circles) */

               }
            
               /*
               ** erase thinking window
               */
               if (Skill_level > LVL_BEGIN)
               {
                  erase_thinking();
               }

               *player = OTHER_PLAYER(*player);
               print_status_player (*player);
            
               score = countscore(gameboard);
               print_status_score (&score);
            }
            
            /*
            ** DEMO HAS ENDED BY KEY PRESS FROM USER
            */

            /* eat keyboard hit that ended demo */
            inp_getch();
            
#if 0
            /* start a new game */
            newgame(gameboard,&score,player,x,y);
            
            /* draw the new game board */
            drawboard(  BRD_START_X,      /* draw the game board */
                        BRD_START_Y);

            /* draw the circles that start the game */
            drawtokens(gameboard);        /* draw the starting tokens (circles) */
            
            /* redraw the cursor */
            draw_cursor(*x,*y);
#endif

            /* exit the option menu */
            exit_opt_menu = TRUE;

            Options_changed = TRUE;
            No_move_left = FALSE;
            
            break;

         /*
         ** Beginner option - skill level
         */
         case BEGIN_OPT:
            /* set the skill level to beginner */
            Skill_level = LVL_BEGIN;
            Options_changed = TRUE;

            /* if game is not over and the user is changing in the middle */
            /* of a game just redraw the new level */

            if (!No_move_left)
            {
               print_status_level (Skill_level);
            }
            else
            {
               /* the user is changing the level after finishing a game */
               /* so we need to redraw the status window with the new */
               /* information */
               
               score = countscore(gameboard);
               draw_status_win ();
               print_status_player (*player);
               print_status_level (Skill_level);
               print_status_score (&score);
               No_move_left = FALSE;
            }
            break;

         /*
         ** Novice option - skill level
         */
         case NOVICE_OPT:
            /* set the skill level to novice */
            Options_changed = TRUE;
            Skill_level = LVL_NOVICE;

            /* if game is not over and the user is changing in the middle */
            /* of a game just redraw the new level */

            if (!No_move_left)
            {
               print_status_level (Skill_level);
            }
            else
            {
               /* the user is changing the level after finishing a game */
               /* so we need to redraw the status window with the new */
               /* information */
               
               draw_status_win ();
               print_status_player (*player);
               print_status_level (Skill_level);
               score = countscore(gameboard);
               print_status_score (&score);
               No_move_left = FALSE;
            }
            break;

         /*
         ** Expert option - skill level
         */
         case EXPERT_OPT:
            /* set the skill level to novice */
            Skill_level = LVL_EXPERT;
            Options_changed = TRUE;
            
            /* if game is not over and the user is changing in the middle */
            /* of a game just redraw the new level */

            if (!No_move_left)
            {
               print_status_level (Skill_level);
            }
            else
            {
               /* the user is changing the level after finishing a game */
               /* so we need to redraw the status window with the new */
               /* information */
               
               draw_status_win ();
               print_status_player (*player);
               print_status_level (Skill_level);
               score = countscore(gameboard);
               print_status_score (&score);
               No_move_left = FALSE;
            }
            break;

         /*
         ** Exit options menu
         */
         case EXIT_OPT:
         case RETURN_KEY:
            exit_opt_menu = TRUE;
            break;
      }
   }

   /* need to redraw main menu now */
   display_menu(Mainmenu);
}

/***************************************************************************
** Unit Name:
**    game_over_message
**
** Description:
**    Prints "GAME OVER" message in status window along with how much
**    you won or lost by.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    score             pointer to the structure with the final score
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void game_over_message(struct SCORE *score)
{ 
   char        game_over_str[30];

   if ((score->player1) > (score->player2))
   {
      if (Num_players == 1)
      {
         sprintf (game_over_str,
                  "GAME OVER\\nYou won by %d",
                  score->player1 - score->player2);
      }
      else
      {
         sprintf (game_over_str,
                  "GAME OVER\\nBLACK won by %d",
                  score->player1 - score->player2);
      }
   }
   else if ((score->player1) < (score->player2))
   {
      if (Num_players == 1)
      {
         sprintf (game_over_str,
                  "GAME OVER\\nYou lost by %d",
                  score->player2 - score->player1);
      }
      else
      {
         sprintf (game_over_str,
                  "GAME OVER\\nWHITE won by %d",
                  score->player2 - score->player1);
      }
   }
   else
   {
      sprintf (game_over_str,
               "GAME OVER\\nIt's a tie!");
   }

   display_status_message(game_over_str);
}

/***************************************************************************
** Unit Name:
**    display_thinking
**
** Description:
**    Displays a window with the message "Thinking..." in it.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    NONE
**
** Return Value:
**    NONE
**
** Notes:
**    It is up to you to erase the thinking window
**
***************************************************************************/

void display_thinking(void)
{
   if (font_select(THINK_FONT) < 0)
   {
      return;
   }

   display_window(   THINK_WIN_X,
                     THINK_WIN_Y,
                     THINK_WIN_WIDTH,
                     THINK_WIN_HEIGHT,
                     GR_WHITE);

   font_printf_centered( "Thinking . . .",
                     THINK_WIN_X,
                     THINK_WIN_Y,
                     THINK_WIN_X+THINK_WIN_WIDTH,
                     THINK_WIN_Y+THINK_WIN_HEIGHT - 3);
}

/***************************************************************************
** Unit Name:
**    erase_thinking
**
** Description:
**    Removes the thinking message window by drawing over the window
**    with a gray rectangular region.
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    NONE
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/

void erase_thinking(void)
{
   gr_rect( THINK_WIN_X - (WIN_BORDER_THICKNESS<<1) - WIN_FRAME_WIDTH,
            THINK_WIN_Y - (WIN_BORDER_THICKNESS<<1) - WIN_FRAME_WIDTH,
            THINK_WIN_WIDTH  + (WIN_BORDER_THICKNESS<<2) + (WIN_FRAME_WIDTH<<1),
            THINK_WIN_HEIGHT + (WIN_BORDER_THICKNESS<<2) + (WIN_FRAME_WIDTH<<1),
            0,GR_NONE,
            BACKGROUND);
}

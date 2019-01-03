/*****************************************************************************
**
**  Module Name:
**      Spin-A-Phrase
**  
**  Description:
**      This module contains the bulk of the Spin-A-Phrase program.
**
**  Exported Units:
**
**  Local Units:
**      app_main (used instead of main for PWPs)
**      process_player_turn
**      process_computer_turn
**      process_stray_input
**      display_timer
**      display_main_menu
**      display_options_menu
**      display_status_menu
**      display_game
**      prompt_player_info
**      initialize_file
**      is_file_open
**      open_file
**      read_file_header
**      get_new_puzzle
**      cleanup_file
**      put_up_puzzle
**      get_next_token
**      draw_tile
**      place_puzzle_tile
**      next_player
**      player_notify
**      letter_in_puzzle
**      display_topic
**      update_available_letters
**      display_available_letters
**      remove_letter_from_string
**      letter_available
**      only_vowels_left
**      spin
**      process_chosen_letter
**      choose_letter
**      buy_vowel
**      solve_puzzle
**      timer
**      game_over_processing
**      match_over_processing
**      getch_timeout
**      
**  Exported Data:
**
**  Notes:
**      none
**
**  Change Log:
**      Date            Description of Change
**      ----            ---------------------
**      05/20/92        Version 1.0 
**
**
*****************************************************************************/


/*
** -------------------- Include Files -----------------------------
*/


#include <stdio.h>
#include <string.h>
#include "types.h"    /* Data type definitions */
#include "sc_def.h"   /* Data type definitions */
#include "inp.h"      /* Keyboard access */
#include "gr.h"       /* Graphics routines */
#include "keys.h"     /* Key definitions */
#include "window.h"   /* Pop up windows */
#include "fontlib.h"  /* Font routines */
#include "fileio.h"   /* File access routines */
#include "helplib.h"  /* Help routines */
#include "splash.h"   /* Splash screen */
#include "phrase.h"   /* Constants for data sizes */

#ifdef LCD
   #include "phr_lcd.hlp"   /* LCD help text */
#else
   #include "phr_crt.hlp"   /* CRT help text */
#endif

   #include "varstab.j"     /* Misc. PWP functions */


/*
** --------------------- Constants -----------------------------
*/


/*
** To make the game run superfast (so you can leave some
** computer players going at it overnight) and without
** stopping for input, define this...
*/
#define BATCH_MODE              0

/*
** To be able to cheat and solve the puzzle at will, define this...
*/
#define DEBUG                   0

/*
** To get all of the phrases in the order they occur in the file,
** define this...
*/
#define INORDER                 0

/*
** On the CRT there is less space at the top of the screen because of
** the title.  On the LCD there is less space on the sides of the screen
** because of the same title.  Take this into account in the dimensions
** of the usable area.
*/
#ifdef LCD
   #define NEW_MIN_X            (MIN_X + LEFT_ADJUSTMENT)   /* From window.h */
   #define NEW_MAX_X            (MAX_X - RIGHT_ADJUSTMENT)  /* From window.h */
   #define NEW_MIN_Y            MIN_Y
   #define NEW_MAX_Y            MAX_Y
#else
   #define NEW_MIN_X            MIN_X
   #define NEW_MAX_X            MAX_X
   #define NEW_MIN_Y            (MIN_Y + TITLE_ADJUSTMENT)  /* From window.h */
   #define NEW_MAX_Y            MAX_Y
#endif

#define GAMES_PER_MATCH         5

/*
** TICKS always returns the current time.
*/
   #define TICKS                (*(unsigned long *) vtab[VT_ticks])
   #define CLOCKS_PER_SEC       63

/*
** The name of the game.  APP_NAME is passed to the splash screen routine.
** TITLE_NAME is passed to the title routine.  On the LCD the title is vertical.
*/
#define APP_NAME                "Spin - A - Phrase"
#ifdef LCD
   #define TITLE_NAME           "SPIN\\n-A-\\nP\\nH\\nR\\nA\\nS\\nE"
#else
   #define TITLE_NAME           APP_NAME
#endif

#define ANY_NUMBER              ((INT2) (TICKS & 0x0000FFFF))

/*
** Timer actions.
*/
#define TIMER_START             0
#define TIMER_STOP              1
#define TIMER_RESET             2
#define TIMER_REPORT            3
#define TIMER_MAX               0xFFFFFFFF
#define TICKS_SEC               (TICKS / CLOCKS_PER_SEC)

/*
** How many pixels between each item
** on the screen be very careful when
** you change this.  Make sure that 
** none of the puzzles overwrite the
** status window (some are 4 lines). 
*/
#ifdef LCD
   #define ITEM_SPACE           3
#else
   #define ITEM_SPACE           10
#endif

/*
** There is not enough room on the LCD to display bevels on the
** boxes.  Use plain rectangles instead.
*/
#ifdef LCD
   #define BORDER_WIDTH         0
   #define FRAME_WIDTH          0
   #define BEVEL_WIDTH          0
#else
   #define BORDER_WIDTH         3
   #define FRAME_WIDTH          5
   #define BEVEL_WIDTH          (BORDER_WIDTH + FRAME_WIDTH + BORDER_WIDTH)
#endif

/*
** Define some macros for testing what a character is.
*/
#define IS_VOWEL(l)             (strchr (Vowels,     l) != NULL)
#define IS_CONSONANT(l)         (strchr (Consonants, l) != NULL)
#define IS_LETTER(l)            (strchr (Alphabet,   l) != NULL)

/*
** Split the alphabet up into vowels and consonants.
*/
#define VOWELS                  "AEIOU"
#define CONSONANTS              "BCDFGHJKLMNPQRSTVWXYZ"

/*
** Split the alphabet up into vowels and consonants.
** Sort them by the frequency with which they occur in normal
** text.  The computer sometimes tries to be smart by guessing
** the most frequent letters first.
*/
#define VOWELS_FRQ              "EAOIU"
#define CONSONANTS_FRQ          "TSRNHCLDPMFYGBWKVXQZJ"

/*
** The location of the lists of letters that have not been chosen.
*/
#define UNUSED_CON_AREA_X       (ITEM_SPACE + NEW_MIN_X)
#define UNUSED_CON_AREA_Y       (NEW_MAX_Y + 1 - ITEM_SPACE - TILE_SIZE)

#ifdef LCD
   #define UNUSED_VWL_AREA_X    (UNUSED_CON_AREA_X + (sizeof (CONSONANTS) - \
                                 sizeof (VOWELS)) * (TILE_SIZE + TILE_SPACING))
#else
   #define UNUSED_VWL_AREA_X    UNUSED_CON_AREA_X
#endif
#define UNUSED_VWL_AREA_Y       (UNUSED_CON_AREA_Y - TILE_SPACING - TILE_SIZE)

/*
** The box that the "notify and wait" messages are centered in.
** The font to print these messages in.
*/
#define NOTIFY_WAIT_MIN_X       NEW_MIN_X
#define NOTIFY_WAIT_MAX_X       NEW_MAX_X
#define NOTIFY_WAIT_MIN_Y       NEW_MIN_Y
#define NOTIFY_WAIT_MAX_Y       NEW_MAX_Y
#ifdef LCD
   #define NOTIFY_WAIT_FONT     "Helvetica 12"
#else
   #define NOTIFY_WAIT_FONT     "Helvetica 24"
#endif

/*
** The box that the "notify and do NOT wait" messages are centered in.
** The font to print these messages in.
*/
#define NOTIFY_NOWAIT_MIN_X     NEW_MIN_X
#define NOTIFY_NOWAIT_MAX_X     NEW_MAX_X
#define NOTIFY_NOWAIT_MIN_Y     NEW_MIN_Y
#define NOTIFY_NOWAIT_MAX_Y     NEW_MAX_Y
#ifdef LCD
   #define NOTIFY_NOWAIT_FONT   "Helvetica 12"
#else
   #define NOTIFY_NOWAIT_FONT   "Helvetica 24"
#endif
#define NOTIFY_NOWAIT_TIMEOUT   3  /* seconds */
#define NOTIFY_NOWAIT_BATCH_TIMEOUT   1  /* seconds */

/*
** Location and size of the main menu.
*/
#ifdef LCD
   #define MENU_FONT            "Helvetica 12"
   #define MENU_WIDTH           (NEW_MAX_X + 1 - NEW_MIN_X - ITEM_SPACE - \
                                 ((sizeof (CONSONANTS) - 1) * \
                                 (TILE_SIZE + TILE_SPACING)) + \
                                 TILE_SPACING - ITEM_SPACE - ITEM_SPACE)
   #define MENU_HEIGHT          100
   #define MENU_Y               (NEW_MAX_Y + 1 - ITEM_SPACE - MENU_HEIGHT)
   #define MENU_LEADING         0
#else
   #define MENU_FONT            "Helvetica 30"
   #define MENU_WIDTH           160
   #define MENU_HEIGHT          290
   #define MENU_Y               (UNUSED_VWL_AREA_Y - ITEM_SPACE - MENU_HEIGHT)
   #define MENU_LEADING         3
#endif
#define MENU_X                  (NEW_MAX_X + 1 - ITEM_SPACE - MENU_WIDTH)

/*
** The text to display in the main menu.
*/
#ifdef LCD
   #define MAIN_MENU_TEXT       "\\uN\\uew\\n\\uO\\uptions\\n\\uH\\uelp\\n\\uQ\\uuit\\n\\uS\\upin\\n\\uV\\uowel\\n\\uG\\uuess"
#else
   #define MAIN_MENU_TEXT       "\\uN\\uew\\n\\uO\\uptions\\n\\uH\\uelp\\n\\uQ\\uuit\\n\\n\\uS\\upin\\n\\uV\\uowel\\n\\uG\\uuess"
#endif

#define MENU_TEMPLATE     "\\uC\\ulock\\n%s\\n\\uT\\uurn Time\\n%d\\n\\uS\\uolve Time\\n%d\\n\\uL\\uevel\\n"

/*
** The main menu and the options menu are in the same place,
** but they have a different leading.
*/
#ifdef LCD
   #define OPTIONS_LEADING      -1
   #define OPTIONS_OFFSET       3
#else
   #define OPTIONS_LEADING      4
   #define OPTIONS_OFFSET       4
#endif

/*
** The number of pieces the options menu is split into.
*/
#define OPTIONS_V_WINDOWS       8

/*
** Text to print in the options menu.
*/
#define TIMER_TEXT              "\\uC\\ulock"
#define ON_TEXT                 "On"
#define OFF_TEXT                "Off"
#define TURN_LIMIT_TEXT         "\\uT\\uurn Time"
#define SOLVE_LIMIT_TEXT        "\\uS\\uolve Time"
#define SKILL_LEVEL_TEXT        "\\uL\\uevel"
#define BEGINNER_TEXT           "\\uB\\ueginner"
#define NOVICE_TEXT             "\\uN\\uovice"
#define EXPERT_TEXT             "\\uE\\uxpert"

/*
** The location of the timer window.  The font to print in.
*/
#define TIMER_X                 MENU_X
#define TIMER_Y                 (NEW_MIN_Y + ITEM_SPACE)
#define TIMER_WIDTH             MENU_WIDTH
#define TIMER_HEIGHT            (MENU_Y - TIMER_Y - ITEM_SPACE)
#ifdef LCD
   #define TIMER_FONT           "Helvetica 12"
   #define TIMER_TXT            "Time:"
#else
   #define TIMER_FONT           "Helvetica 24"
   #define TIMER_TXT            "Time left:"
#endif

/*
** The location of the timer subwindow to print the timer
** identification string in.
*/
#define TIMER1_X                (TIMER_X + BEVEL_WIDTH)
#define TIMER1_Y                (TIMER_Y + BEVEL_WIDTH)
#define TIMER1_WIDTH            ((TIMER_WIDTH - BEVEL_WIDTH - BEVEL_WIDTH) * \
                                 2 / 3)
#define TIMER1_HEIGHT           (TIMER_HEIGHT - BEVEL_WIDTH - BEVEL_WIDTH)

/*
** The location of the timer subwindow to print the time in.
*/
#define TIMER2_X                (TIMER1_X + TIMER1_WIDTH)
#define TIMER2_Y                TIMER1_Y
#define TIMER2_WIDTH            (TIMER_WIDTH - BEVEL_WIDTH - BEVEL_WIDTH - \
                                 TIMER1_WIDTH)
#define TIMER2_HEIGHT           TIMER1_HEIGHT

/*
** The size of the tiles for the phrase and unused letters.
*/
#ifdef LCD
   #define TILE_FONT            "Helvetica 12"
   #define TILE_SIZE            14  /* units = pixels */
   #define TILE_SPACING         2   /* units = pixels */
#else
   #define TILE_FONT            "Helvetica 24"
   #define TILE_SIZE            25  /* units = pixels */
   #define TILE_SPACING         3   /* units = pixels */
#endif

#define TILE_AREA_X             TOPIC_AREA_OUT_X
#define TILE_AREA_Y             (TOPIC_AREA_OUT_Y + TOPIC_AREA_HEIGHT + \
                                 TILE_SPACING)

/*
** The font to print the topic in.
*/
#ifdef LCD
   #define TOPIC_FONT           "Helvetica 12"
#else
   #define TOPIC_FONT           "Helvetica 20"
#endif

/*
** Where the topic is located and how big it is.
*/
#ifdef LCD
   #define TOPIC_AREA_OUT_X     (STATUS_X + STATUS_WIDTH + ITEM_SPACE)
#else
   #define TOPIC_AREA_OUT_X     (NEW_MIN_X + ITEM_SPACE)
#endif
#define TOPIC_AREA_OUT_Y        (NEW_MIN_Y + ITEM_SPACE)
#define TOPIC_BORDER            2   /* units = pixels */
#define TOPIC_IN                3   /* units = pixels */
#define TOPIC_AREA_IN_X         (TOPIC_AREA_OUT_X + TOPIC_BORDER + TOPIC_IN)
#define TOPIC_AREA_HEIGHT       TILE_SIZE

/*
** When the user starts a new game he gets prompted to enter his name.
** This is where that prompt goes. 
*/
#define NAME_AREA_WIDTH         MAX_NAME
#define NAME_AREA_HEIGHT        1
#define NAME_AREA_X             (((NEW_MAX_X - NEW_MIN_X - NAME_AREA_WIDTH * \
                                 (TILE_SIZE + TILE_SPACING)) >> 1) + NEW_MIN_X)
#define NAME_AREA_Y             (((NEW_MAX_Y - NEW_MIN_Y - (NAME_AREA_HEIGHT * \
                                 TILE_SIZE + (TILE_SIZE + TILE_SPACING) * \
                                 3)) >> 1) + NEW_MIN_Y)

/*
** Definitions as to the size, shape of the wheel.
*/
#ifdef LCD
   #define WHEEL_PIX_WIDTH      40
   #define WHEEL_PIX_HEIGHT     10
   #define NUMBER_SLOTS_SEEN    5
   #define WHEEL_WHITE_SPACE    38
   #define ARROW_HEIGHT         20
   #define ARROW_WIDTH          ARROW_HEIGHT
   #define WHEEL_START_X        (STATUS_X +1)
   #define WHEEL_START_Y        (STATUS_Y + (STATUS_HEIGHT - \
                                 (WHEEL_BOX_HEIGHT -2) >>1))
   #define WHEEL_PULL_DOWN_DELAY 2  /* Units = secs */
#else
   #define WHEEL_PIX_WIDTH      72
   #define WHEEL_PIX_HEIGHT     16
   #define NUMBER_SLOTS_SEEN    6
   #define WHEEL_WHITE_SPACE    40
   #define ARROW_HEIGHT         32
   #define ARROW_WIDTH          ARROW_HEIGHT
   #define WHEEL_START_X        (NEW_MIN_X + ((NEW_MAX_X - NEW_MIN_X - \
                                 WHEEL_BOX_WIDTH) >> 1))
   #define WHEEL_START_Y        (UNUSED_VWL_AREA_Y - ITEM_SPACE - \
                                 WHEEL_BOX_HEIGHT)
#endif

/*
** Timer information to control the speed of the spinning wheel.
*/
#define NUM_TICKS_PER_SEC    62  /* Be very careful if you change this for the
                                    PWP. It's timer can't be finer.  */
#define ONE_SECOND              CLOCKS_PER_SEC

/*
** How many slots there are on the wheel for each of the levels.
*/
#define NUMBER_SLOTS_BEGIN      16
#define NUMBER_SLOTS_NOVICE     (NUMBER_SLOTS_BEGIN + 6)
#define NUMBER_SLOTS_EXPERT     (NUMBER_SLOTS_NOVICE + 6)
#define WHEEL_BOX_HEIGHT        ((WHEEL_PIX_HEIGHT << 1) * NUMBER_SLOTS_SEEN\
                                 + (BORDER_WIDTH << 2) + (FRAME_WIDTH << 1) +2)
#define WHEEL_BOX_WIDTH         (WHEEL_PIX_WIDTH + (BORDER_WIDTH << 2) + \
                                 (FRAME_WIDTH << 1) + WHEEL_WHITE_SPACE)

/*
** Where the status menu goes and how big it is.  What font it is to
** be printed in and what the leading adjustment is from the default.
*/
#ifdef LCD
   #define STATUS_FONT          "Helvetica 12"
   #define STATUS_WIDTH         ((TILE_SIZE + TILE_SPACING) * 5 - TILE_SPACING)
   #define STATUS_HEIGHT        (UNUSED_CON_AREA_Y - NEW_MIN_Y - \
                                 (ITEM_SPACE << 1))
   #define STATUS_X             (ITEM_SPACE + NEW_MIN_X)
   #define STATUS_Y             (ITEM_SPACE + NEW_MIN_Y)
   #define STATUS_LEADING       -3
   #define LINES_PER_PLAYER     3
   #define LCD_TAB_WIDTH        4
#else
   #define STATUS_FONT          "Helvetica 20"
   #define STATUS_WIDTH         MENU_WIDTH + 30
   #define STATUS_HEIGHT        WHEEL_BOX_HEIGHT  /* Hits phrase if too high */
   #define STATUS_X             (ITEM_SPACE + NEW_MIN_X)
   #define STATUS_Y             WHEEL_START_Y
   #define STATUS_LEADING       0
   #define LINES_PER_PLAYER     3
#endif

/*
** Strings that are printed in the status window.
*/
#define GAME_TXT                "  Game\\t$"
#ifdef LCD
   #define MATCH_TXT            "  Match\\t\\t$"
#else
   #define MATCH_TXT            "  Match\\t$"
#endif
#define PLAYER_TXT              " %s\\n" GAME_TXT "\\n" MATCH_TXT

/*
** Default time limits and ranges that the limits can vary within.
*/
#define MIN_TURN_LIMIT           3  /* seconds */
#define TAKE_TURN_LIMIT         10  /* seconds */
#define MAX_TURN_LIMIT          20  /* seconds */
#define MIN_SOLVE_LIMIT         20  /* seconds */
#define SOLVE_PUZZLE_LIMIT      30  /* seconds */
#define MAX_SOLVE_LIMIT         60  /* seconds */

/*
** Messages that come up if you time out at various places.
*/
#define TAKE_TURN_MSG           "%s, you did not\\ntake your turn in time!"
#define SOLVE_PUZZLE_MSG        "%s, you did not\\nsolve the puzzle in time!"
#define CHOOSE_LETTER_MSG       "%s, you did not\\nchoose a letter in time!"
#define BUY_VOWEL_MSG           "%s, you did not\\nbuy a vowel in time!"

/*
** General game messages
*/
#define PRESS_A_KEY_TXT         "\\nPress a key"
#define YES_OR_NO_TXT           "\\n\\uY\\ues or \\uN\\uo"
#define ENTER_NAME_TXT          "Please enter player #%d's name\\nand press RETURN"
#define IS_COMPUTER_TXT         "Do you want the computer\\nto play for %s?" YES_OR_NO_TXT
#define MORE_PLAYERS_TXT        "Any more players?" YES_OR_NO_TXT
#define NAME_HAVE_LENGTH_TXT    "You must enter a name"
#define QUIT_TXT                "Are you sure\\nyou want to quit?" YES_OR_NO_TXT
#define DISK_ERROR_TXT          "Could not access the disk.\\nCheck the drive and" PRESS_A_KEY_TXT
#define NEW_PLAYERS_TXT         "Choose new players?" YES_OR_NO_TXT

/*
** Messages about/to human players
*/
#define H_WIN_TXT               "%s, you have just won with $%d!"
#define H_CHOOSE_CONSONANT_TXT  "For $%d each,\\ntype a consonant, %s"
#define H_CHOOSE_VOWEL_TXT      "Type a vowel, %s"
#define H_USE_FREE_SPIN_TXT     "%s, would you like to use a\\nfree spin?  You have %d left." YES_OR_NO_TXT
#define H_SOLVE_START_TXT       "Press RETURN, then type the missing\\nletters of the phrase, %s"
#define H_SOLVE_FAIL_TXT        "Your guess was not correct, %s"
#define H_TURN_NOTIFY_TXT       "It is your turn, %s"
#define H_CANNOT_SPIN_TXT       "%s, you can not spin.  There are\\nno consonants left!"
#define H_NO_VOWELS_TXT         "%s, you can not buy a vowel.\\nThere are no vowels left!"
#define H_ALREADY_CHOSEN_TXT    "'%c' has already been chosen, %s!"
#define H_NOT_CONSONANT_TXT     "%s, '%c' is not a consonant!"
#define H_NOT_VOWEL_TXT         "%s, '%c' is not a vowel!"
#define H_CANNOT_BUY_VOWEL_TXT  "%s, you do not have enough\\nmoney to buy a vowel!\\nIt is $%d to buy a vowel"
#define H_FREE_SPIN_TXT         "%s, you just got a free spin!"
#define H_BROKE_TXT             "You are bankrupt, %s!"
#define H_LOSE_TURN_TXT         "Your turn is over, %s!"
#define H_NONE_IN_PUZZLE_TXT    "%s, there is no '%c' in the puzzle!"
#define H_ONE_IN_PUZZLE_TXT     "There is one '%c', %s!"
#define H_MANY_IN_PUZZLE_TXT    "%s, there are %d '%c's!"
#define H_GRAND_PRIZE_TXT       "%s, you have broken $%ld\\nand have won the match!" PRESS_A_KEY_TXT
#define H_MATCH_WINNER_TXT      "Congratulations %s,\\nyou have just won\\n$%d!" PRESS_A_KEY_TXT

/*
** Messages about computer players
*/
#define C_WIN_TXT               "%s has just won with $%d!\\nThe PWP won!"
#define C_USING_FREE_SPIN_TXT   "%s is using a free spin"
#define C_SOLVE_ATTEMPT_TXT     "%s is trying to guess the puzzle"
#define C_TIME_OUT_TXT          "%s ran out of time"
#define C_TURN_NOTIFY_TXT       "%s is taking a turn"
#define C_BUYING_VOWEL          "%s is buying a vowel"
#define C_FREE_SPIN_TXT         "%s just got a free spin!"
#define C_SOLVE_FAIL_TXT        "%s tried to guess the puzzle and failed"
#define C_BROKE_TXT             "%s has just gone bankrupt!"
#define C_LOSE_TURN_TXT         "%s's turn is over!"
#define C_NONE_IN_PUZZLE_TXT    "%s guessed '%c', but there\\nare none in the puzzle"
#define C_ONE_IN_PUZZLE_TXT     "%s guessed '%c', and there\\nis one in the puzzle"
#define C_MANY_IN_PUZZLE_TXT    "%s guessed '%c', and there\\nare %d in the puzzle!"
#define C_ALREADY_CHOSEN_TXT    "%s guessed '%c', but that\\nhad already been guessed!"
#define C_GRAND_PRIZE_TXT       "%s just broke $%ld\\nand won the match!" PRESS_A_KEY_TXT
#define C_MATCH_WINNER_TXT      "%s has just won\\n$%d!" PRESS_A_KEY_TXT

/*
** The computer players are all percentage/random driven by changing these
** the computer players can be made smarter/dumber.
*/
#define ODDS                    (rand () % 100)
#define ODDS_GUESS_RIGHT        93   /* Even computers can make mistakes! */
#define ODDS_BUY_VOWEL          27   /* It takes money to make money */
#define MIN_PERCENT_TO_SOLVE    75   /* Decrease to make computer 'smarter' */
#define BASE_CHANCE_SOLVE        8   /* Keep this number low */
#define ODDS_USE_FREE_SPIN      80   /* Just to make things more random */
#define MIN_PERCENT_TO_USE_FS   20   /* Better to save them for a while */
#define ODDS_BREAK_DEADLOCK     20   /* See comment later */
#define ODDS_GUESS_FREQUENCY    45   /* Sometimes the computer is smart */
#define COMPUTER_MONEY_WEIGHT   1000 /* More money => faster solve */

#define VOWEL_COST              100  /* Dollar cost of a vowel */


/*
** ----------------------- Types -----------------------------
*/


/*
** File information.
*/
typedef struct file_info {
   FILE_HDL handle;
   INT4     relation_size;
   INT4     index_count;
   CHAR     *relation_table;
   UINT2    *index_table;
} FILE_INFO;

/*
** The wheel can be either displayed or spun.  Menus can
** be displayed in their entirety or just updated.
*/
typedef enum actions {SPIN, DISPLAY, UPDATE} ACTIONS;

/*
** When the computer takes a turn it can do any one of these.
*/
typedef enum do_choices {DO_SPIN, DO_BUY_VOWEL, DO_TIME_OUT, 
                         DO_SOLVE} DO_CHOICES;
/*
** Different results that functions can return about the results of
** a turn.
*/
typedef enum state_type {ONE = 1, TWO, THREE, FOUR, FIVE, SIX, 
                         SEVEN, EIGHT, NINE, TEN, FIFTEEN = 15, 
                         GOOD_MOVE, LOSE_TURN, BROKE, INVALID_CHOICE, 
                         FREE_SPIN, SOLVE, TIME_OUT} STATE_TYPE;
#include "pixmaps.h"

/*
** The available skill levels.
*/
typedef enum skill_type {BEGINNER, NOVICE, EXPERT} SKILL_TYPE;

/*
** A player in the game and some statistics about the player.
*/
typedef struct player_type {
   char     name[MAX_NAME + 1]; /* The players first name */
   INT4     game_dollars;       /* The player's score in the current game */
   INT4     match_dollars;      /* The player's total score */
   UINT1    free_spin;          /* The number of free spins this player has */
   BOOL     computer_player;    /* Is this a computer player? */
} PLAYER_TYPE;

/*
** A character in the current puzzle and a flag telling whether or not the
** letter has been revealed. 
*/
typedef struct puzzle_info {
   char  text[MAX_PUZZLE];
   UINT1 hidden[MAX_PUZZLE]; 
   char  topic[MAX_TOPIC];
   char  consonants[sizeof (CONSONANTS)];
   char  vowels[sizeof (VOWELS)];
   char  consonants_frq[sizeof (CONSONANTS)];
   char  vowels_frq[sizeof (VOWELS)];
} PUZZLE_INFO;


/*
** -------------------- Prototypes ---------------------------
*/    


void  process_player_turn ();
void  process_computer_turn ();
void  process_stray_input ();
void  display_timer (ACTIONS op, INT2 time);
void  display_main_menu (ACTIONS op);
void  display_options_menu ();
void  display_status_menu (ACTIONS op);
void  display_game (ACTIONS op);
void  prompt_player_info ();
BOOL  initialize_file (FILE_INFO *file);
BOOL  is_file_open (FILE_INFO *file);
BOOL  open_file (FILE_INFO *file);
BOOL  read_file_header (FILE_INFO *file);
BOOL  get_new_puzzle (FILE_INFO *file);
void  cleanup_file (FILE_INFO *file);
void  put_up_puzzle (ACTIONS op);
BOOL  get_next_token (char *context, char *token);
void  draw_tile (INT2 x, INT2 y, char letter, BOOL hidden);
void  place_puzzle_tile (INT1 pos);
void  next_player ();
INT4  player_notify (char *notice, BOOL wait, UINT2 *mask, INT2 mask_len);
INT1  letter_in_puzzle (char letter, INT1 *index);
void  display_topic ();
void  update_available_letters (char letter);
void  display_available_letters ();
BOOL  remove_letter_from_string (char letter, char *string);
BOOL  letter_available (char letter);
BOOL  only_vowels_left ();
STATE_TYPE spin (ACTIONS action);
STATE_TYPE process_chosen_letter (char letter, INT1 *num);
STATE_TYPE choose_letter (INT2 letter_value);
STATE_TYPE buy_vowel ();
BOOL  solve_puzzle ();
INT4  timer (INT1 op);
void  game_over_processing ();
void  match_over_processing ();
BOOL  getch_timeout (INT4 time_limit, char *message);


/*
** -------------------- Global Data ---------------------------
*/


/*
** The puzzle in the current game.
*/
PUZZLE_INFO Puzzle;

/*
** Set up an array of players.
*/
PLAYER_TYPE Players[MAX_PLAYERS];
INT1 Player_num     = 0;
INT1 Number_players = 0;

/*
** Is a game going at the moment?
*/
BOOL Game_playing = FALSE;
BOOL Exit_game    = FALSE;

/*
** Is this the start of a new game?
*/
BOOL New_game = TRUE;

/*
** Is the timer on?
*/
BOOL Timer_on = TRUE;

/*
** Load the default timer values
*/
INT2 Turn_limit  = TAKE_TURN_LIMIT;
INT2 Solve_limit = SOLVE_PUZZLE_LIMIT;

/*
** Make a list of the valid characters that are vowels and consonants.
** Concatenate these into the alphabet.
*/
char Vowels[]     = VOWELS;
char Consonants[] = CONSONANTS;
char Alphabet[]   = VOWELS CONSONANTS;

/*
** There are often messages for the user that need to be built
** with calls to sprintf and/or strcat.  Make a global buffer
** that can be used to do this so that each routine does not
** have to allocate one.  The space for this is malloced at
** the start of the program.
*/
char *Text;
INT2 Text_size = 255;

/*
** Current skill level of the game.
*/
SKILL_TYPE Skill_level = BEGINNER;

/*
** Number of games that have been played.
*/
INT1 Game_count = 0;

/*
** Keys that are allowed for a yes/no window.
*/
INT2  Key_yn_mask_len = 6;
UINT2 Key_yn_mask[6] = {
   SELECT_KEY | 'y',
   'y',
   'Y',
   SELECT_KEY | 'n',
   'n',
   'N',
};

INT2  Key_return_mask_len = 1;
UINT2 Key_return_mask[1] = {
   RETURN_KEY
};


/*
** -------------------- Functions ---------------------------
*/


/*******************************************************************************
**
** Unit Name:
**    app_main
**
** Description:
**    Main function.  Performs some initialization and then loops
**    until the user exits the game.
**
** Calling convention:
**    app_main ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void app_main ()
{
   FILE_INFO phrase_file;

   while (!initialize_file (&phrase_file)) {
      player_notify (DISK_ERROR_TXT, TRUE, 0, 0);
   }
   
   display_splash_scr (APP_NAME);
   
   srand (ANY_NUMBER);

   Text = (char *) malloc (Text_size);
   if (Text == NULL) {
      player_notify ("Can not malloc enough space.\\nAborting.\\nPress a key", 
                     TRUE, 0, 0);
      return;
   }
   
   /*
   ** As long as none of the players choose to exit the game,
   ** keep playing new games.
   */
   while (!Exit_game) {

      /*
      ** Keep track of the number of games played.
      */
      Game_count++;

      if (Game_count >= GAMES_PER_MATCH) {
         match_over_processing ();
      }

      Game_playing = TRUE;    

      /*
      ** The newer ROMs close your file for you when you get an error.
      ** If the user takes the disk out and puts it back in then he
      ** will get an error unless the program is smart enough to try to
      ** reopen the file.  Thus the double layer of testing here.
      */
      if (!get_new_puzzle (&phrase_file)) {
         if (!is_file_open (&phrase_file)) {
            open_file (&phrase_file);
         }
         while (!get_new_puzzle (&phrase_file)) {
            player_notify (DISK_ERROR_TXT, TRUE, 0, 0);
            if (!is_file_open (&phrase_file)) {
               open_file (&phrase_file);
            }
         }
      }

      if (New_game) {
         /*
         ** Set Player_num to be the last player so that when 
         ** next_player is called it will increment Player_num
         ** to point to the first player in the list.
         */
         prompt_player_info ();
         Player_num = Number_players;

         Game_count = 0;

         display_game (DISPLAY);
         New_game = FALSE;
      }
      else {
         display_game (UPDATE);
      }

      /*
      ** Keep cycling through the players until the game is over.
      ** Flush the keyboard buffer between turns.
      */
      while (Game_playing && (!Exit_game)) {
         inp_flush ();

         next_player ();

         if (Players[Player_num].computer_player) {
            process_computer_turn ();
            process_stray_input ();
         }
         else {
            process_player_turn ();
         }
      }
   }

   cleanup_file (&phrase_file);

   free (Text);

   return;   
}

/*******************************************************************************
**
** Unit Name:
**    process_player_turn
**
** Description:
**    As long as a player does not lose his turn he can keep playing.  Keep
**    looping and processing the player's moves until he loses his turn.
**
** Calling convention:
**    process_player_turn ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void process_player_turn ()
{
   BOOL  turn_over;
   BOOL  bad_key = FALSE;
   UINT2 key;
   STATE_TYPE state;
   STATE_TYPE spin_state;

   /*
   ** Reset the timer for the start of the turn.
   */
   timer (TIMER_RESET);
   timer (TIMER_START);

   /*
   ** As long as the user keeps getting letters right he can
   ** keep taking turns.
   */
   do {
      /*
      ** Time the user.  If they do not press a key during the time
      ** limit then their turn is over.
      */
      sprintf (Text, TAKE_TURN_MSG, Players[Player_num].name);
      turn_over = getch_timeout (Turn_limit, Text);

      /*
      ** If the user did not time out then he must have pressed
      ** a key.  See what key that was.
      */
      if (!turn_over) {
         key = inp_getch ();
         switch (key) {
#if DEBUG
            case ('c' | SELECT_KEY):
            case 'c':
            case 'C':
               game_over_processing ();
               Game_playing = FALSE;
               break;
#endif
            case ('s' | SELECT_KEY):
            case 's':
            case 'S':
               /*
               ** If you get a free spin you get to keep spinning.
               */
               do {
                  spin_state = spin (SPIN);
                  switch (spin_state) {
                     case LOSE_TURN:
                        turn_over = TRUE;
                        sprintf (Text, H_LOSE_TURN_TXT, 
                                 Players[Player_num].name);
                        player_notify (Text, FALSE, 0, 0);
                        break;
                     case FREE_SPIN:
                        Players[Player_num].free_spin++;
                        sprintf (Text, H_FREE_SPIN_TXT, 
                                 Players[Player_num].name);
                        player_notify (Text, FALSE, 0, 0);
                        break;
                     case BROKE:
                        turn_over = TRUE;
                        sprintf (Text, H_BROKE_TXT, Players[Player_num].name);
                        player_notify (Text, FALSE, 0, 0);
                        Players[Player_num].game_dollars = 0;
                        display_status_menu (UPDATE);
                        break;
                     case INVALID_CHOICE:
                        bad_key = TRUE;
                        break;
                     default:
                        state = choose_letter (spin_state * 100);
                        switch (state) {
                           case GOOD_MOVE:
                              break;
                           default:
                              turn_over = TRUE;
                              break;
                        }
                        break;
                  }
               } while (spin_state == FREE_SPIN);
               break;
            case ('v' | SELECT_KEY):
            case 'V':
            case 'v':
               state = buy_vowel ();
               if (state == INVALID_CHOICE) {
                  bad_key = TRUE;
               }
               else if (state != GOOD_MOVE) {
                  turn_over = TRUE;
               }
               break;
            case ('g' | SELECT_KEY):
            case 'g':
            case 'G':
               if (solve_puzzle ()) {
                  Game_playing = FALSE;
                  turn_over = TRUE;
               }
               else {
                  turn_over = TRUE;
               }
               break;
            case ('n' | SELECT_KEY):
               Game_playing = FALSE;
               New_game = TRUE;
               bad_key = TRUE;
               break;
            case ('o' | SELECT_KEY):
               if (Game_playing) {
                  timer (TIMER_STOP);
               }

               display_options_menu ();
               display_main_menu (UPDATE);

               if (Game_playing) {
                  timer (TIMER_START);
               }

               bad_key = TRUE;
               break;
            case ('q' | SELECT_KEY):
            case EXIT_KEY:
               if (Game_playing) {
                  timer (TIMER_STOP);
               }

               key = prompt_exit (((NEW_MAX_X - NEW_MIN_X) >> 1) + NEW_MIN_X,
                                  ((NEW_MAX_Y - NEW_MIN_Y) >> 1) + NEW_MIN_Y);
               Exit_game = ((key == ('y' | SELECT_KEY)) || 
                            (key == 'y') ||
                            (key == 'Y'));

               if (Game_playing) {
                  timer (TIMER_START);
               }

               if (Exit_game) {
                  Game_playing = FALSE;
               }
               bad_key = TRUE;
               break;
            case ('h' | SELECT_KEY):
#ifdef LCD
            case ('1' | CODE_KEY):
#else
            case '0':
#endif
            case HELP_KEY:
               if (Game_playing) {
                  timer (TIMER_STOP);
               }

               help_display (GAME_HELP);
               display_game (DISPLAY);

               if (Game_playing) {
                  timer (TIMER_START);
               }
               bad_key = TRUE;
               break;
            default:
               bad_key = TRUE;
               break;
         }
      }

      /*
      ** If the user pressed some invalid key then we do not
      ** want to reset the timer.  Reset the timer ONLY if the
      ** user gets another turn.
      */
      if (!bad_key) {
         timer (TIMER_RESET);
         timer (TIMER_START);
      }
      bad_key = FALSE;

      if (turn_over && (Players[Player_num].free_spin > 0) && Game_playing) {
         sprintf (Text, 
                  H_USE_FREE_SPIN_TXT, 
                  Players[Player_num].name, 
                  Players[Player_num].free_spin);
         key = player_notify (Text, TRUE, Key_yn_mask, Key_yn_mask_len);
         key = key & 0x00FF;
         if (toupper (key) == 'Y') {
            turn_over = FALSE;
            Players[Player_num].free_spin--;
            timer (TIMER_RESET);
            timer (TIMER_START);
         }
      }

      /*
      ** Typeahead generally hurts you in this game.  Flush the
      ** keyboard buffer as often as possible.
      */
      inp_flush ();
   } while ((!turn_over) && Game_playing);

   return;
}

/*******************************************************************************
**
** Unit Name:
**    process_computer_turn
**
** Description:
**    As long as the computer does not lose his turn he can keep playing.  Keep
**    looping and processing the computer's moves until he loses his turn.
**
** Calling convention:
**    process_computer_turn ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void process_computer_turn ()
{
   INT1 weight;
   INT1 len;
   INT1 count;
   INT1 x;
   INT2 percentage;
   BOOL turn_over = FALSE;
   DO_CHOICES choice;
   STATE_TYPE state;
   STATE_TYPE spin_state;
   
   /*
   ** Blank out the timer window.  The computer is not timed.
   */
   display_timer (UPDATE, -1);

   /*
   ** The computer gets to go until he loses his turn.
   */
   do {
      /*
      ** See what percentage of the letters are turned over.  If
      ** there is too low a percentage then the computer will not
      ** try to guess it.
      */
      count = 0;
      len = strlen (Puzzle.text);
      for (x = 0; x < len; x++) {
         if (!Puzzle.hidden[x]) {
            count++;
         }
      }
      percentage = (count * 100) / len;

      /*
      ** Add a weight based on how much money the computer has.  The
      ** more money the computer has, the greater the odds are he will
      ** try to guess the puzzle.
      */
      percentage += Players[Player_num].game_dollars / COMPUTER_MONEY_WEIGHT;

      /*
      ** If the percentage is too low for the computer
      ** to try to guess the puzzle then do not let it try.
      */
      if (percentage < MIN_PERCENT_TO_SOLVE) {
         weight = 0;
      }
      else {
         weight = BASE_CHANCE_SOLVE + percentage;
      }

      /*
      ** See if the computer will try to solve the puzzle.  The computer
      ** will if the odds are high enough or if the entire puzzle is
      ** exposed (which is true if there are no consonants left in the
      ** puzzle and all of the vowels have been chosen maybe a routine
      ** should be written some day to abstract this out of this IF stmt).
      */
      if (ODDS < weight || (only_vowels_left () && 
                            (strlen (Puzzle.vowels) == 0))) {
         choice = DO_SOLVE;
      }
      else {
         /*
         ** The computer will spin or buy a vowel.  If there are only
         ** vowels left then the computer will have to buy a vowel.
         ** If there are only vowels left and the computer does not have
         ** enough money to buy a vowel it will time out.
         */
         if (Players[Player_num].game_dollars >= VOWEL_COST) {
            if (only_vowels_left ()) {
               if (strlen (Puzzle.vowels) > 0) {
                  choice = DO_BUY_VOWEL;
               }
               else {
                  choice = DO_SOLVE;
               }
            }
            else {
               /*
               ** There are vowels and consonants.  See which the computer
               ** will try to pick.
               */
               if (ODDS < ODDS_BUY_VOWEL && (strlen (Puzzle.vowels) > 0)) {
                  choice = DO_BUY_VOWEL;
               }
               else {
                  choice = DO_SPIN;
               }
            }
         }
         else {
            /*
            ** The computer can not afford to buy a vowel.  If
            ** there are only vowels left then the computer will
            ** just time out.  Otherwise, the computer will spin.
            */
            if (only_vowels_left ()) {
               /*
               ** If there are only computer players and all of the
               ** computer players run out of money and there are
               ** no consonants left then the game will go into a
               ** deadlock if there is not enough of the puzzle 
               ** exposed for the computer to take a chance at 
               ** guessing it.  If a computer has to time out then
               ** it is very likely this scenario is taking place.
               ** Give the computer another chance at solving the 
               ** puzzle to break the deadlock.  Do not give the 
               ** computer a very high chance, though.  Remember that
               ** the human player is probably in the same boat.
               */
               if (ODDS < ODDS_BREAK_DEADLOCK) {
                  choice = DO_SOLVE;
               }
               else {
                  choice = DO_TIME_OUT;
               }
            }
            else {
               choice = DO_SPIN;
            }
         }
      }


      /*
      ** Now that the computer has finally figured out what he is
      ** going to do, do it.
      */
      switch (choice) {
         case DO_SPIN:
            /*
            ** If you get a free spin you get to keep spinning.
            */
            do {
               /*
               ** The computer will spin.
               */
               spin_state = spin (SPIN);
               switch (spin_state) {
                  case LOSE_TURN:
                     turn_over = TRUE;
                     sprintf (Text, C_LOSE_TURN_TXT, Players[Player_num].name);
                     player_notify (Text, FALSE, 0, 0);
                     break;
                  case FREE_SPIN:
                     sprintf (Text, C_FREE_SPIN_TXT, Players[Player_num].name);
                     player_notify (Text, FALSE, 0, 0);
                     Players[Player_num].free_spin++;
                     break;
                  case BROKE:
                     turn_over = TRUE;
                     sprintf (Text, C_BROKE_TXT, Players[Player_num].name);
                     player_notify (Text, FALSE, 0, 0);
                     Players[Player_num].game_dollars = 0;
                     display_status_menu (UPDATE);
                     break;
                  default:
                     state = choose_letter (spin_state * 100);
                     switch (state) {
                        case SOLVE:
                           Game_playing = FALSE;
                           break;
                        case GOOD_MOVE:
                           break;
                        default:
                           turn_over = TRUE;
                           break;
                     }
                     break;
            
               }
            } while (spin_state == FREE_SPIN);
            break;
         case DO_BUY_VOWEL:
            turn_over = (buy_vowel () == LOSE_TURN);
            break;
         case DO_TIME_OUT:
            sprintf (Text, C_TIME_OUT_TXT, Players[Player_num].name);
            player_notify (Text, FALSE, 0, 0);
            turn_over = TRUE;
            break;
         case DO_SOLVE:
            if (solve_puzzle ()) {
               turn_over = TRUE;
               Game_playing = FALSE;
            }
            else {
               turn_over = TRUE;
            }
            break;
      }

      /*
      ** If the computer loses his turn check to see if he opts to
      ** use his free spin.
      */
      if (turn_over && (Players[Player_num].free_spin > 0) && Game_playing) {
         if (percentage > MIN_PERCENT_TO_USE_FS) {
            if (ODDS < ODDS_USE_FREE_SPIN) {
               sprintf (Text, C_USING_FREE_SPIN_TXT, Players[Player_num].name);
               player_notify (Text, FALSE, 0, 0);
               Players[Player_num].free_spin--;
               turn_over = FALSE;
            }
         }
      }
   
      /*
      ** Check to see if the user has any keys pending.  The user
      ** may pick an option such as NEW GAME that will cause Game_playing
      ** to be set to FALSE, thus ending the computer's turn.
      */
      process_stray_input ();

   } while ((!turn_over) && Game_playing);
   
   return;
}

/*******************************************************************************
**
** Unit Name:
**    process_stray_input
**
** Description:
**    The person using this game can only perform menu choices on his turn
**    unless calls to this function are sprinkled through the computer's
**    turn processing code.  If they are not then the player has to wait
**    for the computer to finish to get control.  If there are only computers
**    playing then the user will NEVER get a chance.  This routine handles
**    stray input such as HELP, EXIT_GAME, etc.
**
** Calling convention:
**    process_stray_input ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void process_stray_input ()
{
   UINT2 key;
   
   while (inp_kbhit ()) {
      key = inp_getch ();
      switch (key) {
#if DEBUG
         case ('c' | SELECT_KEY):
         case 'c':
         case 'C':
            game_over_processing ();
            Game_playing = FALSE;
            break;
#endif
         case ('n' | SELECT_KEY):
            Game_playing = FALSE;
            New_game = TRUE;
            break;
         case ('o' | SELECT_KEY):
            if (Game_playing) {
               timer (TIMER_STOP);
            }

            display_options_menu ();
            display_main_menu (UPDATE);

            if (Game_playing) {
               timer (TIMER_START);
            }
            break;
         case ('q' | SELECT_KEY):
         case EXIT_KEY:
            if (Game_playing) {
               timer (TIMER_STOP);
            }

            key = prompt_exit (((NEW_MAX_X - NEW_MIN_X) >> 1) + NEW_MIN_X,
                               ((NEW_MAX_Y - NEW_MIN_Y) >> 1) + NEW_MIN_Y);
            Exit_game = ((key == ('y' | SELECT_KEY)) || 
                         (key == 'y') ||
                         (key == 'Y'));

            if (Game_playing) {
               timer (TIMER_START);
            }

            if (Exit_game) {
               Game_playing = FALSE;
            }
            break;
         case ('h' | SELECT_KEY):
#ifdef LCD
         case ('1' | CODE_KEY):
#else
         case '0':
#endif
         case HELP_KEY:
            if (Game_playing) {
               timer (TIMER_STOP);
            }

            help_display (GAME_HELP);
            display_game (DISPLAY);

            if (Game_playing) {
               timer (TIMER_START);
            }
            break;
      }
   }

   return;
}

/*******************************************************************************
**
** Unit Name:
**    display_timer
**
** Description:
**    Displays the timer window and the time or just updates the time.
**
** Calling convention:
**    display_timer (ACTIONS op, INT2 time)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    op                How much of the menu is to redraw.
**                         DISPLAY = entire window
**                         UPDATE  = just the text
**    time              What time to display in the window.
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void display_timer (ACTIONS op, INT2 time)
{
   char buffer[4];
   
   font_select (TIMER_FONT);

   if (op == DISPLAY) {
#ifndef LCD
      gr_bevel_rect(TIMER_X,
                    TIMER_Y,
                    TIMER_WIDTH,
                    TIMER_HEIGHT,
                    BORDER_WIDTH,
                    GR_NONE,
                    BEVEL_OUT);

      gr_bevel_rect(TIMER_X + BORDER_WIDTH + FRAME_WIDTH,
                    TIMER_Y + BORDER_WIDTH + FRAME_WIDTH,
                    TIMER_WIDTH - ((BORDER_WIDTH + FRAME_WIDTH) << 1),
                    TIMER_HEIGHT - ((BORDER_WIDTH + FRAME_WIDTH) << 1),
                    BORDER_WIDTH,
                    GR_NONE,
                    BEVEL_IN);
#endif

      gr_rect (TIMER_X + BEVEL_WIDTH,
               TIMER_Y + BEVEL_WIDTH,
               TIMER_WIDTH - (BEVEL_WIDTH << 1),
               TIMER_HEIGHT - (BEVEL_WIDTH << 1),
               1,
               GR_BLACK,
               GR_WHITE);

      font_printf_centered (TIMER_TXT,
                            TIMER1_X,
                            TIMER1_Y,
                            TIMER1_X + TIMER1_WIDTH,
                            TIMER1_Y + TIMER1_HEIGHT);
   }

   gr_rect (TIMER2_X + 1,
            TIMER2_Y + 1,
            TIMER2_WIDTH - 2,
            TIMER2_HEIGHT - 2,
            0,
            GR_NONE,
            GR_WHITE);

   if (time >= 0) {
      sprintf (buffer, "%d", time);
   
      font_printf_centered (buffer,
                            TIMER2_X,
                            TIMER2_Y,
                            TIMER2_X + TIMER2_WIDTH,
                            TIMER2_Y + TIMER2_HEIGHT);
   }

   return;
}

/*******************************************************************************
**
** Unit Name:
**    display_main_menu
**
** Description:
**    This function displays the main menu and the options contained within.
**
** Calling convention:
**    display_main_menu (ACTIONS op)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    op                How much of the menu is to redraw.
**                         DISPLAY = entire window
**                         UPDATE  = just the text
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY_MAIN_MENU */
/* .  DISPLAY MENU BOX */
/* .  DISPLAY MAIN MENU TEXT */
/* END DISPLAY_MAIN_MENU */
void display_main_menu (ACTIONS op)
{
   FONT_ATTR attr;
   
   font_select (MENU_FONT);

   /*
   ** Up the leading a little to space out this menu.
   */
   font_get_attributes (&attr);
   attr.leading += MENU_LEADING;
   font_set_attributes (&attr);


   if (op == DISPLAY) {
#ifndef LCD
      gr_bevel_rect (MENU_X,
                     MENU_Y,
                     MENU_WIDTH,
                     MENU_HEIGHT,
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_OUT);

       gr_bevel_rect (MENU_X + BORDER_WIDTH + FRAME_WIDTH,
                      MENU_Y + BORDER_WIDTH + FRAME_WIDTH,
                      MENU_WIDTH - ((BORDER_WIDTH + FRAME_WIDTH) << 1),
                      MENU_HEIGHT - ((BORDER_WIDTH + FRAME_WIDTH) << 1),
                      BORDER_WIDTH,
                      GR_NONE,
                      BEVEL_IN);
#endif

   }

   gr_rect (MENU_X + BEVEL_WIDTH,
            MENU_Y + BEVEL_WIDTH,
            MENU_WIDTH - (BEVEL_WIDTH << 1),
            MENU_HEIGHT - (BEVEL_WIDTH << 1),
            1,
            GR_BLACK,
            GR_WHITE);

   font_printf_centered (MAIN_MENU_TEXT, 
                         MENU_X, 
                         MENU_Y, 
                         MENU_X + MENU_WIDTH, 
                         MENU_Y + MENU_HEIGHT);

   return;    
}

/*******************************************************************************
**
** Unit Name:
**    display_options_menu
**
** Description:
**    This function displays the options menu and the options contained within.
**
** Calling convention:
**    display_options_menu (ACTIONS op)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    op                How much of the menu is to redraw.
**                         DISPLAY = entire window
**                         UPDATE  = just the text
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY_OPTIONS_MENU */
/* .  DISPLAY MENU BOX */
/* .  DISPLAY MAIN MENU TEXT */
/* END DISPLAY_OPTIONS_MENU */
void display_options_menu ()
{
   FONT_ATTR attr;
   UINT1 v_window_height;
   UINT1 win_num;
   BOOL reprint;
   BOOL exit_menu;
   UINT2 key;
   char menu[120];
   
   font_select (MENU_FONT);

   v_window_height = (MENU_HEIGHT - (BEVEL_WIDTH << 1)) / OPTIONS_V_WINDOWS;

   gr_rect (MENU_X + BEVEL_WIDTH,
            MENU_Y + BEVEL_WIDTH,
            MENU_WIDTH - (BEVEL_WIDTH << 1),
            MENU_HEIGHT - (BEVEL_WIDTH << 1),
            1,
            GR_BLACK,
            GR_WHITE);

   /*
   ** The leading is too small.
   ** The underlines are too far from the text.
   */
   font_get_attributes (&attr);
   attr.leading += OPTIONS_LEADING;
#ifdef LCD
   attr.underline_offset = 2;
#endif
   font_set_attributes (&attr);

   if (Timer_on) {
      sprintf (menu, MENU_TEMPLATE, ON_TEXT,  Turn_limit, Solve_limit);
   }
   else {
      sprintf (menu, MENU_TEMPLATE, OFF_TEXT, Turn_limit, Solve_limit);
   }

   switch (Skill_level) {
      case BEGINNER:
         strcat (menu, "Beginner");
         break;
      case NOVICE:
         strcat (menu, "Novice");
         break;
      case EXPERT:
         strcat (menu, "Expert");
         break;
   }

   font_printf_centered_each (menu,
                              MENU_X,
                              MENU_Y - OPTIONS_OFFSET,
                              MENU_X + MENU_WIDTH,
                              MENU_Y + MENU_HEIGHT);

   /*
   ** Process each of the user's choices.
   */
   exit_menu = FALSE;
   reprint = FALSE;
   do {
      key = inp_getch ();
      switch (key) {
         case EXIT_KEY:
         case RETURN_KEY:
            exit_menu = TRUE;
            break;
         case ('c' | SELECT_KEY):
            Timer_on = !Timer_on;

            if (Timer_on) {
               timer (TIMER_RESET);
               sprintf (Text, "%s", ON_TEXT);
            }
            else {
               /*
               ** Blank out the timer window just to look clean.
               ** Set the font back to what it should be afterwards.
               */
               display_timer (UPDATE, -1);
               font_select (MENU_FONT);
               sprintf (Text, "%s", OFF_TEXT);

            }

            reprint = TRUE;
            win_num = 1;
            break;
         case ('t' | SELECT_KEY):
            Turn_limit++;
            if (Turn_limit > MAX_TURN_LIMIT) {
               Turn_limit = MIN_TURN_LIMIT;
            }

            sprintf (Text, "%d", Turn_limit);
 
            reprint = TRUE;
            win_num = 3;
            break;
         case ('s' | SELECT_KEY):
            Solve_limit++;
            if (Solve_limit > MAX_SOLVE_LIMIT) {
               Solve_limit = MIN_SOLVE_LIMIT;
            }

            sprintf (Text, "%d", Solve_limit);

            reprint = TRUE;
            win_num = 5;
            break;
         case ('l' | SELECT_KEY):
            Skill_level++;
            if (Skill_level > EXPERT) {
               Skill_level = BEGINNER;
            }

            switch (Skill_level) {
               case BEGINNER:
                  strcpy (Text, "Beginner");
                  break;
               case NOVICE:
                  strcpy (Text, "Novice");
                  break;
               case EXPERT:
                  strcpy (Text, "Expert");
                  break;
            }

            reprint = TRUE;
            win_num = 7;
            break;
      }

      if (reprint) {
         gr_rect (MENU_X + BEVEL_WIDTH + 1,
                  MENU_Y + BEVEL_WIDTH + (v_window_height * win_num) + 1,
                  MENU_WIDTH - (BEVEL_WIDTH << 1) - 2,
                  v_window_height,
                  0,
                  GR_NONE,
                  GR_WHITE);
         
         font_printf_centered (Text,
                               MENU_X,
                               MENU_Y + BEVEL_WIDTH + (v_window_height * win_num),
                               MENU_X + MENU_WIDTH,
                               MENU_Y + BEVEL_WIDTH + 
                                (v_window_height * (win_num + 1)));

         reprint = FALSE;
      }
   } while (!exit_menu);
   
   return;    
}

/*******************************************************************************
**
** Unit Name:
**    display_status_menu
**
** Description:
**    Displays the status menu.
**
** Calling convention:
**    display_status_menu (ACTIONS op)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    op                How much of the menu is to redraw.
**                         DISPLAY = entire window
**                         UPDATE  = just the text
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void display_status_menu (ACTIONS op)
{
   INT1 i;
   INT2 width;
   INT2 descender;
   INT2 player_height;
   INT2 gap_size;
   char *temp;
   FONT_ATTR attr;
   FONT_COORD pos;
   static INT2 ascender;
   static FONT_COORD coords[MAX_PLAYERS];

   temp = (char *) malloc (120);
   if (temp == NULL) {
      return;
   }
   
   Text[0] = '\0';
   temp[0] = '\0';

   font_select (STATUS_FONT);

   /*
   ** Up the leading a little to space out this menu.
   */
   font_get_attributes (&attr);
   attr.leading += STATUS_LEADING;
   font_set_attributes (&attr);

   if (op == DISPLAY) {
#ifndef LCD
      gr_bevel_rect (STATUS_X,
                     STATUS_Y,
                     STATUS_WIDTH,
                     STATUS_HEIGHT,
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_OUT);

      gr_bevel_rect (STATUS_X + BORDER_WIDTH + FRAME_WIDTH,
                     STATUS_Y + BORDER_WIDTH + FRAME_WIDTH,
                     STATUS_WIDTH - ((BORDER_WIDTH + FRAME_WIDTH) << 1),
                     STATUS_HEIGHT - ((BORDER_WIDTH + FRAME_WIDTH) << 1),
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_IN);
#endif

      gr_rect (STATUS_X + BEVEL_WIDTH,
               STATUS_Y + BEVEL_WIDTH,
               STATUS_WIDTH - (BEVEL_WIDTH << 1),
               STATUS_HEIGHT - (BEVEL_WIDTH << 1),
               1,
               GR_BLACK,
               GR_WHITE);

#ifdef LCD
      /*
      ** There is just not enough room in the window to put big tabs.
      ** There is not enough room to put big spaces, either.
      */
      attr.tab_width    = LCD_TAB_WIDTH;
      attr.space_width -= 2;
      font_set_attributes (&attr);
#endif

      /*
      ** Get the width of the string for determining how much
      ** to space over when printing the dollar amounts.
      */
      font_string_size (GAME_TXT, &width, &ascender, &descender);

      /*
      ** Figure out the height of the entry for a single player.
      */
      player_height = LINES_PER_PLAYER * (attr.height + attr.leading);

      /*
      ** How much space is there between the top of the status window
      ** and the start of the first player's entry?
      */ 
      gap_size = (STATUS_HEIGHT - Number_players * player_height) >> 1;

      for (i = 0; i < Number_players; i++) {
         sprintf (temp, PLAYER_TXT, Players[i].name);
         strcat (Text, temp);
         if (i < Number_players - 1) {
            strcat (Text, "\\n");
         }

#ifdef LCD
         /*
         ** I don't know why you have to add 3 here, but you do.  
         ** This should be the same for either machine, but obviously
         ** it is not.  
         */
         coords[i].x = STATUS_X + BEVEL_WIDTH + width + attr.kerning + 3;
#else
         coords[i].x = STATUS_X + BEVEL_WIDTH + width + attr.kerning;
#endif
         coords[i].y = STATUS_Y + gap_size + (player_height * i) + 
                       attr.height + attr.leading;
      }

      pos.x = STATUS_X + BEVEL_WIDTH;
      pos.y = STATUS_Y + gap_size + ascender;
      font_set_pos (&pos);

      font_printf (Text);
   }

   for (i = 0; i < Number_players; i++) {
      gr_rect (coords[i].x,
               coords[i].y,
               STATUS_X + STATUS_WIDTH - BEVEL_WIDTH - coords[i].x - 1,
               (attr.height << 1),
               0,
               GR_NONE,
               GR_WHITE);

      pos.x = coords[i].x;
      pos.y = coords[i].y + ascender;
      font_set_pos (&pos);

      sprintf (Text, "%ld", Players[i].game_dollars);
      font_printf (Text);

      pos.y += attr.leading + attr.height;
      font_set_pos (&pos);

      sprintf (Text, "%ld", Players[i].match_dollars);
      font_printf (Text);
   }

   free (temp);

   return;
}

/*******************************************************************************
**
** Unit Name:
**    display_game
**
** Description:
**    This function displays the title bar, the used letter board,
**    and the wheel.
**
** Calling convention:
**    display_game (ACTIONS op)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    op                How much of the menu is to redraw.
**                         DISPLAY = entire window
**                         UPDATE  = just the text
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY_GAME */
/* .  PUT UP SPLASH SCREEN */
/* .  CLEAR SCREEN */
/* .  PUT UP TITLE BAR WITH GAME NAME */
/* .  DISPLAY THE BORDER AROUND THE MAIN MENU */
/* .  DISPLAY THE USED LETTER BOARD */
/* .  DISPLAY THE WHEEL */
/* .  DISPLAY A PLACE FOR THE NAMES OF THE PLAYERS */
/* END DISPLAY GAME */
void display_game (ACTIONS op)
{
   if (op == DISPLAY) {
      gr_clear_scr (GR_GRAY);
   
      window_title (TITLE_NAME);

      display_main_menu (op);

      display_status_menu (op);

      display_timer (op, -1);
       
#ifndef LCD
      spin (op);
#endif
   }

   display_topic ();

   put_up_puzzle (op);
   
   display_available_letters ();

   return;    
}
  
/*******************************************************************************
**
** Unit Name:
**    prompt_player_info
**
** Description:
**    This function displays a prompt in the main menu area to ask the player(s)
**    to enter the number of players that will be playing the game and then
**    enter the first name of each of the players (MAX_PLAYERS at the most).
**
** Calling convention:
**    prompt_player_info ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
/* BEGIN PROMPT_PLAYER_INFO */
/* .  CALL DISPLAY_MESSAGE TO PROMPT USER TO ENTER THE NUMBER OF PLAYERS */
/* .  GET THE PLAYER'S INPUT FOR THE NUMBER OF PLAYERS */
/* .  FOR 1 TO THE NUMBER OF PLAYERS DO */
/* .  .  CALL DISPLAY_MESSAGE TO PROMPT EACH PLAYER TO ENTER A FIRST NAME */
/* .  .  GET THE PLAYER'S NAME */
/* .  END FOR */
/* END PROMPT_PLAYER_INFO */
void prompt_player_info ()
{
   INT2  x;
   INT2  y;
   UINT1 i;
   UINT2 key;
   UINT1 key_char;
   BOOL  more_players;
   BOOL  name_done;

   gr_clear_scr (GR_GRAY);
   
   window_title (TITLE_NAME);

   font_select (TILE_FONT);
   
   /*
   ** Read in the information for each player.
   */
   Number_players = 0;

   do {
      /*
      ** Give the user a place to type in his name.  As he types
      ** letters the tiles will flip over, revealing the letter
      ** he types (just like in a real puzzle).
      */
      for (i = 0; i < MAX_NAME; i++) {
         x = NAME_AREA_X + i * (TILE_SIZE + TILE_SPACING);
         y = NAME_AREA_Y;
         draw_tile (x, y, key_char, TRUE);
      }

      /*
      ** Tell the user to enter the name.  Add one to the player
      ** number because users start counting at 1 while programmers
      ** start counting at 0.
      */
      sprintf (Text, ENTER_NAME_TXT, Number_players+1);
      bevel_window (Text, 
                    NAME_AREA_X, 
                    NAME_AREA_Y + NAME_AREA_HEIGHT * (TILE_SIZE + TILE_SPACING),
                    NAME_AREA_WIDTH * (TILE_SIZE + TILE_SPACING) - TILE_SPACING,
                    (TILE_SIZE + TILE_SPACING) * 3);

      i = 0;
      name_done = FALSE;

      do {
         key = inp_getch ();
         key_char = 0x00FF & key;
         switch (key_char) {
            case RETURN_KEY:
               if (strlen (Players[Number_players].name) > 0) {
                  name_done = TRUE;
               }
               else {
                  player_notify (NAME_HAVE_LENGTH_TXT, FALSE, 0, 0);
               }
               break;
            case BACKSPACE_KEY:
               if (i > 0) {
                  i--;
                  Players[Number_players].name[i] = '\0';
                  x = NAME_AREA_X + i * (TILE_SIZE + TILE_SPACING);
                  y = NAME_AREA_Y;
                  draw_tile (x, y, key_char, TRUE);
               }
               break;
            default:
               /*
               ** If this is a printable character then allow
               ** it in the name.
               */
               if ((key_char >= ' ') && (key_char <= '~') && (i < MAX_NAME)) {
                  x = NAME_AREA_X + i * (TILE_SIZE + TILE_SPACING);
                  y = NAME_AREA_Y;
                  draw_tile (x, y, key_char, FALSE);
                  Players[Number_players].name[i++] = key_char;
               }
               break;
         }
      } while (!name_done);

      /*
      ** Make the string null terminated.
      */
      Players[Number_players].name[i] = '\0';

      /*
      ** Blank out the message that tells the user to enter his name.
      */
      gr_rect (NAME_AREA_X,
               NAME_AREA_Y,
               NAME_AREA_WIDTH * (TILE_SIZE + TILE_SPACING) - TILE_SPACING,
               (TILE_SIZE + TILE_SPACING) * 4,
               0,
               GR_NONE,
               GR_GRAY);
      
      /*
      ** Ask the user if this is a computer player.
      */
      sprintf (Text, IS_COMPUTER_TXT, Players[Number_players].name);
      key = player_notify (Text, TRUE, Key_yn_mask, Key_yn_mask_len);
      key_char = toupper (0x00FF & key);
      Players[Number_players].computer_player = (key_char == 'Y');

      /*
      ** Initialize the player information.
      */
      Players[Number_players].game_dollars  = 0;
      Players[Number_players].match_dollars = 0;
      Players[Number_players].free_spin     = 0;

      Number_players++;

      /*
      ** If there can be more players then ask the user if
      ** he wants to enter more.  If not, return.
      */
      if (Number_players < MAX_PLAYERS) {
         /*
         ** Ask the user if there are more players.
         */
         key = player_notify (MORE_PLAYERS_TXT, TRUE, Key_yn_mask, 
                              Key_yn_mask_len);
         key_char = toupper (0x00FF & key);
         more_players = (key_char == 'Y');
      }
      else {
         more_players = FALSE;
      }

   } while (more_players);
   
   return;    
}

/*******************************************************************************
**
** Unit Name:
**    initialize_file
**
** Description:
**    This function tries to open the phrase file, malloc space for the headers,
**    and read those headers in.
**
** Calling convention:
**    (BOOL) success = get_new_puzzle (FILE_INFO *file)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    file              A pointer to a structure that contains the
**                      necessary file information.
**
** Return value:
**    BOOLean status as to whether the function was successful or not.
**
** Notes:
**
*******************************************************************************/
BOOL initialize_file (FILE_INFO *file)
{
   BOOL status = FALSE;

   if (open_file (file)) {
      /*
      ** Malloc storage for the relation table and the index table.
      */
      if ((file->relation_table = (char *) malloc (file->relation_size)) != 
          NULL) {
         /*
         ** Malloc storage for the index table.
         */
         if ((file->index_table = (UINT2 *) malloc (file->index_count)) != 
             NULL) {
            /*
            ** If the open and the malloc's worked then try to read the 
            ** file header.
            */
            if (!read_file_header (file)) {
               fileio_close (file->handle, (INT) SUSPEND);
            }
            status = TRUE;
         }
         else {
            free (file->relation_table);
         }
      }
   }

   return (status);
}

/*******************************************************************************
**
** Unit Name:
**    is_file_open
**
** Description:
**    Checks to see if the phrase file is open.  To check it tries to seek
**    into the file.  If the seek returns E_FILE_NOT_OPEN then the file
**    is not open.
**
** Calling convention:
**    (BOOL) state = is_file_open (FILE_INFO *file)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    file              A pointer to a structure that contains the
**                      necessary file information.
**
** Return value:
**    BOOLean status as to whether the file is open or not.
**
** Notes:
**
*******************************************************************************/
BOOL is_file_open (FILE_INFO *file)
{
   ERROR status;

   /*
   ** Try and access the file.  If the file is closed an error
   ** will be returned to that effect.
   */
   if (fileio_seek (file->handle, 
                    (LONG) 0, 
                    (USHORT) FILE_SEEK_SET, 
                    (INT) SUSPEND) == E_FILE_NOT_OPEN) {
      return (FALSE);
   }

   /*
   ** Sometimes you have to try twice to get the actual file
   ** not open error.
   */
   return (fileio_seek (file->handle, 
                        (LONG) 0, 
                        (USHORT) FILE_SEEK_SET, 
                        (INT) SUSPEND) != E_FILE_NOT_OPEN);
}

/*******************************************************************************
**
** Unit Name:
**    open_file
**
** Description:
**    Opens the phrase file.
**
** Calling convention:
**    (BOOL) success = open_file (FILE_INFO *file)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    file              A pointer to a structure that contains the
**                      necessary file information.
**
** Return value:
**    BOOLean status value as to whether the routine was able to complete
**    successfully.
**
** Notes:
**
*******************************************************************************/
BOOL open_file (FILE_INFO *file)
{
   BOOL  status = FALSE;
   INT2  x;
   UINT2 return_count;
   UINT2 buffer[BUFFER_SIZE];

   /*
   ** Try to open the file.
   */
   if (fileio_open (PHRASE_FILE_NAME, 
                    "rb", 
                    &file->handle, 
                    (INT) SUSPEND) == DISK_SUCCESS) {
      /*
      ** Read in the magic number, the number of relations, 
      ** and the number of indices.
      */
      if ((fileio_read (file->handle, 
                        (CHAR *) buffer, 
                        (USHORT) BUFFER_SIZE * sizeof (UINT2), 
                        &return_count, 
                        (INT) SUSPEND) == DISK_SUCCESS) &&
          (return_count == (BUFFER_SIZE * sizeof (UINT2)))) {
         /*
         ** Adjust the values for the machine this is on.
         */
         for (x = 0; x < BUFFER_SIZE; x++) {
            buffer[x] = BYTE_SWAP (buffer[x]);
         }

         /*
         ** If the magic number is not equal to what it 
         ** should be, then close the file.
         */
         if (buffer[MAGIC_NUMBER_IDX] == MAGIC_NUMBER) {
            file->relation_size = buffer[RELATION_SIZE_IDX];
            file->index_count   = buffer[INDEX_COUNT_IDX];
            status = TRUE;
         }
         else {
            cleanup_file (file);
         }
      }
   }

   return (status);
}

/*******************************************************************************
**
** Unit Name:
**    read_file_header
**
** Description:
**    Reads the header of the phrase file.
**
** Calling convention:
**    (BOOL) success = read_file_header (FILE_INFO *file)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    file              A pointer to a structure that contains the
**                      necessary file information.
**
** Return value:
**    BOOLean status value as to whether the routine was able to complete
**    successfully.
**
** Notes:
**
*******************************************************************************/
BOOL read_file_header (FILE_INFO *file)
{
   BOOL  success = FALSE;
   INT2  x;
   UINT2 return_count;
   ERROR status;


   /*
   ** Read in the relation table.
   */
   status = fileio_read (file->handle, 
                         file->relation_table,
                         file->relation_size,
                         &return_count, 
                         (INT) SUSPEND);
   if ((status == DISK_SUCCESS) && (return_count == file->relation_size)) {
      /*
      ** Read in the index table.
      */
      status = fileio_read (file->handle, 
                            (CHAR *) file->index_table, 
                            file->index_count, 
                            &return_count, 
                            (INT) SUSPEND);
      if ((status == DISK_SUCCESS) && (return_count == file->index_count)) {
         /*
         ** Adjust the values for the machine this is on.
         */
         for (x = 0; x < (file->index_count >> 1); x++) {
            file->index_table[x] = BYTE_SWAP (file->index_table[x]);
         }
         success = TRUE;
      }
   }

   return (success);
}

/*******************************************************************************
**
** Unit Name:
**    get_new_puzzle
**
** Description:
**    Reads a random phrase from the phrase file.
**
** Calling convention:
**    (BOOL) success = get_new_puzzle (FILE_INFO *file)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    file              A pointer to a structure that contains the
**                      necessary file information.
**
** Return value:
**    Returns a puzzle descriptor telling what the puzzle describes.
**
** Notes:
**
*******************************************************************************/
/* BEGIN GET_NEW_PUZZLE */
/* .  REPEAT */
/* .  .  RANDOMLY CHOOSE A PUZZLE CATEGORY */
/* .  .  ATTEMPT TO OPEN THE CORRESPONDING FILE ON THE DISK */
/* .  .  IF (UNABLE TO OPEN FILE) */
/* .  .  .  CALL DISPLAY_MESSAGE TO TELL USER TO INSERT THE GAME DISKETTE */
/* .  .  ELSE */
/* .  .  .  CHOOSE A RANDOM LINE IN THE FILE */
/* .  .  .  READ CHOSEN LINE INTO THE PUZZLE */
/* .  .  END IF */
/* .  .  FOR (EACH POSSIBLE LINE OF THE PUZZLE DISPLAYED) DO */
/* .  .  .  IF A WORD CROSSES THE BOUNDARY */
/* .  .  .  .  CALCULATE THE DISTANCE TO THE BEGINNING OF THE WORD */
/* .  .  .  .  IF THE DISTANCE IS LESS THAN THE SPACE AVAILABLE AT END OF PUZZLE */
/* .  .  .  .  .  SHIFT BEGINNING OF THE WORD TO THE BEGINNING OF THE NEXT LINE */
/* .  .  .  .  ELSE */
/* .  .  .  .  .  PUZZLE WILL NOT FIT ON THE BOARD */
/* .  .  .  .  END IF */
/* .  .  .  END IF */
/* .  .  END FOR */
/* .  .  RETURN THE PUZZLE CATEGORY */
/* .  END REPEAT WHEN PUZZLE FITS ON THE BOARD */
/* END GET_NEW_PUZZLE */
BOOL get_new_puzzle (FILE_INFO *file)
{
   char  phrase[MAX_PUZZLE + 5];
   BOOL  success = TRUE;
   INT1  i;
   INT2  length;
   UINT2 return_count;
   ERROR status;
#if INORDER
   static INT4 index = -1;
#else
   INT4 index;
#endif

   /*
   ** Pick a random phrase.  Figure out how long it is.  Subtract
   ** one for the linefeed.
   */
#if INORDER
   index++;
#else
   index  = rand () % ((file->index_count >>1) - 1);
#endif
   length = file->index_table[index + 1] - file->index_table[index] - 1;

   /*
   ** Seek to that phrase.
   */
   if (fileio_seek (file->handle, 
                    (LONG) file->index_table[index], 
                    (USHORT) FILE_SEEK_SET, 
                    (INT) SUSPEND) != DISK_SUCCESS) {
      success = FALSE;
   }
   else {
      /*
      ** Read that phrase.
      */
      status = fileio_read (file->handle,
                            phrase, 
                            (USHORT) length, 
                            &return_count, 
                            (INT) SUSPEND);
      if ((status != DISK_SUCCESS) ||
          (return_count != length)) {
         success = FALSE;
      }
   }

   if (success) {
      /*
      ** Make sure the string is null terminated.
      */
      phrase[return_count] = '\0';

      /*
      ** Copy the corresponding topic into the Puzzle struct.
      */
      strcpy (Puzzle.topic, 
              &file->relation_table[(phrase[0] - TOPIC_ADJUSTMENT) * 
               MAX_TOPIC]);

      /*
      ** Copy the puzzle text into the Puzzle struct.  Skip over the first
      ** byte of the text, as that is the byte that tells what kind of 
      ** phrase this is.  Initialize the unused letter strings.
      */
      strcpy (Puzzle.text, &(phrase[1]));
      strcpy (Puzzle.consonants, CONSONANTS);
      strcpy (Puzzle.vowels, VOWELS);
      strcpy (Puzzle.consonants_frq, CONSONANTS_FRQ);
      strcpy (Puzzle.vowels_frq, VOWELS_FRQ);
   
      /*
      ** Decide which characters should/should not be hidden.
      ** If the character is in the Alphabet then hide it.
      */
      for (i = 0; i < strlen (Puzzle.text); i++) {
         Puzzle.hidden[i] = IS_LETTER (Puzzle.text[i]);
      }
   }

   return (success);
}

/*******************************************************************************
**
** Unit Name:
**    cleanup_file
**
** Description:
**    Closes the phrase file and frees any memory that had been malloced.
**
** Calling convention:
**    (BOOL) success = cleanup_file (FILE_INFO *file)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    file              A pointer to a structure that contains the
**                      necessary file information.
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void cleanup_file (FILE_INFO *file)
{
   fileio_clearerror (file->handle, (INT) SUSPEND);

   free (file->relation_table);
   free (file->index_table);

   fileio_close (file->handle, (INT) SUSPEND);

   return;
}

/*******************************************************************************
**
** Unit Name:
**    put_up_puzzle
**
** Description:
**    This function displays the puzzle on the game board with the letters
**    facing away from the player.
**
** Calling convention:
**    put_up_puzzle (ACTIONS op)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    op                How much of the menu is to redraw.
**                         DISPLAY = entire window
**                         UPDATE  = just the text
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
/* BEGIN PUT_UP_PUZZLE */
/* .  FOR (EACH LINE IN THE PUZZLE) DO */
/* .  .  FOR (EACH CHARACTER IN THE LINE) DO */
/* .  .  .  IF (CHARACTER IS A LETTER) */
/* .  .  .  .  PUT UP THE LETTER FACING AWAY FROM THE PLAYER */
/* .  .  .  END IF */
/* .  .  END FOR */
/* .  .  ADVANCE TO NEXT LINE */
/* .  END FOR */
/* END PUT-UP_PUZZLE */
void put_up_puzzle (ACTIONS op)
{
   INT1 i;

   if (op == UPDATE) {
      gr_rect (TILE_AREA_X,
               TILE_AREA_Y, 
               TILE_AREA_WIDTH * (TILE_SIZE + TILE_SPACING),
               TILE_AREA_HEIGHT * (TILE_SIZE + TILE_SPACING),
               0,
               GR_NONE,
               GR_GRAY);
   }

   /*
   ** Display each tile in the puzzle.
   */
   for (i = 0; i < strlen (Puzzle.text); i++) {
      place_puzzle_tile (i);
   }

   return;    
}

/*******************************************************************************
**
** Unit Name:
**    get_next_token
**
** Description:
**    This function parses the first token of a string and returns that
**    token.  It also removes the token from the input string.
**
** Calling convention:
**    (BOOL) get_next_token = get_next_token (char *context, char *token)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    context           A pointer to the source string.  It is destructively
**                      parsed.
**    token             The string that was removed from 'context'.
**
** Return value:
**    BOOLean status value as to whether there was a token to get.
**
** Notes:
**
*******************************************************************************/
BOOL get_next_token (char *context, char *token)
{
#if 0
   INT1 x;
   INT1 start;

   /*
   ** If there is nothing left in the string then there are no
   ** tokens to pull out of it.
   */
   if (context[0] == '\0') {
      token[0] = '\0';
      return (FALSE);
   }
   
   /*
   ** Copy everything from the context string into the 
   ** token string up to the first space.
   */
   x = 0;
   while ((context[x] != ' ') && (context[x] != '\0')) {
      token[x] = context[x];
      x++;
   }

   /*
   ** Add a space to the end of the token and put on the null.
   */
   token[x] = ' ';
   token[x + 1] = '\0';

   /*
   ** Shift the values in context down to remove the part of 
   ** the string that was just copied into token.
   */
   start = x;
   while (context[x+1] != '\0') {
      x++;
      context[x - start - 1] = context[x];
   }

   /*
   ** Put the null on the new end of the string.
   */
   context[x - start] = '\0';

   return (TRUE);
#endif

   INT1 x;
   INT1 y;

   /*
   ** Copy everything from the context string into the 
   ** token string up to the first space.  Skip over any
   ** leading spaces first.
   */
   x = 0;
   while (context[x] == ' ') {
      x++;
   }

   y = 0;
   while ((context[x] != ' ') && (context[x] != '\0')) {
      token[y++] = context[x];
      context[x++] = ' ';
   }

   /*
   ** See if there was anything left in the string.
   */
   if (y == 0) {
      return (FALSE);
   }

   /*
   ** Add a space to the end of the token and put on the null.
   */
   token[y++] = ' ';
   token[y] = '\0';

   return (TRUE);
}

/*******************************************************************************
**
** Unit Name:
**    draw_tile
**
** Description:
**    Places a given tile on the screen at a given location.
**
** Calling convention:
**    draw_tile (INT2 x, INT2 y, char letter, BOOL hidden)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    x                 X coordinate (in pixels) of the upper left corner 
**                      of the tile
**    y                 Y coordinate (in pixels) of the upper left corner 
**                      of the tile
**    letter            The letter to be displayed on the tile
**    hidden            Is the letter to be covered up?
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void draw_tile (INT2 x, INT2 y, char letter, BOOL hidden)
{
   if (hidden) {
      /*
      ** Draw a shape to show that there is something hidden.
      */
#ifdef LCD
      gr_rect (x,
               y,
               TILE_SIZE,
               TILE_SIZE,
               1,
               GR_BLACK,
               GR_GRAY);
#else
      gr_bevel_rect (x,
                     y,
                     TILE_SIZE,
                     TILE_SIZE,
                     3,
                     GR_GRAY,
                     BEVEL_OUT);
#endif
   }
   else {
      /*
      ** Draw a white square and put the text on it.
      */
      gr_rect (x,
               y,
               TILE_SIZE,
               TILE_SIZE,
               1,
               GR_BLACK,
               GR_WHITE);

      /*
      ** font_printf_centered expects a string, not a character.
      */
      Text[0] = letter;
      Text[1] = '\0';

      /*
      ** Set the font that this is to be printed in.
      */
      font_select (TILE_FONT);
   
      /*
      ** Center the character in the tile.
      */
      font_printf_centered (Text,
                            x,
                            y,
                            x + TILE_SIZE,
                            y + TILE_SIZE);
   }

   return;    
}

/*******************************************************************************
**
** Unit Name:
**    place_puzzle_tile
**
** Description:
**    Plots the tile that corresponds to the letter at the given position
**    in the puzzle string.
**
** Calling convention:
**    place_puzzle_tile (INT1 pos)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    pos               Index into the source string of the character to
**                      be placed
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void place_puzzle_tile (INT1 pos)
{
   INT1 x;
   INT1 y;
   INT1 len;
   INT1 total;
   INT2 screen_x;
   INT2 screen_y;
   char token[MAX_PUZZLE];
   char context[MAX_PUZZLE];

   /*
   ** If this is a space then do not display anything.  If the
   ** calling routine is trying to plot a tile that is not in
   ** the puzzle then do not display anything.
   */
   if ((Puzzle.text[pos] == ' ') || (pos > strlen (Puzzle.text)) || (pos < 0)) {
      return;
   }
   
   y = 0;
   x = 0;
   total = 0;

   /*
   ** If the puzzle will fit on one line then we do not have
   ** to do much calculating.  If it will not fit on one line
   ** loop through the string, grabbing off a word at a time.
   ** If the word will fit on the current line then add its 
   ** length to x.  If it will not then increment y and set
   ** x to be the length of the string.  Once the desired
   ** position in the input string is passed, stop.
   */
   if (strlen (Puzzle.text) > TILE_AREA_WIDTH) {
      /*
      ** The tokenizing function is destructive.  Make a
      ** copy of the string.
      */
      strcpy (context, Puzzle.text);

      /*
      ** As long as we have not reached the character we
      ** are interested in, keep formatting.
      */
      while (total <= pos) {
         get_next_token (context, token);
         len = strlen (token);
         total += len;
         /*
         ** If the token will fit on the line (allow the trailing
         ** space to exceed the line length) then put it there,
         ** else wrap it to the next line.
         */
         if ((x + len - 1) <= TILE_AREA_WIDTH) {
            x += len;
         }
         else {
            x  = len;
            y++;

            /*
            ** Check to see if the puzzle is too long for the
            ** screen.  IF it is, then exit the while loop.
            */
            if (y >= TILE_AREA_HEIGHT) {
               player_notify ("The puzzle is too large.", TRUE, 0, 0);
               break;
            }
         }
      }
      /*
      ** We just passed the position we were looking for. 
      ** Back up to it.
      */
      x -= (total - pos);
   }
   else {
      /*
      ** The entire phrase will fit on one line.  This is just
      ** too easy!
      */
      x = pos;
      y = 0;
   }

   /*
   ** Given the size of the tiles, the spacing between them, and
   ** where the first one should start on the screen we should be
   ** able to calculate where this one sits.
   */
   screen_x = x * (TILE_SIZE + TILE_SPACING) + TILE_AREA_X;
   screen_y = y * (TILE_SIZE + TILE_SPACING) + TILE_AREA_Y;

   draw_tile (screen_x, screen_y, Puzzle.text[pos], Puzzle.hidden[pos]);

   return;    
}

/*******************************************************************************
**
** Unit Name:
**    next_player
**
** Description:
**    Cycles through the player list to the next player.  Tells this player
**    that it is now his turn.
**
** Calling convention:
**    next_player ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void next_player ()
{
   
   /*
   ** Increment the player number.  If the number is now
   ** greater than the total number of players then loop
   ** back to zero.
   */
   Player_num++;
   if (Player_num >= Number_players) {
      Player_num = 0;
   }

   /*
   ** Tell the user it is his turn.  If it is not his turn, but
   ** rather a computer's turn, then tell the user that.
   */
   if (Players[Player_num].computer_player) {
      sprintf (Text, C_TURN_NOTIFY_TXT, Players[Player_num].name);
   }
   else {
      sprintf (Text, H_TURN_NOTIFY_TXT, Players[Player_num].name);
   }

   player_notify (Text, FALSE, 0, 0);

   return;
}

/*******************************************************************************
**
** Unit Name:
**    player_notify
**
** Description:
**    Tells the current player a message that is passed in.  Based on the
**    call it either waits for a key press before pulling the notification
**    down or waits a given time.
**
** Calling convention:
**    (INT4) key = player_notify (char *notice, TRUE, UINT2 *mask, 
**                                INT2 mask_len)
**  or
**    player_notify (char *notice, FALSE, 0, 0)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    notice            A pointer to a string to be displayed in a pop-up
**                      window
**    wait              TRUE  - the window will remain until the user 
**                              presses a key
**                      FALSE - the window will remain for a predetermined
**                              amount of time
**    mask              A pointer to an array of 2 byte values that represent
**                      keypresses that are allowed to pull down the window.
**    mask_len          The number of elements in the mask array.
**
** Return value:
**    If the message was to be pulled down by a keystroke then that key is
**    returned.
**
** Notes:
**
*******************************************************************************/
INT4 player_notify (char *notice, BOOL wait, UINT2 *mask, INT2 mask_len)
{
   INT4 key = 0;

   /*
   ** If we are to wait for the user to press a key then do
   ** so.  Return the key that was pressed.  Otherwise just
   ** print the message and return.
   */
   if (wait) {
      /*
      ** Select the font that this is to be printed in.
      */
      font_select (NOTIFY_WAIT_FONT);

      key = pop_up_masked (notice,
                           NOTIFY_WAIT_MIN_X, 
                           NOTIFY_WAIT_MIN_Y,
                           NOTIFY_WAIT_MAX_X,
                           NOTIFY_WAIT_MAX_Y,
                           mask,
                           mask_len);
   }
   else {
      font_select (NOTIFY_NOWAIT_FONT);

#if BATCH_MODE
      flash_up (notice,
                NOTIFY_NOWAIT_BATCH_TIMEOUT,
                NOTIFY_NOWAIT_MIN_X, 
                NOTIFY_NOWAIT_MIN_Y,
                NOTIFY_NOWAIT_MAX_X,
                NOTIFY_NOWAIT_MAX_Y);
#else
      flash_up (notice,
                NOTIFY_NOWAIT_TIMEOUT,
                NOTIFY_NOWAIT_MIN_X, 
                NOTIFY_NOWAIT_MIN_Y,
                NOTIFY_NOWAIT_MAX_X,
                NOTIFY_NOWAIT_MAX_Y);
#endif
   }

   return (key);
}

/*******************************************************************************
**
** Unit Name:
**    letter_in_puzzle
**
** Description:
**    This function searches the puzzle for all occurrences of a given letter.
**
** Calling convention:
**    (INT1) count = letter_in_puzzle (char letter, INT1 *index)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    letter            The letter being sought in the puzzle.
**    index             Pointer to an array of integers representing
**                      indices in the puzzle where the letters were found.
**
** Return value:
**    Returns the number of occurences of letter in the puzzle.
**
** Notes:
**
*******************************************************************************/
/* BEGIN LETTER_IN_PUZZLE */
/* .  FOR (EACH LETTER IN THE PUZZLE) DO */
/* .  .  IF (LETTER IS THE ONE WE ARE LOOKING FOR) */
/* .  .  .  ADD THE INDEX OF THE LETTER IN THE PUZZLE TO THE LIST OF INDICES */
/* .  .  .  INCREMENT THE NUMBER OF OCCURRENCES FOUND */
/* .  .  END IF */
/* .  END FOR */
/* .  RETURN THE NUMBER OF OCCURRENCES */
/* END LETTER_IN_PUZZLE */
INT1 letter_in_puzzle (char letter, INT1 *index)
{
   INT1 i;
   INT1 count = 0;

   /*
   ** Count the number of times the given letter occurs in the
   ** puzzle.  Store these locations at which it is found in an
   ** array.
   */
   for (i = 0; i < strlen (Puzzle.text); i++) {
      if (Puzzle.text[i] == letter) {
         index[count++] = i;
      }
   }

   return (count);
}

/*******************************************************************************
**
** Unit Name:
**    display_topic
**
** Description:
**    Displays a small window that tells what kind of phrase this is.
**
** Calling convention:
**    display_topic ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void display_topic ()
{
   static INT2 width = 1;
   INT2 ascender;
   INT2 descender;

   gr_rect (TOPIC_AREA_OUT_X,
            TOPIC_AREA_OUT_Y,
            width + ((TOPIC_BORDER + TOPIC_IN) << 1),
            TOPIC_AREA_HEIGHT,
            0,
            GR_GRAY,
            GR_GRAY);

   font_select (TOPIC_FONT);
 
   sprintf (Text, "A %s. . .", Puzzle.topic);
 
   font_string_size (Text, &width, &ascender, &descender);

   gr_rect (TOPIC_AREA_OUT_X,
            TOPIC_AREA_OUT_Y,
            width + ((TOPIC_BORDER + TOPIC_IN) << 1),
            TOPIC_AREA_HEIGHT,
            1,
            GR_BLACK,
            GR_WHITE);

   font_printf_centered (Text,
                         TOPIC_AREA_IN_X, 
                         TOPIC_AREA_OUT_Y,
                         TOPIC_AREA_IN_X + width,
                         TOPIC_AREA_OUT_Y + TOPIC_AREA_HEIGHT);

   return;
}

/*******************************************************************************
**
** Unit Name:
**    update_available_letters
**
** Description:
**    Given a letter, redisplay it on the unused letter list.
**
** Calling convention:
**    update_available_letters (char letter)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    letter            The letter that is to be re-printed
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void update_available_letters (char letter)
{
   INT1 i;
   INT2 x;
   INT2 y;
   char *ptr;

   if (IS_VOWEL (letter)) {
      ptr = Vowels;
      x = UNUSED_VWL_AREA_X;
      y = UNUSED_VWL_AREA_Y;
   }
   else if (IS_CONSONANT (letter)) {
      ptr = Consonants;
      x = UNUSED_CON_AREA_X;
      y = UNUSED_CON_AREA_Y;
   }
   else {
      return;
   }

   i = 0;
   while (ptr[i] != letter) {
      i++;
   }
   
   x += i * (TILE_SIZE + TILE_SPACING);

   draw_tile (x, y, ptr[i], TRUE);

   return;
}
/*******************************************************************************
**
** Unit Name:
**    display_available_letters
**
** Description:
**    Redisplay the entire list of unused letters, blanking out the ones that
**    have been used.
**
** Calling convention:
**    display_available_letters ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void display_available_letters ()
{
   INT1 i;
   INT2 x;
   INT2 y;
   BOOL hidden;

   y = UNUSED_VWL_AREA_Y;
   for (i = 0; i < strlen (Vowels); i++) {
      x = UNUSED_VWL_AREA_X + i * (TILE_SIZE + TILE_SPACING);
      hidden = (strchr (Puzzle.vowels, Vowels[i]) == NULL);
      draw_tile (x, y, Vowels[i], hidden);
   }

   y = UNUSED_CON_AREA_Y;
   for (i = 0; i < strlen (Consonants); i++) {
      x = UNUSED_CON_AREA_X + i * (TILE_SIZE + TILE_SPACING);
      hidden = (strchr (Puzzle.consonants, Consonants[i]) == NULL);
      draw_tile (x, y, Consonants[i], hidden);
   }

   return;
}

/*******************************************************************************
**
** Unit Name:
**    remove_letter_from_string
**
** Description:
**    Given a letter, remove the first occurence of it from the given string.
**
** Calling convention:
**    (BOOL) was_guessed = remove_letter_from_string (char letter, char *string)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    letter            The letter to be removed.
**    string            A pointer to the string to remove the letter from.
**
** Return value:
**    Returns TRUE if the letter has not been guessed, FALSE otherwise.
**
** Notes:
**
*******************************************************************************/
BOOL remove_letter_from_string (char letter, char *string)
{
   UINT1 x;
   BOOL avail;

   /*
   ** Scan through the string looking for the letter.  If it
   ** is found then remove it and shift the rest of the letters
   ** down in the string to replace it.
   */
   avail = FALSE;
   x = 0;
   while (string[x] != '\0') {
      if (string[x] == letter) {
         avail = TRUE;
      }
      if (avail) {
         string[x] = string[x + 1];
      }
      x++;
   }

   return (avail);
}

/*******************************************************************************
**
** Unit Name:
**    letter_available
**
** Description:
**    This function determines if the letter passed to it has been selected yet.
**
** Calling convention:
**    (BOOL) avail = letter_available (char letter)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    letter            The letter selected by the current player.
**
** Return value:
**    Returns TRUE if the letter has not been guessed, FALSE otherwise.
**
** Notes:
**
*******************************************************************************/
/* BEGIN LETTER_AVAILABLE */
/* .  CONVERT LETTER TO INDEX OF THE AVAILABLE LETTER ARRAY */
/* .  IF (CORRESPONDING ARRAY VALUE HAS BEEN GUESSED) */
/* .  .  RETURN FALSE */
/* .  ELSE */
/* .  .  SET CORRESPONDING ARRAY VALUE TO GUESSED */
/* .  .  RETURN TRUE */
/* .  END IF */
/* END LETTER_AVAILABLE */
BOOL letter_available (char letter)
{
   BOOL avail;

   /*
   ** Set ptr to point at the string we are to scan.  If
   ** the letter is a vowel then point to the vowel string.
   ** If it is a consonant then point to the consonant string.
   ** If it is neither of these then return FALSE.
   */
   if (IS_VOWEL (letter)) {
      avail = remove_letter_from_string (letter, Puzzle.vowels_frq);
      avail = remove_letter_from_string (letter, Puzzle.vowels);
   }
   else if (IS_CONSONANT (letter)) {
      avail = remove_letter_from_string (letter, Puzzle.consonants_frq);
      avail = remove_letter_from_string (letter, Puzzle.consonants);
   }
   else {
      return (FALSE);
   }

   if (avail) {
      update_available_letters (letter);
   }

   return (avail);
}

/*******************************************************************************
**
** Unit Name:
**    only_vowels_left
**
** Description:
**    Checks the unexposed letters of the puzzle to see if there are only
**    vowels left to guess.
**
** Calling convention:
**    (BOOL) vowels = only_vowels_left ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    BOOLean status as to whether or not there are only vowels left.
**
** Notes:
**
*******************************************************************************/
BOOL only_vowels_left ()
{
   INT1 x;

   /*
   ** Loop through the puzzle string.  If there are any consonants
   ** that are still hidden then return FALSE.  Otherwise there 
   ** must be only vowels left (or nothing left) so return TRUE.
   */
   for (x = 0; x < strlen (Puzzle.text); x++) {
      if (IS_CONSONANT (Puzzle.text[x]) && Puzzle.hidden[x]) {
         return (FALSE);
      }
   }

   return (TRUE);
}


/*******************************************************************************
**
** Unit Name:
**    spin
**
** Description:
**    This function simulates the spinning of the wheel, spins the wheel on the
**    gameboard and returns the results of a spin.
**
** Calling convention:
**    (STATE_TYPE) state = spin (ACTION action)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    op                How much of the wheel is to redraw.
**                         DISPLAY = entire wheel
**                         SPIN    = just the numbers
**
** Return value:
**    SPIN_TYPE representing the results of a spin.
**
** Notes:
**
*******************************************************************************/
/* BEGIN SPIN */
/* .  RETURN RANDOM SPIN_TYPE */
/* END SPIN */
STATE_TYPE spin (ACTIONS action)
{
   BOOL  key_pressed;
   UINT2 key;
   STATE_TYPE state = GOOD_MOVE;

   /*
   ** Current number of entries on the wheel 
   */
   UCHAR    number_slots;   

   /*
   ** A virtual window is a slot on the wheel visible to the player numbered
   ** in order from bottom to top.
   */
   register INT2   vwin_number = (NUMBER_SLOTS_SEEN << 1) -1;

   /*
   ** An index in the array of pixmaps referring to the pixmap being
   ** displayed at the bottom of the wheel and pointed at by the arrow.
   */
   static   INT2   pixmap_index = 0;

   /*
   ** The number of slots a wheel is moved until it begins slowing down.
   */
   INT2     wheel_moves;

   /*
   ** The current pixmap being displayed.
   */
   INT2     current_pix_index;

   /*
   ** Current virtual window in which a pixmap is being displayed.
   */
   INT2     current_vwin;

   /*
   ** 
   */
   INT4     last_time = 0;
   INT4     first_time;
   INT4     elapsed_time;
   INT4     wait_time = 2;
   INT2     random_time;
   INT4     wait_duration = (ONE_SECOND /NUM_TICKS_PER_SEC);
   UINT1    pointing_at;
   BOOL     timer_expired;
   UINT2    wheel_pix_start_x;

#ifdef LCD
   BACKING_HANDLE *wheel_handle;
   BACKING_HANDLE *ptr_handle;
#endif

   if (Skill_level == EXPERT)
   {
      number_slots = NUMBER_SLOTS_EXPERT;
   }
   else if (Skill_level == NOVICE)
   {
      number_slots = NUMBER_SLOTS_NOVICE;
   }
   else
   {
      number_slots = NUMBER_SLOTS_BEGIN;
   }

   /*
   ** Bevels around the wheel.
   */
   wheel_pix_start_x = WHEEL_START_X + (BORDER_WIDTH <<1) + FRAME_WIDTH 
                       + (WHEEL_WHITE_SPACE >>1);

   if (action == DISPLAY) {
#ifndef LCD
      gr_bevel_rect (WHEEL_START_X,
                     WHEEL_START_Y,
                     WHEEL_BOX_WIDTH,
                     WHEEL_BOX_HEIGHT,
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_OUT);
      
      gr_bevel_rect (WHEEL_START_X + BORDER_WIDTH + FRAME_WIDTH,
                     WHEEL_START_Y + BORDER_WIDTH + FRAME_WIDTH,
                     WHEEL_BOX_WIDTH - (BORDER_WIDTH <<1) - (FRAME_WIDTH <<1),
                     WHEEL_BOX_HEIGHT - (BORDER_WIDTH <<1) - (FRAME_WIDTH <<1),
                     BORDER_WIDTH,
                     GR_WHITE,
                     BEVEL_IN);
#endif
      gr_rect (WHEEL_START_X + (BORDER_WIDTH <<1) + FRAME_WIDTH,
               WHEEL_START_Y + (BORDER_WIDTH <<1) + FRAME_WIDTH,
               WHEEL_BOX_WIDTH - (BORDER_WIDTH <<2) - (FRAME_WIDTH <<1),
               WHEEL_BOX_HEIGHT - (BORDER_WIDTH <<2) - (FRAME_WIDTH <<1),
               1,
               GR_BLACK,
               GR_NONE);

      gr_pixmap (WHEEL_START_X + WHEEL_BOX_WIDTH + 1,
                 WHEEL_START_Y + WHEEL_BOX_HEIGHT - ARROW_HEIGHT - 
                  (BORDER_WIDTH << 1),
                 Arrow,
                 GROP_SRC);

      for (current_vwin=0;current_vwin<(NUMBER_SLOTS_SEEN <<1);current_vwin++) {
         current_pix_index = current_vwin + pixmap_index;

         if (current_vwin + pixmap_index > (number_slots << 1) -1)
             current_pix_index = current_vwin + pixmap_index - (number_slots << 1);

         gr_pixmap (wheel_pix_start_x,
                    (((vwin_number - current_vwin) * WHEEL_PIX_HEIGHT)
                     + (BORDER_WIDTH <<1) + WHEEL_START_Y + FRAME_WIDTH) + 1,
                    Pix_type[current_pix_index],
                    GROP_SRC);
      }
   }

   else if (action == SPIN) {
      /*
      ** If there are no consonants left to expose then the user
      ** can not spin.  Do not charge the user for the time this
      ** message is up.
      */
      if (only_vowels_left ()) {
         if (!Players[Player_num].computer_player) {
            sprintf (Text, H_CANNOT_SPIN_TXT, Players[Player_num].name);
            timer (TIMER_STOP);
            player_notify (Text, FALSE, 0, 0);
            timer (TIMER_START);
         }
         state = INVALID_CHOICE;
      }
      else {
#ifdef LCD
         /*
         ** Store what is behind the wheel and the pointer.
         */
         wheel_handle = backing_store (WHEEL_START_X,
                                         WHEEL_START_Y,
                                         WHEEL_START_X + WHEEL_BOX_WIDTH,
                                         WHEEL_START_Y + WHEEL_BOX_HEIGHT);

         ptr_handle   = backing_store (WHEEL_START_X + WHEEL_BOX_WIDTH,
                                         WHEEL_START_Y + WHEEL_BOX_HEIGHT - 
                                         ARROW_HEIGHT - (BORDER_WIDTH << 1),
                                         WHEEL_START_X + WHEEL_BOX_WIDTH + 
                                         gr_pixmap_width (Arrow),
                                         WHEEL_START_Y + WHEEL_BOX_HEIGHT - 
                                         ARROW_HEIGHT - (BORDER_WIDTH << 1) + 
                                         gr_pixmap_height (Arrow));

         gr_rect (WHEEL_START_X,
                  WHEEL_START_Y,
                  WHEEL_BOX_WIDTH -2,
                  WHEEL_BOX_HEIGHT -2,
                  0,
                  GR_BLACK,
                  GR_WHITE);

         gr_pixmap (WHEEL_START_X + WHEEL_BOX_WIDTH - 2,
                    WHEEL_START_Y + WHEEL_BOX_HEIGHT - 
                     ARROW_HEIGHT - (BORDER_WIDTH << 1) + 1,
                    Arrow,
                    GROP_SRC);
       
#endif
         random_time = ((rand() % number_slots) + number_slots) << 1;

         for (wheel_moves = 0; wheel_moves < random_time; wheel_moves++) {
            for (current_vwin = 0; current_vwin < (NUMBER_SLOTS_SEEN << 1);
                 current_vwin++)
            {
               current_pix_index = current_vwin + pixmap_index;

               if (current_vwin + pixmap_index > (number_slots << 1) -1) {
                  current_pix_index = current_vwin + pixmap_index - 
                                      (number_slots << 1);
               }

               gr_pixmap (wheel_pix_start_x,
                          (((vwin_number - current_vwin) * WHEEL_PIX_HEIGHT)
                           + (BORDER_WIDTH <<1) + FRAME_WIDTH + WHEEL_START_Y) + 1,
                          Pix_type[current_pix_index],
                          GROP_SRC);
            }
            if (wheel_moves ==0){
               waitsecs(1); 
            }
            pixmap_index++;
            if (pixmap_index > (number_slots << 1) -1) {
               pixmap_index = 0;
            }

         }

         while(wait_time < NUM_TICKS_PER_SEC) {
            for (current_vwin = 0; current_vwin < (NUMBER_SLOTS_SEEN << 1); 
                 current_vwin++)
            {
               current_pix_index = current_vwin + pixmap_index;

               if (current_vwin + pixmap_index > (number_slots << 1) -1)
                   current_pix_index = current_vwin + pixmap_index - 
                                       (number_slots << 1);

               if (current_vwin == 1)
                   pointing_at = current_pix_index;

               gr_pixmap (wheel_pix_start_x,
                          (((vwin_number - current_vwin) * WHEEL_PIX_HEIGHT)
                           + (BORDER_WIDTH <<1) + WHEEL_START_Y + FRAME_WIDTH) + 1,
                          Pix_type[current_pix_index],
                          GROP_SRC);
            }             
            pixmap_index++;
            if (pixmap_index > (number_slots << 1) -1) {
               pixmap_index = 0;
            }

            first_time = TICKS;
            timer_expired = FALSE;

            do {
               last_time = TICKS;

               if (last_time < first_time) {
                  elapsed_time = TIMER_MAX - last_time + first_time;
               }
               else {
                  elapsed_time = last_time - first_time;
               }

               if (elapsed_time > (wait_time * wait_duration))
                  timer_expired = TRUE;

            } while (timer_expired == FALSE);

            wait_time *= 3;
            wait_time /= 2;
         }

         pixmap_index--;

#ifdef LCD
   /*
   ** Wait a bit so the user can see what was spun.
   */
   waitsecs (WHEEL_PULL_DOWN_DELAY);

   /*
   ** Remove the pop-up wheel.
   */
   backing_restore (wheel_handle);
   backing_restore (ptr_handle);

#endif

         state = Wheel_values[pointing_at >> 1];
      }
   }

   return (state);
}

/*******************************************************************************
**
** Unit Name:
**    process_chosen_letter
**    
** Description:
**    Given a letter, see if there were any in the puzzle and expose them
**    if there are.
**
** Calling convention:
**    (STATE_TYPE) state = process_chosen_letter (char letter, INT1 *num)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    letter            The letter to check against the current puzzle
**    num               A pointer to an integer in which the number of
**                      occurences will be returned
**
** Return value:
**    What state resulted from that letter, LOSE_TURN if there were none in
**    the puzzle, GOOD_MOVE if there were.
**
** Notes:
**
*******************************************************************************/
STATE_TYPE process_chosen_letter (char letter, INT1 *num)
{
   INT1  pos[MAX_PUZZLE];
   INT1  x;
   STATE_TYPE state = LOSE_TURN;

   if (letter_available (letter)) {
      /*
      ** If the letter is available (and it should be since it
      ** was chosen from the list of available letters) then
      ** see if it is in the puzzle.
      */
      *num = letter_in_puzzle (letter, pos);
      if (*num > 0) {
         /*
         ** There are *num occurences of the letter in the puzzle.
         ** Tell everyone that and reveal the letters.
         */
         if (*num == 1) {
            if (Players[Player_num].computer_player) {
               sprintf (Text, C_ONE_IN_PUZZLE_TXT,
                        Players[Player_num].name, letter);
            }
            else {
               sprintf (Text, H_ONE_IN_PUZZLE_TXT, letter,
                        Players[Player_num].name);
            }
         }
         else {
            if (Players[Player_num].computer_player) {
               sprintf (Text, C_MANY_IN_PUZZLE_TXT, 
                        Players[Player_num].name, letter, *num);
            }
            else {
               sprintf (Text, H_MANY_IN_PUZZLE_TXT, 
                        Players[Player_num].name, *num, letter);
            }
         }
         player_notify (Text, FALSE, 0, 0);
         
         for (x = 0; x < *num; x++) {
            Puzzle.hidden[pos[x]] = FALSE;
            place_puzzle_tile (pos[x]);
         }

         state = GOOD_MOVE;
      }
      else {
         /*
         ** There were none of those in the puzzle.
         */
         if (Players[Player_num].computer_player) {
            sprintf (Text, C_NONE_IN_PUZZLE_TXT, 
                     Players[Player_num].name, letter);
         }
         else {
            sprintf (Text, H_NONE_IN_PUZZLE_TXT, 
                     Players[Player_num].name, letter);
         }
         player_notify (Text, FALSE, 0, 0);
      }
   }
   else {
      /*
      ** If we get here it really is a bug since the computer
      ** was supposed to choose from the available letter list.
      ** Tell the user the computer goofed and picked a previously
      ** used letter.
      */
      if (Players[Player_num].computer_player) {
         sprintf (Text, C_ALREADY_CHOSEN_TXT, Players[Player_num].name,
                  letter);
      }
      else {
         sprintf (Text, H_ALREADY_CHOSEN_TXT, letter,
                  Players[Player_num].name);
      }
      player_notify (Text, FALSE, 0, 0);
   }

   return (state);
}

/*******************************************************************************
**
** Unit Name:
**    choose_letter
**
** Description:
**    If the timer option is set, a timer is displayed until the player chooses
**    a letter or the timer expires.
**
** Calling convention:
**    (STATE_TYPE) state = choose_letter (INT2 letter_value) 
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    letter_value      The worth of the current letter
**
** Return value:
**    What state resulted from that letter, LOSE_TURN if there were none in
**    the puzzle, GOOD_MOVE if there were.
**
** Notes:
**
*******************************************************************************/
/* BEGIN PLAYER_CHOOSE_LETTER */
/* .  IF (TIMER OPTION HAS BEEN SELECTED) */
/* .  .  START THE TIMER */
/* .  .  REPEAT */
/* .  .  .  WHILE(THE TIMER HAS NOT EXPIRED AND A KEY HAS NOT BEEN PRESSED) DO */
/* .  .  .  .  READ THE TIMER */
/* .  .  .  .  UPDATE THE TIMER ON THE SCREEN */
/* .  .  .  END WHILE */
/* .  .  .  IF (TIMER HAS NOT EXPIRED) */
/* .  .  .  .  PAUSE THE TIMER */
/* .  .  .  .  RETRIEVE THE KEYPRESS */
/* .  .  .  .  ECHO THE KEYPRESS TO THE SCREEN */
/* .  .  .  .  IF (KEYPRESS CORRESPONDS TO A LETTER) */
/* .  .  .  .  .  A LETTER HAS BEEN CHOSEN */
/* .  .  .  .  ELSE */
/* .  .  .  .  .  KEYPRESS IS NOT A LETTER */
/* .  .  .  .  .  A LETTER HAS NOT BEEN CHOSEN */
/* .  .  .  .  .  EMPTY KEYBOARD BUFFER */
/* .  .  .  .  .  CALCULATE REMAINING TIME */
/* .  .  .  .  .  RESUME TIMER */
/* .  .  .  .  END IF */
/* .  .  .  ELSE */
/* .  .  .  .  TIMER HAS EXPIRED */
/* .  .  .  .  LETTER_TYPE IS NO_LETTER */
/* .  .  .  END IF */
/* .  .  END REPEAT WHEN TIMER HAS EXPIRED OR LETTER HAS BEEN CHOSEN */
/* .  .  IF (LETTER_TYPE IS NOT NO_LETTER) */
/* .  .  .  DETERMINE TYPE OF LETTER */
/* .  .  END IF */
/* .  ELSE */
/* .  .  REPEAT */
/* .  .  .  IF (KEY HAS BEEN PRESSED) */
/* .  .  .  .  RETRIEVE THE KEYPRESS */
/* .  .  END REPEAT WHEN A VALID LETTER HAS BEEN CHOSEN */
/* .  END IF */
/* END PLAYER_CHOOSE_LETTER */
STATE_TYPE choose_letter (INT2 letter_value)
{
   UINT2 key;
   char  key_char;
   BOOL  timeout;
   INT1  index;
   INT1  num;
   STATE_TYPE state;

   /*
   ** See if this is a computer player or a human player.
   */
   if (Players[Player_num].computer_player) { 
      /*
      ** See if the computer will guess a letter at random,
      ** or if he will take the first one from the frequency
      ** list.
      */
      if (ODDS < ODDS_GUESS_FREQUENCY) {
         key_char = Puzzle.consonants_frq[0];
      }
      else {
         index = rand () % strlen (Puzzle.consonants);
         key_char = Puzzle.consonants[index];
      }

      state = process_chosen_letter (key_char, &num);
   }
   else {
      /*
      ** Flush the buffer just in case the user
      ** typed some junk.
      */
      inp_flush ();

      /*
      ** Tell the user to pick a consonant.
      */
      sprintf (Text, H_CHOOSE_CONSONANT_TXT, letter_value, 
               Players[Player_num].name);
      player_notify (Text, FALSE, 0, 0);

      /*
      ** This is a human player.  Time him.
      */
      timer (TIMER_RESET);
      timer (TIMER_START);

      /*
      ** See if the user can choose a letter before time runs out.
      */
      sprintf (Text, CHOOSE_LETTER_MSG, Players[Player_num].name);

      do {
         /*
         ** If the key is a SELECT_OUT_KEY and the user times out
         ** the loop will go forever unless you reset the key.
         */
         key = 0;
         timeout = getch_timeout (Turn_limit, Text);

         if (!timeout) {
            /*
            ** The user pressed a key before time ran out.  Get the key.
            ** Convert the key to a single byte and uppercase it.
            */
            key = inp_getch ();
            if (key != SELECT_OUT_KEY) {
               key_char = (UCHAR) (key & 0x00FF);
               key_char = toupper (key_char);

               /*
               ** If the letter is not a consonant then return a bad
               ** choice.
               */
               if (IS_CONSONANT (key_char)) {
                  state = process_chosen_letter (key_char, &num);
               }
               else {
                  sprintf (Text, H_NOT_CONSONANT_TXT, 
                           Players[Player_num].name, key_char);
                  player_notify (Text, FALSE, 0, 0);
               }
            }
         }
      } while (key == SELECT_OUT_KEY);
   }

   /*
   ** Give the player his money if he deserves any.
   */
   if (state == GOOD_MOVE) {
      Players[Player_num].game_dollars += letter_value * num;
      display_status_menu (UPDATE);
   }

   return (state);
}

/*******************************************************************************
**
** Unit Name:
**    buy_vowel
**
** Description:
**    Allow the player to choose a vowel.  Score that choice.
**
** Calling convention:
**    (STATE_TYPE) state = buy_vowel ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    What state resulted from that letter, LOSE_TURN if there were none in
**    the puzzle, GOOD_MOVE if there were.
**
** Notes:
**
*******************************************************************************/
STATE_TYPE buy_vowel ()
{
   UINT2 key;
   char  key_char;
   BOOL  timeout;
   INT1  index;
   INT1  num;
   STATE_TYPE state;

   /*
   ** If there are no vowels left to guess from then return.  Do not
   ** charge the user for the time this message is up.
   */
   if (strlen (Puzzle.vowels) == 0) {
      if (!Players[Player_num].computer_player) {
         timer (TIMER_STOP);
         sprintf (Text, H_NO_VOWELS_TXT, Players[Player_num].name);
         player_notify (Text, FALSE, 0, 0);
         timer (TIMER_START);
      }
      return (INVALID_CHOICE);
   }

   /*
   ** If the player tries to buy a vowel and does not have
   ** enough money then he will lose his turn.
   */
   if (Players[Player_num].game_dollars < VOWEL_COST) {
      sprintf (Text, H_CANNOT_BUY_VOWEL_TXT, Players[Player_num].name, 
               VOWEL_COST);
      player_notify (Text, FALSE, 0, 0);
      return (LOSE_TURN);
   }

   /*
   ** See if this is a computer player or a human player.
   */
   if (Players[Player_num].computer_player) { 
      /*
      ** See if the computer will guess a letter at random,
      ** or if he will take the first one from the frequency
      ** list.
      */
      if (ODDS < ODDS_GUESS_FREQUENCY) {
         key_char = Puzzle.vowels_frq[0];
      }
      else {
         index = rand () % strlen (Puzzle.vowels);
         key_char = Puzzle.vowels[index];
      }

      sprintf (Text, C_BUYING_VOWEL, Players[Player_num].name);
      player_notify (Text, FALSE, 0, 0);

      state = process_chosen_letter (key_char, &num);
   }
   else {
      /*
      ** Flush the buffer just in case the user
      ** typed some junk.
      */
      inp_flush ();

      /*
      ** Tell the user to pick a vowel.
      */
      sprintf (Text, H_CHOOSE_VOWEL_TXT, Players[Player_num].name);
      player_notify (Text, FALSE, 0, 0);

      /*
      ** This is a human player.  Time him.
      */
      timer (TIMER_RESET);
      timer (TIMER_START);

      /*
      ** See if the user can choose a letter before time runs out.
      */
      sprintf (Text, BUY_VOWEL_MSG, Players[Player_num].name);

      do {
         /*
         ** If the key is a SELECT_OUT_KEY and the user times out
         ** the loop will go forever unless you reset the key.
         */
         key = 0;
         timeout = getch_timeout (Turn_limit, Text);

         if (!timeout) {
            /*
            ** The user pressed a key before time ran out.  Get the key.
            ** Convert the key to a single byte and uppercase it.
            */
            key = inp_getch ();
            if (key != SELECT_OUT_KEY) {
               key_char = (UCHAR) (key & 0x00FF);
               key_char = toupper (key_char);

               /*
               ** If the letter is not a vowel then return a bad
               ** choice.
               */
               if (IS_VOWEL (key_char)) {
                  state = process_chosen_letter (key_char, &num);
               }
               else {
                  sprintf (Text, H_NOT_VOWEL_TXT, Players[Player_num].name,
                           key_char);
                  player_notify (Text, FALSE, 0, 0);
               }
            }
         }
      } while (key == SELECT_OUT_KEY);
   }

   /*
   ** Charge the player for the privilege of having entered this routine.
   */
   Players[Player_num].game_dollars -= VOWEL_COST;
   display_status_menu (UPDATE);

   return (state);
}

/*******************************************************************************
**
** Unit Name:
**    solve_puzzle
**
** Description:
**    This function prompts the player to attempt to solve the puzzle by entering
**    the letters that make up a puzzle. The player is then prompted to confirm
**    the letters entered.
**
** Calling convention:
**    (BOOL) solved = solve_puzzle ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    BOOLean status value as to whether the solution was correct.
**
** Notes:
**
*******************************************************************************/
/* BEGIN PLAYER_SOLVE_PUZZLE */
/* .  PROMPT THE USER TO ENTER A SOLUTION TO THE PUZZLE */
/* .  IF (TIMER IS SELECTED) */
/* .  .  START THE TIMER */
/* .  END IF */
/* .  REPEAT */
/* .  .  READ TIMER */
/* .  .  CHECK FOR */
BOOL solve_puzzle ()
{
   char  attempt[MAX_PUZZLE];
   char  old_char;
   UINT2 key;
   INT1  key_char;
   INT2  x;
   INT2  y;
   BOOL  solved;
   BOOL  solve_done;
   BOOL  timeout;
   INT1  i;

   /*
   ** See if this is a computer player or a human player.
   */
   if (Players[Player_num].computer_player) { 
      /*
      ** The computer will be correct at random.
      */
      sprintf (Text, C_SOLVE_ATTEMPT_TXT, Players[Player_num].name);
      player_notify (Text, FALSE, 0, 0);
      solved = (ODDS < ODDS_GUESS_RIGHT);
   }
   else {
      /*
      ** Tell the user to type his solution.
      */
      sprintf (Text, H_SOLVE_START_TXT, Players[Player_num].name);
      player_notify (Text, TRUE, Key_return_mask, Key_return_mask_len);

      /*
      ** Read in each character of the solution.  If the user takes
      ** too long to enter the solution, they lose their turn.
      */
      timer (TIMER_RESET);
      timer (TIMER_START);
      
      i = 0;
      solve_done = FALSE;

      do {
         sprintf (Text, SOLVE_PUZZLE_MSG, Players[Player_num].name);
         timeout = getch_timeout (SOLVE_PUZZLE_LIMIT, Text);

         /*
         ** If the user really does take too long then they lose.
         */
         if (!timeout) {
            /*
            ** Fill in everything for the user that is not hidden.
            */
            while ((!Puzzle.hidden[i]) && i < strlen (Puzzle.text)) {
               attempt[i] = Puzzle.text[i];
               i++;
            }
            attempt[i] = '\0';
    
            key = inp_getch ();
            key_char = toupper (0x00FF & key);
            switch (key_char) {
               case RETURN_KEY:
                  solve_done = TRUE;
                  break;
               case BACKSPACE_KEY:
                  if (i > 0) {
                     i--;
                     place_puzzle_tile (i);
                  }
                  while ((i > 0) && !Puzzle.hidden[i]) {
                     i--;
                     place_puzzle_tile (i);
                  }
                  attempt[i] = '\0';
                  break;
               default:
                  /*
                  ** If this is a letter then allow it in the solution.  
                  */
                  if ((key_char >= 'A') && (key_char <= 'Z') &&
                      (i < strlen (Puzzle.text))) {
                     /*
                     ** Uppercase the letter so that it matches the
                     ** phrase.
                     */
                     attempt[i] = key_char;
      
                     /*
                     ** Print the character the user typed by stuffing
                     ** it into the puzzle string and printing that
                     ** character of the puzzle.  Set the puzzle back
                     ** when done.
                     */
                     old_char = Puzzle.text[i];
                     Puzzle.text[i] = attempt[i];
                     Puzzle.hidden[i] = FALSE;
                     place_puzzle_tile (i);
                     Puzzle.text[i] = old_char;
                     Puzzle.hidden[i] = TRUE;
    
                     i++;
                     attempt[i] = '\0';
                  }
                  break;
            }
         }
      } while ((!solve_done) && (!timeout));

      if (timeout) {
         solved = FALSE;
      }
      else {
         /*
         ** Make the string null terminated.
         */
         attempt[i] = '\0';

         /*
         ** Check the attempt against the puzzle.
         */
         solved = (strcmp (attempt, Puzzle.text) == 0);
      }
   }

   if (solved) {
      if (!Players[Player_num].computer_player) {
         /*
         ** If the puzzle is correct then the correct letters are already
         ** on the screen.  Set all of the "hidden" values to false so that
         ** when game_over_processing is called it will not flip over pieces
         ** that are already exposed.  This gets rid of an annoying flicker.
         */
         for (x = 0; x < strlen (Puzzle.text); x++) {
            Puzzle.hidden[x] = FALSE;
         }
      }
      game_over_processing ();
   }
   else {
      /*
      ** Do not put up this message if it was a human player
      ** and the time ran out.
      */
      if (Players[Player_num].computer_player || !timeout) {  
         if (Players[Player_num].computer_player) {
            sprintf (Text, C_SOLVE_FAIL_TXT, Players[Player_num].name);
         }
         else {
            sprintf (Text, H_SOLVE_FAIL_TXT, Players[Player_num].name);
         }
         player_notify (Text, FALSE, 0, 0);
      }
      put_up_puzzle (DISPLAY);
   }

   return (solved);
}

/*******************************************************************************
**
** Unit Name:
**    timer
**
** Description:
**    Implementation of a stopwatch timer.
**
** Calling convention:
**    (INT4) time = timer (INT1 op)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    op                What the timer is to do
**                         TIMER_START  = start/continue timing
**                         TIMER_STOP   = pause timing
**                         TIMER_RESET  = zero the timer
**                         TIMER_REPORT = return the elapsed time
**
** Return value:
**    What time it is.
**
** Notes:
**
*******************************************************************************/
INT4 timer (INT1 op)
{
   static INT4 last_time    = 0;
   static INT4 start_time   = 0;
   static INT4 pause_time   = 0;
   static INT4 pause_start  = 0;
   static BOOL timer_paused = FALSE;
   INT4 now_time;

   /*
   ** See what time it is.
   */
   now_time = TICKS;

   switch (op) {
      /*
      ** You can start the timer only if it was paused.  If it is
      ** not paused then it is already started.
      */
      case TIMER_START:
         if (timer_paused) {
            pause_time   += now_time - pause_start;
            timer_paused  = FALSE;
         }
         break;
      case TIMER_STOP:
         /*
         ** You can stop the timer only if it was NOT paused.  If it is
         ** paused then it is already stopped.
         */
         if (!timer_paused) {
            pause_start  = now_time;
            timer_paused = TRUE;
         }
         break;
      case TIMER_RESET:
         /*
         ** Reset the time back to zero.
         */
         start_time   = now_time;
         pause_time   = 0;
         if (timer_paused) {
            pause_start = now_time;
         }
         break;
      case TIMER_REPORT:
         /*
         ** The timer ALWAYS reports, so just do nothing here.
         */
         break;
      default:
         break;
   }

   /*
   ** If the timer is paused then don't let any time elapse.
   */
   if (!timer_paused) {
      last_time = (now_time - start_time) - pause_time;
   }

   return (last_time);
}

/*******************************************************************************
**
** Unit Name:
**    game_over_processing
**
** Description:
**    Resets all the free spins and moneys of the players.  Puts up a
**    message to the winner.  Expose the rest of the puzzle.  Adds the 
**    game money of the winner to his match money.
**
** Calling convention:
**    game_over_processing ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void game_over_processing ()
{
   INT1 x;

   /*
   ** Expose the part of the puzzle that has not already been.
   */
   for (x = 0; x < strlen (Puzzle.text); x++) {
      if (Puzzle.hidden[x]) {
         Puzzle.hidden[x] = FALSE;
         place_puzzle_tile (x);
      }
   }
   
   /*
   ** Give credit to the winner.
   */
   if (Players[Player_num].computer_player) {
      sprintf (Text, C_WIN_TXT,
               Players[Player_num].name,
               Players[Player_num].game_dollars);
   }
   else {
      sprintf (Text, H_WIN_TXT,
               Players[Player_num].name,
               Players[Player_num].game_dollars);
   }

#if BATCH_MODE
   player_notify (Text, FALSE, 0, 0);
#else
   strcat  (Text, PRESS_A_KEY_TXT);
   player_notify (Text, TRUE, 0, 0);
#endif

   /*
   ** Give the player his money.
   */
   Players[Player_num].match_dollars += Players[Player_num].game_dollars;

   /*
   ** Reset everyone's money to zero.  Take away their
   ** free spins.
   */
   for (x = 0; x < Number_players; x++) {
      Players[x].game_dollars = 0;
      Players[x].free_spin    = 0;
   }

   display_status_menu (UPDATE);

   return;
}

/*******************************************************************************
**
** Unit Name:
**    match_over_processing
**
** Description:
**    Same as game_over_processing except that it resets the match values and
**    asks if new players should be chosen for the next game.
**
** Calling convention:
**    match_over_processing ()
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**    none
**
** Notes:
**
*******************************************************************************/
void match_over_processing () 
{
   INT1 ranking[MAX_PLAYERS];
   INT1 x;
   INT1 temp;
   BOOL ranking_changed;
   UINT2 key;
   char key_char;

   for (x = 0; x < Number_players; x++) {
      ranking[x] = x;
   }
   
   do {
      ranking_changed = FALSE;
      for (x = 0; x < (Number_players - 1); x++) {
         if (Players[ranking[x]].match_dollars < 
             Players[ranking[x+1]].match_dollars) {
            temp = ranking[x+1];
            ranking[x+1] = ranking[x];
            ranking[x] = temp;
            ranking_changed = TRUE;
         }
      }
   } while (ranking_changed);
   
   x = -1;
   do {
      x++;
      if (Players[ranking[x]].computer_player) {
         sprintf (Text, C_MATCH_WINNER_TXT, Players[ranking[x]].name, 
                  Players[ranking[x]].match_dollars);
      }
      else {
         sprintf (Text, H_MATCH_WINNER_TXT, Players[ranking[x]].name, 
                  Players[ranking[x]].match_dollars);
      }
      player_notify (Text, TRUE, 0, 0);
   } while ((Players[ranking[x]].match_dollars ==
             Players[ranking[x+1]].match_dollars) && 
            (x < (Number_players - 1)));

   key = player_notify (NEW_PLAYERS_TXT, TRUE, 0, 0);
   key_char = toupper (key & 0x00FF);
   New_game = (key_char == 'Y');

   Game_playing = FALSE;

   Game_count = 0;
   
   for (x = 0; x < Number_players; x++) {
      Players[x].game_dollars  = 0;
      Players[x].match_dollars = 0;
      Players[x].free_spin     = 0;
   }
   
   display_status_menu (UPDATE);

   return;
}

/*******************************************************************************
**
** Unit Name:
**    getch_timeout 
**
** Description:
**    Function that will wait the given amount of time and then display
**    the given message, or will return if a key was pressed.
**
** Calling convention:
**    (BOOL) timeout = getch_timeout (INT4 time_limit, char *message)
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    time_limit        How long to wait on the user for a keystroke before
**                      timing out
**    message           The dunning message to print if the user did not
**                      press a key in time
**
** Return value:
**    BOOLean status as to whether there is a key waiting or not.
**
** Notes:
**
*******************************************************************************/
BOOL getch_timeout (INT4 time_limit, char *message)
{
   BOOL timeout = FALSE;
   INT4 time;
   INT2 secs;
   static INT2 last_secs = -1;

   if (Timer_on) {
      /*
      ** Select the font that this is to be printed in.
      */
      font_select (TIMER_FONT);

      /*
      ** Convert the time limit from seconds to ticks.
      */
      time_limit *= CLOCKS_PER_SEC;

      /*
      ** Start the timer going.  Do not reset it, though.  This way
      ** this routine can be called repeatedly to see if the time
      ** is up yet, but to still allow for keystroke processing.
      */
      timer (TIMER_START);

      do {
         time = timer (TIMER_REPORT);

         /*
         ** Make the division round off instead of truncate.
         */
         secs = (time_limit - time + (CLOCKS_PER_SEC >> 1)) / CLOCKS_PER_SEC;

         /*
         ** Only update the time if it has changed.  Do not print
         ** the time if it is negative.  It can go negative if the
         ** user goes into the options menu while he is being timed
         ** and stays in there until after his time runs out.
         */
         if (secs < 0) {
            secs = 0;
         }
         if (secs != last_secs) {
            /*
            ** Tell the user how long they have left.
            */
            display_timer (UPDATE, secs);
            last_secs = secs;
         }

         if (time > time_limit) {
            /*
            ** The player timed out.
            */
            player_notify (message, FALSE, 0, 0);
            timeout = TRUE;
         }
      } while ((!inp_kbhit ()) && !timeout);
   }
   else {
      while (!inp_kbhit ());
   }

   return (timeout);
}


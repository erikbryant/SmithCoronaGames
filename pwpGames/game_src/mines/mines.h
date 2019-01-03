/*****************************************************************************
**
**  Module Name:
**      Mines Alive!
**  
**  Description:
**      This module contains many of the macro and types definitions.
**
**  Exported Units:
**
**  Local Units:
**      
**  Exported Data:
**
**  Notes:
**      none
**
**  Change Log:
**      Date            Description of Change
**      ----            ---------------------
**      04/24/92        Version 1.0 
**
**
*****************************************************************************/

#ifndef mines_h
#define mines_h

#include "window.h"
#include "splash.h"

   #include "varstab.j"

/*
** If you want to be able to deposit mines and look under tiles
** then define DEBUG.
*/
#define DEBUG          0

/*
** This is the name of the program.  APP_NAME goes on the splash screen.
** TITLE_NAME goes on the title bar.
*/
#define APP_NAME       "Mines Alive!"
#ifdef LCD
/*
   #define TITLE_NAME  "M   \\nI   \\nN  A\\nE \\cwi\\cb L\\cwi\\cb\\nS \\cwi\\cb I\\cwi\\cb\\n   V\\n   E"
*/
   #define TITLE_NAME  "M   \\nI   \\nN  A\\n\\cwi\\cbE \\cwik\\cbL\\cwi\\cb\\nS \\cwi\\cb I\\cwi\\cb\\n   V\\n   E"
#else
   #define TITLE_NAME  APP_NAME
#endif

/*
** Different messages to the user during the game.
*/
#define PAUSE_MSG   "Game paused\\nPress Select + P"
#define QUIT_MSG    "Are you sure\\nyou want to quit?\\n\\uY\\ues or \\uN\\uo"
#define LOSE_MSG    "Bang!\\nGame Over\\nPress RETURN"
#define WIN_MSG     "Congratulations,\\nyou won!\\nPress RETURN"

/*
** Everything printed by this game is in the same font.  Set the value
** to increase the leading by so that the menus are spaced out well.
*/
#ifdef LCD
   #define MINES_FONT           "Helvetica 12"
   #define LEADING_ADJUSTMENT   2
#else
   #define MINES_FONT           "Helvetica 30"
   #define LEADING_ADJUSTMENT   12
#endif

/*
** Skill levels available.
*/
#define SKILL_BEGINNER 0
#define SKILL_NOVICE   1
#define SKILL_EXPERT   2

/*
** This is the board that will be displayed when the game is 
** started.
*/
#define FIRST_BOARD 0

/*
** The size of a board.  This includes the "dead" space around
** the edge of the board.
*/
#ifdef LCD
   #define MAX_BOARD_HEIGHT 12
   #define MAX_BOARD_WIDTH  28
#else
   #define MAX_BOARD_HEIGHT 25
   #define MAX_BOARD_WIDTH  30
#endif

/*
** Since there is a banner across the top of the screen the
** MIN_Y value is greater.
**
** On the LCD the titles are on the sides, not the top.  Update
** the X values.
*/
#ifdef LCD
   #define NEW_MIN_X (MIN_X + LEFT_ADJUSTMENT)   /* From window.h */
   #define NEW_MAX_X (MAX_X - RIGHT_ADJUSTMENT)  /* From window.h */
   
   #define NEW_MIN_Y MIN_Y
   #define NEW_MAX_Y MAX_Y
#else
   #define NEW_MIN_X MIN_X                       
   #define NEW_MAX_X MAX_X

   #define NEW_MIN_Y (MIN_Y + TITLE_ADJUSTMENT)  /* From window.h */
   #define NEW_MAX_Y MAX_Y                       
#endif

/*
** Coordinates on the screen that bound the playing field.  Add one
** to the X coords for the LCD to line the pixmaps up.
*/
#define BOARD_MIN_X (NEW_MIN_X + 1)
#ifdef LCD
   #define BOARD_MAX_X (4 * ((NEW_MAX_X - NEW_MIN_X) / 5) + NEW_MIN_X)
#else
   #define BOARD_MAX_X (3 * ((NEW_MAX_X - NEW_MIN_X) / 4) + NEW_MIN_X)
#endif
#define BOARD_MIN_Y NEW_MIN_Y
#define BOARD_MAX_Y NEW_MAX_Y

/*
** Directions that the pointer can move.
*/
#define UP_LEFT  -1, -1
#define UP        0, -1
#define UP_RIGHT  1, -1
#define LEFT     -1,  0
#define RIGHT     1,  0
#define DN_LEFT  -1,  1
#define DOWN      0,  1
#define DN_RIGHT  1,  1

/*
** A byte on the game board has two nibbles, the lower of which
** has the color of the tile, the upper of which describes the tile.
**
**
** |---7----|---6----|---5---|---4----|---3-------2-------1-------0---|
** |        |        |       |C       |       |       |        |      |
** | T      | M      | M     | o      |       |       |        |      |
** |  e     |  a     |  i    |  v     |       |       |        |      |
** |   s    |   r    |   n   |   e    |       |     color      |      |
** |    t   |    k   |    e  |    r   |       |       |        |      |
** |     ?  |     e  |     ? |     e  |       |       |        |      |
** |        |      d |       |      d |       |       |        |      |
** |        |       ?|       |       ?|       |       |        |      |
** |--------|--------|-------|--------|-------------------------------|
**
*/

/*
** Operations on the color bits.
*/
#define COLOR_OFFSET      0
#define COLOR_MASK        (UCHAR) (0x0F << COLOR_OFFSET)
#define COLOR(s)          (((s & COLOR_MASK) >> COLOR_OFFSET) - 1)
                          
/*
** Operations on the covered bit.
*/
#define COVERED_OFFSET    4
#define COVERED_MASK      (UCHAR) (1 << COVERED_OFFSET)
#define COVERED(s)        ((s & COVERED_MASK) >> COVERED_OFFSET)
#define COVERED_SET(s)    (s |=  COVERED_MASK)
#define COVERED_UNSET(s)  (s &= ~COVERED_MASK)
#define COVERED_TOGGLE(s) (s ^=  COVERED_MASK)

/*
** Operations on the mine bit.
*/
#define MINE_OFFSET       5
#define MINE_MASK         (UCHAR) (1 << MINE_OFFSET)
#define MINE(s)           ((s & MINE_MASK) >> MINE_OFFSET)
#define MINE_SET(s)       (s |=  MINE_MASK)
#define MINE_UNSET(s)     (s &= ~MINE_MASK)
#define MINE_TOGGLE(s)    (s ^=  MINE_MASK)

/*
** Operations on the marked bit.
*/
#define MARKED_OFFSET     6
#define MARKED_MASK       (UCHAR) (1 << MARKED_OFFSET)
#define MARKED(s)         ((s & MARKED_MASK) >> MARKED_OFFSET)
#define MARKED_SET(s)     (s |=  MARKED_MASK)
#define MARKED_UNSET(s)   (s &= ~MARKED_MASK)
#define MARKED_TOGGLE(s)  (s ^=  MARKED_MASK)

/*
** Operations on the test bit.
*/
#define TEST_OFFSET       7
#define TEST_MASK         (UCHAR) (1 << TEST_OFFSET)
#define TEST(s)           ((s & TEST_MASK) >> TEST_OFFSET)
#define TEST_SET(s)       (s |=  TEST_MASK)
#define TEST_UNSET(s)     (s &= ~TEST_MASK)
#define TEST_TOGGLE(s)    (s ^=  TEST_MASK)

/*
** How many mines does a given square have bordering it?
*/
#define NEIGHBORS(t,y,x)  (MINE (t[y-1][x-1]) + \
                           MINE (t[y-1][x  ]) + \
                           MINE (t[y-1][x+1]) + \
                           MINE (t[y  ][x-1]) + \
                           MINE (t[y  ][x+1]) + \
                           MINE (t[y+1][x-1]) + \
                           MINE (t[y+1][x  ]) + \
                           MINE (t[y+1][x+1]))

/*
** Define a tile that is not part of the board.  Make it be not a mine
** so that the MINE macro does not have to see if the tile it is looking
** at is on the board or not.
*/
#define NO_TILE           (0xFF & ~MINE_MASK)

/*
** Define the different types of markers there can be on a tile.
*/
#define MINE_MARKER 0
#define TEST_MARKER 1

/*
** Define the boards.
*/
/*
#include "pixmaps.h"
*/

/*
** Set up parameters for the menuing windows.
*/
#define MAIN_MENU_TEXT    "\\uN\\uew\\n\\uP\\uause\\n\\uO\\uptions\\n\\uH\\uelp\\n\\uQ\\uuit"
#define OPTIONS_MENU_TEXT "\\uB\\ueginner\\n\\uN\\uovice\\n\\uE\\uxpert\\n\\uP\\uicture:\\n"

/*
** There is not enough room on the LCD screen to do bevels.
*/
#ifdef LCD
   #define BORDER_WIDTH      0
   #define FLAT_WIDTH        0
#else
   #define BORDER_WIDTH      3
   #define FLAT_WIDTH        5
#endif

/*
** Set up the spacing for the menus.
*/
#ifdef LCD
   #define MENU_SPACING      4
   #define MENU_WIDTH        50
   #define MENU_HEIGHT       80
#else
   #define MENU_SPACING      5
   #define MENU_WIDTH        126
   #define MENU_HEIGHT       230
#endif

/*
** The window that displays the time and level is much smaller than the
** options menu.
*/
#define STATUS_HEIGHT     (MENU_HEIGHT >> 1)

/*
** "Stick" the two windows together and center them on the screen.
*/
#define TOTAL_HEIGHT      (MENU_HEIGHT + MENU_SPACING + STATUS_HEIGHT)

#define MENU_X            (BOARD_MAX_X + ((NEW_MAX_X - BOARD_MAX_X - MENU_WIDTH) >> 1))
#define MENU_Y            (((NEW_MAX_Y - NEW_MIN_Y + 1 - TOTAL_HEIGHT) >> 1) + NEW_MIN_Y)

/*
** Dimensions of the whole status window.
*/
#define STATUS_WIDTH      MENU_WIDTH
#define STATUS_X          MENU_X
#define STATUS_Y          (MENU_Y + MENU_HEIGHT + MENU_SPACING)

/*
** Dimensions of the level sub-window.
*/
#define LEVEL_WIDTH       MENU_WIDTH
#define LEVEL_HEIGHT      (STATUS_HEIGHT >> 1)
#define LEVEL_X           MENU_X
#define LEVEL_Y           STATUS_Y

/*
** Dimensions of the time sub-window.
*/
#define TIME_WIDTH        MENU_WIDTH
#define TIME_HEIGHT       (LEVEL_HEIGHT + 1)
#define TIME_X            LEVEL_X
#define TIME_Y            (LEVEL_Y + LEVEL_HEIGHT)

/*
** For calls to bevel_window there needs to be some extra space
** around the text.
*/
#ifdef LCD
   #define EXTRA_SPACE    20
#else
   #define EXTRA_SPACE    40
#endif

/*
** The functions that display the menus are parameter driven.
*/
#define DISPLAY_WINDOW  0
#define UPDATE_TIME     1
#define UPDATE_LEVEL    2

/*
** The timer can be started, stopped, or reset.  It will also tell
** you the elapsed time if you ask.
*/
#define TIMER_RESET     0
#define TIMER_START     1
#define TIMER_STOP      2
#define TIMER_REPORT    3

/*
** Define macros for accessing the clock
*/
   #define TICKS         ((*(unsigned long *)vtab[VT_ticks]) / 63)
   #define POINTER_TICKS ((*(unsigned long *)vtab[VT_ticks]) / 31)

/*
** Define a seed for the random number generator.
*/
#define ANY_NUMBER  ((INT2) (TICKS & 0x0000FFFF))

/*
** States that the game can be in.
*/
#define LOSE    0
#define WIN     1
#define NEUTRAL 2

/*
** Status values that the routines can return.
*/
#define MINES_NOT_MOVED 1
#define MINES_SUCCESS   0
#define MINES_ERROR    -1

/*
** Define stack operations.  These stacks are implemented with
** arrays, not dynamic memory allocation.  Each PUSH puts two values
** on the stack and each POP takes two values off of the stack.
*/
#define BUFF_LEN (MAX_BOARD_HEIGHT * MAX_BOARD_WIDTH)

typedef struct list {
   UINT1 data[BUFF_LEN][2];
   INT2  head; 
   INT2  tail; 
} QUEUE;

/*
** Different operations on the stack.
*/
typedef enum manage_type { ALLOCATE, DEALLOCATE, INIT, PUSH, POP,
                           EMPTY, IN } MANAGE_TYPE;

/*
** Convenience macros for buffer management.
*/
#define ALLOC_BUFF()      manage_buffer (ALLOCATE,     0,   0)
#define DEALLOC_BUFF()    manage_buffer (DEALLOCATE,   0,   0)
#define INIT_BUFF()       manage_buffer (INIT,         0,   0)
#define IN_BUFF(a,b)      manage_buffer (IN,         (a), (b))
#define PUSH_BUFF(a,b)    manage_buffer (PUSH,       (a), (b))
#define POP_BUFF(a,b)     manage_buffer (POP,        (a), (b))
#define EMPTY_BUFF()      manage_buffer (EMPTY,        0,   0)

/*
** Given the (x1, y1) coordinate on the screen and the (x2, y2) 
** coordinate on the board, plot the given pixmap.
*/
#define PLOT(y1,x1,y2,x2,p) gr_pixmap ((x1) + (x2-1) * gr_pixmap_width  (p), \
                                       (y1) + (y2-1) * gr_pixmap_height (p), \
                                       (p),                                  \
                                       GROP_SRC)

/*
** Store where the pointer is on the board.
*/
typedef struct game_pointer {
   UINT1 board_x;
   UINT1 board_y;
} GAME_POINTER;

/*
** Define the structure that holds the board being played.
*/
#define NAME_LEN 10

typedef struct game_board {
   UCHAR name[NAME_LEN];
   INT2  screen_x;
   INT2  screen_y;
   UINT1 start_x;
   UINT1 start_y;
   UINT1 board_number;
   UINT1 level;
   UINT1 width;
   UINT1 height;
   INT2  mines[3];
   UCHAR board[MAX_BOARD_HEIGHT][MAX_BOARD_WIDTH];
} GAME_BOARD;

/*
** The boards in "storage" are packed to save space.
*/
typedef struct packed_board {
   UCHAR name[NAME_LEN];
   UINT1 start_x;
   UINT1 start_y;
   UINT1 width;
   UINT1 height;
   INT2  mines[3];
   UCHAR board[MAX_BOARD_HEIGHT][MAX_BOARD_WIDTH >> 1];
} PACKED_BOARD;

/*
** Function prototypes.
*/
void init_board (GAME_BOARD *board, GAME_POINTER *pointer, 
                 UINT1 board_number, UINT1 level);
void display_board (GAME_BOARD *board, GAME_POINTER *pointer, BOOL wipe_screen);
void redisplay_screen (GAME_BOARD *board, GAME_POINTER *pointer);
void display_pointer (GAME_POINTER *pointer, GAME_BOARD *board, BOOL on);
INT1 move_pointer (GAME_BOARD *board, GAME_POINTER *pointer, 
                   INT1 delta_x, INT1 delta_y);
void blink_pointer (GAME_POINTER *pointer, GAME_BOARD *board);
INT4 timer (INT1 op);
void display_mines (GAME_BOARD *board);
INT1 expose_tile (GAME_BOARD *board, GAME_POINTER *pointer);
void mark_tile (GAME_BOARD *board, GAME_POINTER *pointer, INT1 type);
void display_status (INT1 op, INT1 level);
void display_main_menu (BOOL text_only);
void display_options_menu (UCHAR *picture, BOOL picture_only);
void process_options (GAME_BOARD *board, GAME_POINTER *pointer);

#endif


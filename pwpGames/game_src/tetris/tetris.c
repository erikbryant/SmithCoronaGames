#define BOARD_WIDTH                 10      /* Width of Tetris board in blocks */

#ifndef LCD
    #define BOARD_HEIGHT            20      /* Height of Tetris board in blocks
                                               for the CRT version. */
#endif

#ifdef LCD
    #define BOARD_HEIGHT            15      /* Height of Tetris board in blocks
                                                for the LCD version. */
#endif

#define ON                          "On"    /* Option is enabled */
#define OFF                         "Off"   /* Option is disabled */
#define NUMBER_PIECES               7       /* Number of different Tetris pieces */
#define EMPTY                       NUMBER_PIECES /* Value of an empty space
                                                     on the game board. */
#define MAX_SKILL_LEV               10
#define MAX_START_LEV               MAX_SKILL_LEV
#define EVENT_TYPE                  UINT2

    #include "varstab.j"
    #define TICKS                   (*(unsigned long *)vtab[VT_ticks])
    #define TIMER_NOW               TICKS
    #define ONE_SECOND              62.0


#include <stdlib.h>
#include "inp.h"
#include "gr.h"
#include "helplib.h"

#ifndef LCD
    #include "tetcrt.hlp"
#endif

#ifdef LCD
    #include "tetlcd.hlp"
#endif

#include "sc_def.h"
#include "keys.h"
#include "fontlib.h"
#include "window.h"
#include "splash.h"

#define HI_LITED                    TRUE    /* 1 */
#define NORMAL                      FALSE   /* 0 */

#define ANY_NUMBER                  (INT2)TIMER_NOW

#define MAXX	                    (gr_scr_width()-1)
#define MAXY	                    (gr_scr_height()-1)

#ifndef LCD
    #define BLOCK_HEIGHT            20
    #define BLOCK_WIDTH             BLOCK_HEIGHT
    #define NUMBER_BLOCKS_HI        20
    #define NUMBER_BLOCKS_WIDE      10
#endif

#ifdef LCD
    #define BLOCK_HEIGHT            8
    #define BLOCK_WIDTH             BLOCK_HEIGHT
    #define NUMBER_BLOCKS_HI        15
    #define NUMBER_BLOCKS_WIDE      10
#endif

#ifndef LCD
    #define BORDER_WIDTH            3
#endif

#ifdef LCD
    #define BORDER_WIDTH			    4
    #define PIECE_WINDOW_BORDER_WIDTH   1
#endif

#ifndef LCD
    #define WINDOW_HEIGHT           ((MAXY / 3) <<1)
#endif

#ifdef LCD
    #define WINDOW_HEIGHT           (MAXY - 19)
#endif

#define WINDOW_WIDTH                (MAXX / 5)
#define PLAYING_AREA_HEIGHT         (BLOCK_HEIGHT * NUMBER_BLOCKS_HI)
#define PLAYING_AREA_WIDTH          (BLOCK_WIDTH * NUMBER_BLOCKS_WIDE)

#ifndef LCD
    #define GAME_BOARD_HEIGHT       (PLAYING_AREA_HEIGHT + (BORDER_WIDTH <<1))
    #define GAME_BOARD_WIDTH        (PLAYING_AREA_WIDTH + (BORDER_WIDTH <<1))
#endif

#ifdef LCD
    #define GAME_BOARD_HEIGHT       (PLAYING_AREA_HEIGHT\
                                    + (PIECE_WINDOW_BORDER_WIDTH <<1))

    #define GAME_BOARD_WIDTH        (PLAYING_AREA_WIDTH\
                                    + (PIECE_WINDOW_BORDER_WIDTH <<1))
#endif

#ifndef LCD
    #define PANE_WIDTH              5
    #define FRAME_WIDTH             ((BORDER_WIDTH << 2) + (PANE_WIDTH <<1))
    #define OUTER_BOARD_HEIGHT      (PLAYING_AREA_HEIGHT + FRAME_WIDTH)
    #define OUTER_BOARD_WIDTH       (PLAYING_AREA_WIDTH + FRAME_WIDTH)
    #define OUT_WINDOW_HEIGHT       (WINDOW_HEIGHT + FRAME_WIDTH)
    #define OUT_WINDOW_WIDTH        (WINDOW_WIDTH + FRAME_WIDTH)
    #define IN_WINDOW_HEIGHT        (WINDOW_HEIGHT + (BORDER_WIDTH <<1))
    #define IN_WINDOW_WIDTH         (WINDOW_WIDTH + (BORDER_WIDTH <<1))
#endif

#define NUMBER_H_GAPS               4

#ifndef LCD
    #define GAP_WIDTH               ((MAXX - OUTER_BOARD_WIDTH\
                                    - (OUT_WINDOW_WIDTH <<1))/ NUMBER_H_GAPS)
#endif

#ifdef LCD
    #define GAP_WIDTH               ((MAXX - GAME_BOARD_WIDTH\
                                    - (WINDOW_WIDTH <<1)\
                                    + (LEFT_ADJUSTMENT + RIGHT_ADJUSTMENT))\
                                    / NUMBER_H_GAPS)
#endif

#define SCORE_START_X               GAP_WIDTH
#define MENU_START_X                (MAXX - WINDOW_WIDTH - (GAP_WIDTH))

#ifndef LCD
    #define MENU_START_Y            (TITLE_ADJUSTMENT\
                                    + ((MAXY - TITLE_ADJUSTMENT -1) >>1)\
                                    - (WINDOW_HEIGHT >>1))

    #define SCORE_START_Y           MENU_START_Y
#endif

#ifdef LCD
    #define MENU_START_Y			((MAXY - WINDOW_HEIGHT) >> 1)
    #define SCORE_START_Y           MENU_START_Y
#endif

#ifndef LCD
    #define OUT_MENU_START_X        (MENU_START_X - (BORDER_WIDTH <<1)\
                                    - PANE_WIDTH)

    #define OUT_MENU_START_Y        (MENU_START_Y - (BORDER_WIDTH <<1)\
                                    - PANE_WIDTH)

    #define IN_MENU_START_X         (MENU_START_X - BORDER_WIDTH)
    #define IN_MENU_START_Y         (MENU_START_Y - BORDER_WIDTH)
    #define OUT_SCORE_START_X       (SCORE_START_X - (BORDER_WIDTH <<1)\
                                    - PANE_WIDTH)

    #define OUT_SCORE_START_Y       (SCORE_START_Y - (BORDER_WIDTH <<1)\
                                    - PANE_WIDTH)
#endif

#define IN_SCORE_START_X            (SCORE_START_X - BORDER_WIDTH)
#define IN_SCORE_START_Y            (SCORE_START_Y - BORDER_WIDTH)

#ifndef LCD
    #define OUTER_BOARD_START_X     (MAXX - OUTER_BOARD_WIDTH\
                                    - (GAP_WIDTH <<1) - OUT_WINDOW_WIDTH)

    #define OUTER_BOARD_START_Y     (((MAXY - TITLE_ADJUSTMENT) >>1) + TITLE_ADJUSTMENT\
                                    - (OUTER_BOARD_HEIGHT >>1))

    #define BOARD_START_X           (OUTER_BOARD_START_X + BORDER_WIDTH\
                                    + PANE_WIDTH)

    #define BOARD_START_Y           (OUTER_BOARD_START_Y + BORDER_WIDTH\
                                    + PANE_WIDTH)

    #define AREA_START_Y            (BOARD_START_Y + BORDER_WIDTH)
    #define AREA_START_X            (BOARD_START_X + BORDER_WIDTH)
#endif

#ifdef LCD
    #define AREA_START_X            ((MAXX >>1) - (GAME_BOARD_WIDTH >>1))
    #define BOARD_START_X           (AREA_START_X - PIECE_WINDOW_BORDER_WIDTH)
    #define BOARD_START_Y           ((MAXY + 1 - GAME_BOARD_HEIGHT) >> 1)
    #define AREA_START_Y            (BOARD_START_Y + PIECE_WINDOW_BORDER_WIDTH)
    #define SHADOW_THICKNESS        3
#endif

#ifndef LCD
    #define PIECE_WINDOW_HEIGHT     (4 * BLOCK_HEIGHT)
    #define PIECE_WINDOW_START_Y    (SCORE_START_Y + WINDOW_HEIGHT\
                                    - PIECE_WINDOW_HEIGHT)
#endif

#ifdef LCD
    #define PIECE_WINDOW_HEIGHT     (3 * BLOCK_HEIGHT)
    #define PIECE_WINDOW_START_Y    (SCORE_START_Y\
                                    + (WINDOW_HEIGHT - PIECE_WINDOW_HEIGHT))
#endif

#define PIECE_WINDOW_CENTER_X       (SCORE_START_X + (WINDOW_WIDTH >>1))
#define PIECE_WINDOW_CENTER_Y       (PIECE_WINDOW_START_Y\
                                    + (PIECE_WINDOW_HEIGHT >>1))

#ifndef LCD
    #define SCORE_ENTRY_HEIGHT      ((WINDOW_HEIGHT - (BORDER_WIDTH <<1)\
                                    - (PIECE_WINDOW_HEIGHT)) / 6)
#endif

#ifdef LCD
    #define SCORE_ENTRY_HEIGHT      ((WINDOW_HEIGHT - (PIECE_WINDOW_HEIGHT)) / 6)
#endif

#ifndef LCD
    #define SCORE_NUM_START_Y       SCORE_START_Y + BORDER_WIDTH\
                                    + SCORE_ENTRY_HEIGHT
#endif

#ifdef LCD
    #define SCORE_NUM_START_Y       SCORE_START_Y + SCORE_ENTRY_HEIGHT
#endif

#define LEVEL_TEXT_START_Y          SCORE_NUM_START_Y + SCORE_ENTRY_HEIGHT
#define LEVEL_NUM_START_Y           LEVEL_TEXT_START_Y + SCORE_ENTRY_HEIGHT
#define LINES_TEXT_START_Y          LEVEL_NUM_START_Y + SCORE_ENTRY_HEIGHT
#define LINES_NUM_START_Y           LINES_TEXT_START_Y + SCORE_ENTRY_HEIGHT

#ifdef LCD
    #define OPTIONS_ENTRY_HEIGHT    ((WINDOW_HEIGHT - (BORDER_WIDTH <<1)) /6)
#else
    #define OPTIONS_ENTRY_HEIGHT    ((WINDOW_HEIGHT - (BORDER_WIDTH <<1)) /7)
#endif

#define SKILL_TEXT_START_Y          (MENU_START_Y + BORDER_WIDTH)
#define SKILL_NUM_START_Y           (SKILL_TEXT_START_Y + OPTIONS_ENTRY_HEIGHT)
#define START_TEXT_START_Y          (SKILL_NUM_START_Y + OPTIONS_ENTRY_HEIGHT)

#ifdef LCD
    #define START_NUM_START_Y       (START_TEXT_START_Y + OPTIONS_ENTRY_HEIGHT)
#else
    #define START_NUM_START_Y       (START_TEXT_START_Y + (OPTIONS_ENTRY_HEIGHT <<1))
#endif

#define PIECE_PRE_TEXT_START_Y      (START_NUM_START_Y + OPTIONS_ENTRY_HEIGHT)
#define PIECE_PRE_NUM_START_Y       (PIECE_PRE_TEXT_START_Y\
                                    + OPTIONS_ENTRY_HEIGHT)

#ifndef LCD
    #define HILITE_FACTOR           4
#endif

#ifdef LCD
    #define HILITE_FACTOR           6
#endif

#define ROW_DELAY                   200

#ifndef LCD
   #define MENU_FONT                "Helvetica 30"
   #define DEFAULT_FONT             MENU_FONT
#endif

#ifdef LCD
    #define MENU_FONT               "Helvetica 16"
    #define DEFAULT_FONT            "Helvetica 12"
#endif

#ifndef LCD
    #define MENU_TEXT               "\\uN\\uew\\n\\n\\uP\\uause \\n\\n\\u\
O\\uptions\\n\\n\\u\
H\\uelp\\n\\n\\uQ\\uuit"
#endif

#ifdef LCD
    #define MENU_TEXT               "\\uN\\uew\\n\\uP\\uause\\n\\uO\\uptions\\n\
\\uH\\uelp\\n\\uQ\\uuit"
#endif

#define PAUSED_MESSAGE              "Game is\\npaused\\n\\nPress\\nselect + P\
\\nto resume"
#define SKILL_TEXT                  "\\uL\\uevel"

#define END_GAME_TEXT               "Game Over\\nPress Enter\\nto continue"

#ifndef LCD
    #define START_TEXT              "\\uS\\utart\\nRows"
#endif

#ifdef LCD
    #define START_TEXT              "\\uS\\utart"
#endif

#define PREV_TEXT                   "\\uP\\ureview"

#define SCORE_FONT                  DEFAULT_FONT
#define SCORE_TEXT                  "Score:"
#define LEVEL_TEXT                  "Level:"
#define LINES_TEXT                  "Lines:"

/*
** Define each of the keys that control the game.
*/

#define KEY_ROTATE                  '8'
#define KEY_LEFT                    '4'
#define KEY_RIGHT                   '6'
#define KEY_DROP                    '2'
#define KEY_SPACE                   ' '

#define KEY_MENU_START              ('n' | SELECT_KEY)
#define KEY_MENU_PAUSE              ('p' | SELECT_KEY)
#define KEY_MENU_HELP               ('h' | SELECT_KEY)
#define KEY_MENU_OPTIONS            ('o' | SELECT_KEY)
#define KEY_MENU_QUIT               ('q' | SELECT_KEY)

#ifdef LCD
    #define  PWP_HELP_KEY           CODE_KEY|'1'
#else
    #define  PWP_HELP_KEY           HELP_KEY
#endif

#define HELP_NUM_KEY                '0'
#define KEY_SKILL_LEVEL             ('l' | SELECT_KEY)
#define KEY_START_ROWS              ('s' | SELECT_KEY)
#define KEY_PIECE_PRE               ('p' | SELECT_KEY)

#define EXIT_GAME_KEY               ('y' | SELECT_KEY)
#define RETURN_GAME_KEY             ('n' | SELECT_KEY)
#define EXIT_TETRIS_KEY             255

#define TIMER_KEY                   TRUE

#define TIMER_MAX                   0xFFFFFFFF

#define TIMER_LEVEL                 (ONE_SECOND /100.0)


/******************************************************************************
***         Variable definitions            ***********************************
******************************************************************************/

typedef struct board_pos {
   char x;                     /* X coordinate of position */
   char y;                     /* Y coordinate of position */
} BOARD_POS;

/*
** There are seven unique types of blocks used in Tetris.  Each of
** these blocks has a unique pattern.  Four of the blocks are connected
** together to construct a Tetris piece.
*/

typedef struct blocks_piece {
   UCHAR block_style;           /* Index in array that designates a pixel
                                   map for all block within piece */
   BOARD_POS start_position;    /* Starting position of piece on board */
   BOARD_POS block_map[4][3];   /* Positions of blocks which makeup piece */
   float h_center_offset;       /* Horizontal adjustment of the pivot
                                   point. It is  measured in blocks/fractions
                                   of a block which must be added to center
                                   the piece*/
   float v_center_offset;       /* Horizontal adjustment of the pivot
                                   point. It is  measured in blocks/fractions
                                   of a block which must be added to center
                                   the piece*/
} TETRIS_PIECE;

/*
** Each active Tetris piece has certain characteristics that determine score
** and how the piece reacts to the current state of the game board.
*/

typedef struct active_blocks_piece {
   BOARD_POS position;      /* Position of piece on board */
   UCHAR rotation;          /* Orientation of piece on board */
   UCHAR number_moves;      /* # of moves/rotates by the current piece */
   UCHAR drop_distance;     /* Distance piece fell if dropped */
   UCHAR rows_cleared;      /* # of rows cleared when piece hits bottom */
   TETRIS_PIECE *data;      /* Pointer to piece's description */
} ACTIVE_TETRIS_PIECE;


/*
** Tetris has three game options. The value of skill_lev and start_lev is it's
** current setting while the value of piece_pre is either ON ,OFF.
*/

typedef struct options_type {
   UCHAR skill_lev;         /* Current skill level of game */
   UCHAR start_lev;         /* Current Starting level of game */
   char piece_pre[4];       /* Status of piece preview ON or OFF */
}OPTIONS_TYPE;


/*
** The score,timer length, and last timer tick all affect the flow of the game.
*/

typedef struct game_stats_type {
   INT4  tot_score;         /* Total score of the game in progress */
   UCHAR skill_lev_changed; /* Skill level was changed when piece landed */
   INT4  timer_length;      /* Length of time between timer ticks */
   INT4  last_tick;         /* Value of last timer tick */
}GAME_STATS_TYPE;

/*
** The Tetris board is a two dimensional array that represents spaces a block
** may fill on the game board.
*/

typedef struct game_board{
   UCHAR     blocks_board[BOARD_HEIGHT][BOARD_WIDTH]; /* Pix types */
   UCHAR     peak;                                   /* y-value of highest */
}GAME_BOARD;                                        /* non-empty row */


/*
** This structure maintains row statistics for the TETRIS game board.
*/

typedef struct row_stats_type {
   UCHAR tot_rows_cleared;  /* Total # of rows cleared during game */
   UCHAR more_rows_cleared; /* Additional rows cleared when piece landed */
}ROW_STATS_TYPE;


/******************************************************************************
***         Variable declarations            **********************************
******************************************************************************/
/*
** Data structures for each of the Tetris pieces.
*/

TETRIS_PIECE Tetris_piece_1 = {          /* Square-Shaped piece */
   0,
   {(BOARD_WIDTH/2) -1, -1},
   {
	  {{ 1, 0}, { 0, 1}, { 1, 1}},
      {{ 1, 0}, { 0, 1}, { 1, 1}},
      {{ 1, 0}, { 0, 1}, { 1, 1}},
      {{ 1, 0}, { 0, 1}, { 1, 1}}
   },
   -1,
   -1
};

TETRIS_PIECE Tetris_piece_2 = {          /* Rectangle-shaped piece */
   1,
   {(BOARD_WIDTH/2), -1},
   {
      {{-2, 0}, {-1, 0}, { 1, 0}},
      {{ 0,-1}, { 0, 1}, { 0, 2}},
	  {{-2, 0}, {-1, 0}, { 1, 0}},
      {{ 0,-1}, { 0, 1}, { 0, 2}}
   },
   0,
   (-1.0/2.0)
};

TETRIS_PIECE Tetris_piece_3 = {          /* T-shaped piece */
   2,
   {(BOARD_WIDTH/2) - 1, -1},
   {
	  {{-1, 0}, { 1, 0}, { 0, 1}},
	  {{ 0,-1}, { 0, 1}, { 1, 0}},
	  {{-1, 0}, { 1, 0}, { 0,-1}},
	  {{-1, 0}, { 0, 1}, { 0,-1}}
   },
   (-1.0/2.0),
   -1
};

TETRIS_PIECE Tetris_piece_4 = {          /* Left L-shaped piece */
   3,
   {(BOARD_WIDTH/2) -1, -1},
   {
	  {{-1, 0}, { 1, 0}, {-1, 1}},
	  {{ 0,-1}, { 0, 1}, { 1, 1}},
	  {{-1, 0}, { 1, 0}, { 1,-1}},
	  {{-1,-1}, { 0,-1}, { 0, 1}}
   },
   (-1.0/2.0),
   -1
};

TETRIS_PIECE Tetris_piece_5 = {          /* Right L-shaped piece */
   4,
   {(BOARD_WIDTH/2) -1, -1},
   {
	  {{-1, 0}, { 1, 0}, { 1, 1}},
	  {{ 0,-1}, { 0, 1}, { 1,-1}},
	  {{-1, 0}, {-1,-1}, { 1, 0}},
	  {{ 0,-1}, { 0, 1}, {-1, 1}}
   },
   (-1.0/2.0),
   -1
};

TETRIS_PIECE Tetris_piece_6 = {          /* Left S-shaped piece */
   5,
   {(BOARD_WIDTH/2) -1, 0},
   {
	  {{-1,-1}, { 0,-1}, { 1, 0}},
	  {{-1, 0}, {-1, 1}, { 0,-1}},
	  {{-1,-1}, { 0,-1}, { 1, 0}},
	  {{-1, 0}, {-1, 1}, { 0,-1}}
   },
   (-1.0/2.0),
   0
};

TETRIS_PIECE Tetris_piece_7 = {          /* Right S-shaped piece */
   6,
   {(BOARD_WIDTH/2) -1, 0},
   {
	  {{-1, 0}, { 0,-1}, { 1,-1}},
	  {{ 0,-1}, { 1, 0}, { 1, 1}},
	  {{-1, 0}, { 0,-1}, { 1,-1}},
	  {{ 0,-1}, { 1, 0}, { 1, 1}}
   },
   (-1.0/2.0),
   0
};


/*
** These are the included PIXMAPs depicting a separate pattern for each piece
*/

PIXMAP Pix_1[] =
{
#ifndef LCD
#include "tetpc1.pix"
#endif

#ifdef LCD
#include "tetlc1.pix"
#endif
};

PIXMAP Pix_2[] =
{
#ifndef LCD
#include "tetpc2.pix"
#endif

#ifdef LCD
#include "tetlc2.pix"
#endif
};

PIXMAP Pix_3[] =
{
#ifndef LCD
#include "tetpc3.pix"
#endif

#ifdef LCD
#include "tetlc3.pix"
#endif
};

PIXMAP Pix_4[] =
{
#ifndef LCD
#include "tetpc4.pix"
#endif

#ifdef LCD
#include "tetlc4.pix"
#endif
};

PIXMAP Pix_5[] =
{
#ifndef LCD
#include "tetpc5.pix"
#endif

#ifdef LCD
#include "tetlc5.pix"
#endif
};

PIXMAP Pix_6[] =
{
#ifndef LCD
#include "tetpc6.pix"
#endif

#ifdef LCD
#include "tetlc6.pix"
#endif
};

PIXMAP Pix_7[] =
{
#ifndef LCD
#include "tetpc7.pix"
#endif

#ifdef LCD
#include "tetlc7.pix"
#endif
};


/*
** The Tetris piece table contains pointers to each of the Tetris game
** piece structures.
*/

TETRIS_PIECE *Tetris_pieces_table[NUMBER_PIECES] = {
   &Tetris_piece_1,
   &Tetris_piece_2,
   &Tetris_piece_3,
   &Tetris_piece_4,
   &Tetris_piece_5,
   &Tetris_piece_6,
   &Tetris_piece_7
};


/*
** The Tetris board array stores integers representing indices of the Pix_type
** array. Indexing the array with the integer from the game board will give
** you a pointer to a pixmap of the block displayed at each location on the
** Tetris board. Locations that don't contain blocks contain EMPTY.
*/

PIXMAP *Pix_type[NUMBER_PIECES] = {
   Pix_1,
   Pix_2,
   Pix_3,
   Pix_4,
   Pix_5,
   Pix_6,
   Pix_7
};

/*
** Pointers to the piece in play and its successor.
*/

ACTIVE_TETRIS_PIECE Current_piece;
ACTIVE_TETRIS_PIECE *Current_pc_ptr = &Current_piece;
ACTIVE_TETRIS_PIECE Next_piece;
ACTIVE_TETRIS_PIECE *Next_pc_ptr = &Next_piece;

/*
** Tetris game board structue.
*/

GAME_BOARD Game_board;

/*
** Game options structure.
*/

OPTIONS_TYPE Options;

/*
** Game statistics structure
*/

GAME_STATS_TYPE Game_stats;

/*
** The following report if a game is in progress or if paused.
*/

UCHAR Game_playing;   /* True if game is in progress, else false */
UCHAR Game_paused;    /* True if game is paused, else false */

/*
** Row statistics structure.
*/

ROW_STATS_TYPE Row_data;

/*
** Game name
*/
char Name[] ="Tetris \xAD";

#ifdef LCD
char Banner_name[] ="T\\nE\\nT\\nR\\nI\\nS";
#else
char Banner_name[] ="Tetris";
#endif

/*
** This array is the timer table used to calculate amount of time between timer
** ticks at a given skill level.
*/
UCHAR Timer_table[MAX_SKILL_LEV] = {

   0,
   29,
   44,
   54,
   63,
   71,
   78,
   84,
   89,
   93
};

/*
** Value of timer intervals before most recent change.
**
** 0,
** 22,
** 38,
** 50,
** 60,
** 68,
** 76,
** 84,
** 90,
** 94
*/

/* BEGIN TETRIS */
/* .  DISPLAY_SPLASH SCREEN */
/* .  SEED THE RANDOM NUMBER GENERATOR */
/* .  INITIALIZE GAME OPTIONS */
/* .  SET OPTIONS_CHANGED STATUS TO FALSE */
/* .  RECORD THE CURRENT SETTING OF THE GAME SKILL LEVEL */
/* .  CALL NEW_GAME */
/* .  CALL DISPLAY_GAME */
/* .  CALL GET_EVENT */
/* .  WHILE (THE_EVENT IS NOT QUIT) DO */
/* .  .  IF (GAME_PLAYING AND NOT GAME_PAUSED) THEN */
/* .  .  .  CASE BASED ON EVENT */
/* .  .  .  .  CASE THE_EVENT IS ROTATE_PIECE: */
/* .  .  .  .  .  CALL ROTATE_PIECE TO TRY TO ROTATE PIECE */
/* .  .  .  .  .  INCREMENT THE NUMBER OF MOVES BY 1 IF ROTATE IS SUCCESSFULL */
/* .  .  .  .  CASE THE_EVENT IS MOVE_PIECE_LEFT: */
/* .  .  .  .  .  CALL MOVE_PIECE_LEFT */
/* .  .  .  .  .  INCREMENT THE NUMBER OF MOVES BY 1 IF MOVE LEFT IS SUCCESSFULL */
/* .  .  .  .  .  END IF */
/* .  .  .  .  CASE THE_EVENT IS MOVE_PIECE_RIGHT: */
/* .  .  .  .  .  CALL MOVE_PIECE_RIGHT */
/* .  .  .  .  .  INCREMENT THE NUMBER OF MOVES BY 1 IF MOVE RIGHT IS SUCCESSFUL */
/* .  .  .  .  .  END IF */
/* .  .  .  .  CASE THE_EVENT IS TIMER_TICK: */
/* .  .  .  .  .  IF (NOT ABLE TO MOVE_PIECE_DOWN) THEN */
/* .  .  .  .  .  .  CALL PIECE_LANDED */
/* .  .  .  .  .  .  CALL DISPLAY_SCORE TO SHOW NEW SCORE */
/* .  .  .  .  .  .  RANDOMLY SELECT THE NEXT PIECE */
/* .  .  .  .  .  .  SWAP CURRENT AND NEXT PIECE */
/* .  .  .  .  .  .  NEXT PIECE IS LOADED WITH DATA FROM RANDOMLY SELECTED PIECE */
/* .  .  .  .  .  .  EMPTY KEYBOARD BUFFER */
/* .  .  .  .  .  .  IF (PIECE PREVIEW IS ON) THEN */
/* .  .  .  .  .  .  .  CALL DISPLAY PREVIEW PIECE */
/* .  .  .  .  .  .  END IF */
/* .  .  .  .  .  .  CALL INIT_DISPLAY_PIECE */
/* .  .  .  .  .  .  IF (NOT ABLE TO DISPLAY_NEXT_PIECE) THEN */
/* .  .  .  .  .  .  .  GAME_PLAYING IS FALSE */
/* .  .  .  .  .  .  .  DISPLAY GAME ENDING MESSAGE */
/* .  .  .  .  .  .  .  DISPLAY MAIN MENU */
/* .  .  .  .  .  .  END IF */
/* .  .  .  .  .  END IF */
/* .  .  .  .  CASE THE_EVENT IS DROP_PIECE: */
/* .  .  .  .  .  CALL DROP_PIECE */
/* .  .  .  .  .  CALL PIECE_LANDED */
/* .  .  .  .  .  CALL DISPLAY_SCORE */
/* .  .  .  .  .  RANDOMLY SELECT THE NEXT PIECE */
/* .  .  .  .  .  SWAP CURRENT AND NEXT PIECE */
/* .  .  .  .  .  NEXT PIECE IS LOADED WITH DATA FROM RANDOMLY SELECTED PIECE */
/* .  .  .  .  .  EMPTY KEYBOARD BUFFER */
/* .  .  .  .  .  IF (PIECE PREVIEW IS ON) THEN */
/* .  .  .  .  .  .  CALL DISPLAY PREVIEW PIECE */
/* .  .  .  .  .  END IF */
/* .  .  .  .  .  CALL INIT_DISPLAY_PIECE */
/* .  .  .  .  .  IF (NOT ABLE TO DISPLAY_NEXT_PIECE) THEN */
/* .  .  .  .  .  .  GAME_PLAYING IS FALSE */
/* .  .  .  .  .  .  DISPLAY GAME ENDING MESSAGE */
/* .  .  .  .  .  .  DISPLAY MAIN MENU */
/* .  .  .  .  .  END IF */
/* .  .  .  END CASE */
/* .  .  END IF */
/* .  .  CASE BASED ON EVENT */
/* .  .  .  CASE THE_EVENT IS MENU_START: */
/* .  .  .  .  IF ( GAME IS NOT PAUSED) */
/* .  .  .  .  .  RESET SKILL LEVEL */
/* .  .  .  .  .  CALL NEW_GAME */
/* .  .  .  .  .  CALL DISPLAY_SCORE_WINDOW */
/* .  .  .  .  .  CALL DISPLAY_BOARD */
/* .  .  .  .  .  RANDOMLY SELECT THE NEXT PIECE */
/* .  .  .  .  .  SWAP CURRENT AND NEXT PIECE */
/* .  .  .  .  .  NEXT PIECE IS LOADED WITH DATA FROM RANDOMLY SELECTED PIECE */
/* .  .  .  .  .  CALL INIT_DISPLAY_PIECE */
/* .  .  .  .  .  IF (PIECE PREVIEW IS ON) THEN */
/* .  .  .  .  .  .  CALL DISPLAY PREVIEW PIECE */
/* .  .  .  .  .  END IF */
/* .  .  .  .  .  SET GAME_PLAYING TO TRUE */
/* .  .  .  .  .  SET GAME_PAUSED TO FALSE */
/* .  .  .  .  END IF */
/* .  .  .  CASE THE_EVENT IS MENU_PAUSE: */
/* .  .  .  .  GAME_PAUSED = GAME_PLAYING AND (NOT GAME_PAUSED) */
/* .  .  .  .  IF (NOT GAME_PAUSED) THEN */
/* .  .  .  .  .  LAST_TICK = TIMER_NOW */
/* .  .  .  .  END IF */
/* .  .  .  .  IF (GAME_PLAYING) */
/* .  .  .  .  .  CALL DISPLAY_MAIN_MENU */
/* .  .  .  .  ELSE */
/* .  .  .  .  .  CALL DISPLAY_PAUSED_MESSAGE */
/* .  .  .  .  END IF */
/* .  .  .  CASE THE_EVENT IS MENU_HELP: */
/* .  .  .  .  IF (GAME_PLAYING) */
/* .  .  .  .  .  GAME PAUSED IS TRUE */
/* .  .  .  .  END IF */
/* .  .  .  .  CALL HELP_DISPLAY */
/* .  .  .  .  IF (NOT GAME_PLAYING) */
/* .  .  .  .  .  CALL NEW_GAME */
/* .  .  .  .  END IF */
/* .  .  .  .  CALL DISPLAY_GAME */
/* .  .  .  .  IF (GAME_PLAYING) */
/* .  .  .  .  .  CALL DISPLAY_PAUSED_GAME */
/* .  .  .  .  .  CALL DISPLAY_BOARD */
/* .  .  .  .  .  IF (CURRENT PIECE CAN BE DISPLAYED) */
/* .  .  .  .  .  .  DISPLAY CURRENT PIECE */
/* .  .  .  .  .  END IF */
/* .  .  .  .  .  IF (PIECE PREVIEW IS ON) */
/* .  .  .  .  .  .  CALL DISPLAY_PREVIEW_PIECE */
/* .  .  .  .  .  END IF */
/* .  .  .  CASE THE_EVENT IS MENU_OPTIONS: */
/* .  .  .  .  IF (NOT GAME_PAUSED) THEN */
/* .  .  .  .  .  CALL ALTER_OPTIONS */
/* .  .  .  .  END IF */
/* .  .  .  .  IF (OPTIONS HAVE BEEN CHANGED) THEN */
/* .  .  .  .  .  RESET SKILL LEVEL */
/* .  .  .  .  .  CALL NEW_GAME */
/* .  .  .  .  .  CALL DISPLAY_SCORE_WINDOW */
/* .  .  .  .  .  GAME_PLAYING IS FALSE */
/* .  .  .  .  .  GAME_PAUSED IS FALSE */
/* .  .  .  .  END IF */
/* .  .  .  .  CALL DISPLAY MAIN_MENU */
/* .  .  .  CASE THE_EVENT IS MENU_QUIT: */
/* .  .  .  .  CALL PROMPT_EXIT */
/* .  .  .  .  IF (KEYPRESS = QUIT_YES) THEN */
/* .  .  .  .  .  EVENT = QUIT */
/* .  .  .  .  END IF */
/* .  .  END CASE */
/* .  .  IF (EVENT != QUIT) THEN */
/* .  .  .  CALL GET_EVENT */
/* .  .  END */
/* .  END WHILE */
/* END TETRIS_MAIN */

EVENT_TYPE get_event();

void display_main_menu (void);
void display_options_menu (void);
void display_end_game (void);
void display_paused_game (void);
void display_preview_piece(void);
void display_score_window (void);
void display_board(GAME_BOARD *game_board);
void display_piece(ACTIVE_TETRIS_PIECE *current_piece,INT2 start_coord_x,INT2 start_coord_y);
void display_score(OPTIONS_TYPE *options,ACTIVE_TETRIS_PIECE *current_piece_ptr);
void display_game(void);

void new_game(UCHAR start_rows,UCHAR skill_lev, GAME_STATS_TYPE *game_stats);
void init_game_opts(OPTIONS_TYPE *options);

UCHAR alter_options(OPTIONS_TYPE *options);
UCHAR init_display_piece(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board);
UCHAR move_piece_left(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board);
UCHAR move_piece_right(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board);
UCHAR move_piece_down(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board);
UCHAR rotate_piece(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board);
UCHAR drop_piece(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board);
UCHAR piece_in_bounds(GAME_BOARD *game_board, ACTIVE_TETRIS_PIECE *moved_piece);
UCHAR is_valid_move(GAME_BOARD *game_board, ACTIVE_TETRIS_PIECE *moved_piece);
void piece_landed(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board);
INT4 update_score(OPTIONS_TYPE *options,ACTIVE_TETRIS_PIECE *current_piece);


app_main(void)

{
   EVENT_TYPE event;                /* Tetris game event */
   UCHAR random_piece;              /* Randomly selected next piece */
   UINT2 exit_choice;               /* Player's reply when prompted to quit */
   ACTIVE_TETRIS_PIECE *temp;       /* Temporary pointer used in a swap */
   UCHAR move_valid;                /* TRUE if valid move is attempted, else FALSE */
   UCHAR start_skill_lev;           /* Skill level that the last game began at */
   UCHAR options_changed;           /* TRUE if options were changed, else FALSE */

   srand(ANY_NUMBER);               /* Seed the random number generator */
   display_splash_scr_license(Name);
   init_game_opts(&Options);
   options_changed = FALSE;
   start_skill_lev = Options.skill_lev;
   new_game(Options.start_lev,Options.skill_lev,&Game_stats);
   display_game();
   event = get_event();

   while(event != EXIT_TETRIS_KEY)
   {
	  if (Game_playing && ! Game_paused)
	  {
		 switch (event)
		 {
			case KEY_ROTATE:
			case CURSUP_KEY:
      case '5':
      case ADVANCE_KEY:
			   Current_pc_ptr->number_moves +=
				  rotate_piece(Current_pc_ptr,&Game_board);
			   break;

			case KEY_LEFT:
			case CURSLEFT_KEY:
			   Current_pc_ptr->number_moves +=
				  move_piece_left(Current_pc_ptr,&Game_board);
			   break;

			case KEY_RIGHT:
			case CURSRIGHT_KEY:
			   Current_pc_ptr->number_moves +=
				  move_piece_right(Current_pc_ptr,&Game_board);
			   break;

			case TIMER_KEY:
			case KEY_DROP:
			case CURSDOWN_KEY:
			   if (!move_piece_down(Current_pc_ptr,&Game_board))
			   {
				  if (piece_in_bounds(&Game_board, Current_pc_ptr))
                  {
                     piece_landed(Current_pc_ptr,&Game_board);
                     display_score(&Options,Current_pc_ptr);
				     random_piece =(UCHAR)rand();
				     temp = Current_pc_ptr;
				     Current_pc_ptr = Next_pc_ptr;
				     Next_pc_ptr = temp;
				     Next_pc_ptr->data =
				         Tetris_pieces_table[random_piece % NUMBER_PIECES];
				     Next_pc_ptr->rotation =0;

                     event = get_event();
                     while (event != TIMER_KEY)
                     {
                        event = get_event();
                     }

				     Game_playing = init_display_piece(Current_pc_ptr,&Game_board);

				     if ((Options.piece_pre[1] == 'n')&&(Game_playing))
                         display_preview_piece();
                  }
                  else
                  {
                     Game_playing = FALSE;
                  }
               }
			   if (! Game_playing)
			   {
				  display_end_game();
                  Options.skill_lev = start_skill_lev;
			   }
			   break;


			case KEY_SPACE:

			   Current_pc_ptr->drop_distance =
			      drop_piece(Current_pc_ptr,&Game_board);
			   if (piece_in_bounds(&Game_board, Current_pc_ptr))
			   {
                  piece_landed(Current_pc_ptr,&Game_board);
                  display_score(&Options, Current_pc_ptr);
                  random_piece =(UCHAR)rand();
                  temp = Current_pc_ptr;
                  Current_pc_ptr = Next_pc_ptr;
                  Next_pc_ptr = temp;
                  Next_pc_ptr->data =
                      Tetris_pieces_table[random_piece % NUMBER_PIECES];
                  Next_pc_ptr->rotation =0;

                  while(inp_kbhit())
                      inp_getch();

                  Game_playing = init_display_piece(Current_pc_ptr,&Game_board);

                  event = get_event();
                  while (event != TIMER_KEY)
                  {
                     event = get_event();
                  }
               }
               else
               {
                  Game_playing = FALSE;
               }

               if (! Game_playing)
               {
			      display_end_game();
                  Options.skill_lev = start_skill_lev;
               }
               else
               {
				  if ((Options.piece_pre[1] == 'n')&&(Game_playing))
                     display_preview_piece();

               }
               break;

            default:
     	       break;
         }

      }

	  switch(event)
      {
         case KEY_MENU_START:
            if (!Game_paused)
            {
               Options.skill_lev = start_skill_lev;
               new_game(Options.start_lev,Options.skill_lev,&Game_stats);
               display_score_window();
            /*
            ** Clear the Tetris board.
            */
#ifdef LCD
               gr_rect(BOARD_START_X,BOARD_START_Y,
                       GAME_BOARD_WIDTH,GAME_BOARD_HEIGHT,
				       PIECE_WINDOW_BORDER_WIDTH,GR_BLACK,GR_WHITE);
#else
               gr_bevel_rect(BOARD_START_X,BOARD_START_Y,
                             GAME_BOARD_WIDTH,GAME_BOARD_HEIGHT,
			                 BORDER_WIDTH,GR_GRAY,BEVEL_IN);
#endif
               display_board(&Game_board);
               random_piece =(UCHAR)rand();
               Current_pc_ptr->data =
                  Tetris_pieces_table[random_piece % NUMBER_PIECES];
               random_piece =(UCHAR)rand();
               Next_pc_ptr->data =
                  Tetris_pieces_table[random_piece % NUMBER_PIECES];
               init_display_piece(Current_pc_ptr,&Game_board);
               Game_playing = TRUE;
			   Game_paused = FALSE;

			   if (Options.piece_pre[1] == 'n')
                  display_preview_piece();
            }
			break;

		 case KEY_MENU_PAUSE:
			Game_paused = Game_playing && (!Game_paused);
			if (! Game_paused)
			{
               Game_stats.last_tick = TIMER_NOW;

               if (Game_playing)
               {
                  display_main_menu();
               }
            }
            else
            {
               display_paused_game();
            }
			break;

		 case KEY_MENU_HELP:
		 case PWP_HELP_KEY:
#ifndef LCD
		 case HELP_NUM_KEY:
#endif
        if (Game_playing)
        {
           Game_paused = TRUE;
        }

        help_display(GAME_HELP);

        if (!Game_playing)
        {
           new_game(Options.start_lev,Options.skill_lev,&Game_stats);
        }

        display_game();

        if (Game_playing)
        {
           display_paused_game();
           display_board(&Game_board);
           move_valid = is_valid_move(&Game_board,Current_pc_ptr);

        if (move_valid)
        {
                 display_piece(Current_pc_ptr,
                    (Current_pc_ptr->position.x * BLOCK_WIDTH)
                              +AREA_START_X,
                    (Current_pc_ptr->position.y * BLOCK_HEIGHT)
                              +AREA_START_Y);
              }

        if (Options.piece_pre[1] == 'n')
              {
                 display_preview_piece();
              }
           }
			break;

		 case KEY_MENU_OPTIONS:
			if (!Game_paused)
            {
               display_options_menu();
               options_changed = alter_options(&Options);
               if (options_changed)
               {
               /*
               ** Clear the Tetris board.
               */
#ifdef LCD
                  gr_rect(BOARD_START_X,BOARD_START_Y,
                          GAME_BOARD_WIDTH,GAME_BOARD_HEIGHT,
			      	       PIECE_WINDOW_BORDER_WIDTH,GR_BLACK,GR_WHITE);
#else
                  gr_bevel_rect(BOARD_START_X,BOARD_START_Y,
                                GAME_BOARD_WIDTH,GAME_BOARD_HEIGHT,
	     		                 BORDER_WIDTH,GR_GRAY,BEVEL_IN);
#endif
                  start_skill_lev = Options.skill_lev;
			      new_game(Options.start_lev,Options.skill_lev,&Game_stats);
                  display_score_window();
                  Game_playing = FALSE;
			      Game_paused = FALSE;
               }
               display_main_menu();

            }
			break;

		 case KEY_MENU_QUIT:
		 case EXIT_KEY:
            exit_choice = prompt_exit(AREA_START_X +
                                     (NUMBER_BLOCKS_WIDE * BLOCK_WIDTH >>1),
                                      AREA_START_Y +
                                     (NUMBER_BLOCKS_HI * BLOCK_HEIGHT >>1) );

            if ((exit_choice == EXIT_GAME_KEY)||(exit_choice=='y')
                 ||(exit_choice=='Y'))
            {
               event = EXIT_TETRIS_KEY;
            }
            break;

         default:
            break;
      }

      if (event != EXIT_TETRIS_KEY)
         event = get_event();

   }
}


/*******************************************************************************
**
** Unit Name:
**    init_game_opts
**
** Description:
**    This function sets the game options to a starting value when the game
**    first begins.Piece preview is set to "On", starting rows is set to 0,
**    and skill level is set to 1.
**
** Calling Convention:
**    void init_game_opts(OPTIONS_TYPE *options);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    options       Pointer to a options structure
**
** Return Value:
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN INITIALIZE GAME OPTIONS */
/* .  INITIALIZE STARTING LEVEL */
/* .  INITIALIZE SKILL LEVEL */
/* .  INITIALIZE PIECE PREVIEW */
/* END INITIALIZE GAME OPTIONS */

void init_game_opts(OPTIONS_TYPE *options)
{
   options->skill_lev = 1;
   options->start_lev = 0;
   sprintf(options->piece_pre,"%s",ON);
}


/*******************************************************************************
**
** Unit Name:
**    init_game_stats
**
** Description:
**    This function resets important game statistics and condition variables.
**    Score is reset,pause is turned off, the cleared rows count is zeroed, and
**    the timer length is set.
**
** Calling Convention:
**    void init_game_stats(GAME_STATS_TYPE *game_stats, UCHAR skill_lev);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    game_stats    Pointer to game statistics structure.
**    skill_lev     Current skill level of the game.
**
** Return Value:
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN INITIALIZE GAME STATISTICS */
/* .  GAME STATUS IS NOT PAUSED */
/* .  INITIALIZE TOTAL SCORE */
/* .  INITIALIZE TOTAL NUMBER OF ROWS CLEARED */
/* .  RESET MORE_ROWS_WERE_CLEARED TO FALSE */
/* .  RESET SKILL_LEV_CHANGE TO FALSE */
/* .  SET TIMER LENGTH */
/* .  RESET TIMER */
/* END INITIALIZE GAME STATISTICS */

void init_game_stats(GAME_STATS_TYPE *game_stats, UCHAR skill_lev)
{
   float time_level_tick;

   Game_playing = FALSE;
   game_stats->tot_score = 0;
   Row_data.tot_rows_cleared = 0;
   Row_data.more_rows_cleared = FALSE;
   game_stats->skill_lev_changed = FALSE;
   time_level_tick = ((float)Timer_table[skill_lev -1] * (float)TIMER_LEVEL);
   game_stats->timer_length = ONE_SECOND - (UINT2)time_level_tick;
}


/*******************************************************************************
**
** Unit Name:
**    init_board
**
** Description:
**    This function initializes the Tetris board data structure representing
**    the occupied blocks on the board and creates any additonal blocks on
**    the board required with a starting level that is greater than 0.
**
** Calling Convention:
**    void init_board(GAME_BOARD *game_board, UCHAR start_rows);
**
** Parameters:
**    Name          Description
**    ------------------------------------------------------------------
**    start_rows    Number of starting rows.
**    game_board    Pointer to the structure containing the Tetris board.
**
** Return Value:
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN INIT_BOARD */
/* .  FOR TOP ROW TO BOTTOM ROW */
/* .  .  FOR EACH POSITION IN ROW */
/* .  .  .  SET POSITION TO EMPTY */
/* .  .  END FOR */
/* .  END FOR */
/* .  IF (NUMBER OF STARTING ROWS IS GREATER THAN 0) THEN */
/* .  .  FOR BOTTOM ROW TO START LEVEL ROW DO */
/* .  .  .  FOR EACH POSITION IN ROW DO */
/* .  .  .  .  RANDOMLY DECIDE TO PUT A BLOCK HERE */
/* .  .  .  .  IF (BLOCK IS TO BE PUT HERE) THEN */
/* .  .  .  .  .  PICK RANDOM PATTERN */
/* .  .  .  .  .  ASSIGN BLOCK WITH PATTERN TO THIS POSITION */
/* .  .  .  .  END IF */
/* .  .  .  END FOR */
/* .  .  END FOR */
/* .  .  FOR (RANGE OF ROWS) DO */
/* .  .  .  PICK A RANDOM BLOCK IN THE ROW */
/* .  .  .  DELETE THE BLOCK FROM THIS POSITION */
/* .  .  END FOR */
/* .  .  SET GAME_BOARD PEAK */
/* .  END IF */
/* END INIT_BOARD */

void init_board(GAME_BOARD *game_board, UCHAR start_rows)
{
   INT1 x ;
   INT1 y ;
   UCHAR piece_probability;

   for (y=0;y< BOARD_HEIGHT;y++)
   {
      for(x=0;x< BOARD_WIDTH;x++)
      {
	     game_board->blocks_board[y][x] = EMPTY;
      }
   }

   if (start_rows > 0)
   {
      for(y=BOARD_HEIGHT-1;y>=(BOARD_HEIGHT - start_rows);y--)
      {
        for(x=0;x< BOARD_WIDTH;x++)
	    {
           piece_probability = (rand() % 10);
	       if (piece_probability < NUMBER_PIECES)
           {
	          game_board->blocks_board[y][x]= piece_probability;
	       }
        }
      }
      for(y=BOARD_HEIGHT-1;y>=(BOARD_HEIGHT - start_rows);y--)
      {
         piece_probability = (rand() % 10);
         game_board->blocks_board[y][piece_probability]= EMPTY;
      }
   }
   game_board->peak = BOARD_HEIGHT - 1 - start_rows;
}


/*******************************************************************************
**
** Unit Name:
**    new_game
**
** Description:
**    This function performs all of the necessary initializations for beginning
**    a new game. Previous option settings are checked.
**
** Calling Convention:
**    void new_game(UCHAR start_rows,UCHAR skill_lev GAME_STATS_TYPE *game_stats);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    game_stats    Pointer to structure holding vital game statistics.
**    start_rows    Current number of starting rows in game.
**    skill_lev     Currnt skill level
**
** Return Value:
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN NEW_GAME */
/* .  CALL INIT_GAME_STATS */
/* .  CALL INIT_BOARD */
/* .  INITIALIZE NUMBER OF MOVES BY CURRENT PIECE */
/* .  INITIALIZE DROP DISTANCE OF CURRENT PIECE */
/* .  INITIALIZE GAME TOTAL ROWS CLEARED */
/* .  SET ADDITIONAL ROWS CLEARED FLAG TO FALSE */
/* END NEW_GAME */

void new_game(UCHAR start_rows,UCHAR skill_lev, GAME_STATS_TYPE *game_stats)
{
   init_game_stats(game_stats, skill_lev);
   init_board(&Game_board, start_rows);
   Current_pc_ptr->rows_cleared = 0;
   Current_pc_ptr->drop_distance = 0;
   Row_data.tot_rows_cleared = 0;
   Row_data.more_rows_cleared = FALSE;

}


/*******************************************************************************
**
** Unit Name:
**    row_is_full
**
** Description:
**    This function tests a row to see if it is full.
**
** Calling Convention:
**    UCHAR row_is_full(GAME_BOARD *game_board, UCHAR row_num);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    game_board    Pointer to the game board structure.
**    row_num       Number of the row to be checked.
**
** Return Value
**    1 uppon success, else 0.
**
** Notes:
**
*******************************************************************************/
/* BEGIN ROW IS FULL */
/* .  FULL ROW = TRUE */
/* .  REPEAT */
/* .  .  FULL ROW = (SPACE IS NOT EMPTY) */
/* .  END REPEAT WHEN (FULL ROW IS FALSE OR TESTED ALL SPACES) */
/* .  RETURN FULL ROW */
/* END ROW IS FULL */


UCHAR row_is_full(GAME_BOARD *game_board, UCHAR row_num)
{
   UCHAR full_row = TRUE;
   UCHAR row_pos = 0;

   while ((full_row) && (row_pos < BOARD_WIDTH))
   {
      full_row = (game_board->blocks_board[row_num][row_pos] != EMPTY);
      row_pos +=1;
   }
   return(full_row);
}


/*******************************************************************************
**
** Unit Name:
**    display_piece
**
** Description:
**    This function displays the passed Tetris piece.
**
** Calling Convention:
**    void display_piece(ACTIVE_TETRIS_PIECE *current_piece, INT2 start_coord_x,
**                       INT2 start_coord_y);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    current_piece Pointer to the current active Tetris piece.
**    start_coord_x X-coordinate of the pivot block of the current_piece.
**    start_coord_y Y-coordinate of the pivot block of the current_piece.
**
** Return Value
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY_PIECE */
/* .  DISPLAY PIVOT (REFERENCE BLOCK) */
/* .  FOR (ALL REMAINING BLOCKS) DO */
/* .  .  DISPLAY THE BLOCK */
/* .  END DO */
/* END DISPLAY_PIECE */

void display_piece(ACTIVE_TETRIS_PIECE *current_piece,
		   INT2 start_coord_x,
		   INT2 start_coord_y)
{
   INT1 x;

   if (start_coord_y >= AREA_START_Y)
   {
      gr_pixmap(start_coord_x,
              start_coord_y,
              Pix_type[current_piece->data->block_style],GROP_SRC);
   }

   for(x=0;x<3;x++)
      if (((current_piece->data->block_map[current_piece->rotation][x].y
	        * BLOCK_WIDTH) + start_coord_y) >= AREA_START_Y)
      {
         gr_pixmap((current_piece->data->block_map[current_piece->rotation][x].x
	         * BLOCK_WIDTH) + start_coord_x,
	         (current_piece->data->block_map[current_piece->rotation][x].y
	         * BLOCK_WIDTH) + start_coord_y,
	         Pix_type[current_piece->data->block_style],GROP_SRC);
      }
}


/*******************************************************************************
**
** Unit Name:
**    display_row
**
** Description:
**    This function displays the blocks int the given row.
**
** Calling Convention:
**    void display_row(UCHAR row_number, BOOL hilite_status);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    row_number    The row to display.
**    hilite_status NORMAL or HILITED
**
** Return Value:
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY ROW */
/* .  IF (ROW IS NOT TO BE HILITED) THEN */
/* .  .  FOR (FIRST COLUMN TO LAST COLUMN) DO */
/* .  .  .  DISPLAY BLOCK */
/* .  .  END  DO */
/* .  ELSE */
/* .  .  HILITE THE ROW */
/* END DISPLAY BOARD */

void display_row(UCHAR row_number, BOOL hilite_status)
{
   INT1 x ;


   if (hilite_status != HI_LITED)
   {
      for(x=0;x < NUMBER_BLOCKS_WIDE;x++)
      {

         if (Game_board.blocks_board[row_number][x] != EMPTY)
         {
         /*
         ** Display the block.
         */
            gr_pixmap((x * BLOCK_WIDTH) + AREA_START_X,
                      (row_number * BLOCK_HEIGHT) + AREA_START_Y,
                      Pix_type[Game_board.blocks_board[row_number][x]],
                      GROP_SRC);

         }
         else
         {
         /*
         ** Display an empty spot.
         */
#ifdef LCD
            gr_rect((x * BLOCK_WIDTH) + AREA_START_X,
                    (row_number * BLOCK_HEIGHT)+AREA_START_Y,
                    BLOCK_WIDTH,
                    BLOCK_HEIGHT,
                    0,
                    GR_WHITE,
                    GR_WHITE);
#else
            gr_rect((x * BLOCK_WIDTH) + AREA_START_X,
                    (row_number * BLOCK_HEIGHT)+AREA_START_Y,
                    BLOCK_WIDTH,
                    BLOCK_HEIGHT,
                    0,
                    GR_GRAY,
                    GR_GRAY);
#endif
         }
      }
   }
   else
   {
   /*
   ** Display a hilited row.
   */
#ifdef LCD
      gr_rect(AREA_START_X,
              (row_number * BLOCK_HEIGHT)+AREA_START_Y,
              BLOCK_WIDTH * NUMBER_BLOCKS_WIDE,
              BLOCK_HEIGHT,
              0,
              GR_GRAY,
              GR_GRAY);
#else
      gr_rect(AREA_START_X,
              (row_number * BLOCK_HEIGHT)+AREA_START_Y,
              BLOCK_WIDTH * NUMBER_BLOCKS_WIDE,
              BLOCK_HEIGHT,
              0,
              GR_WHITE,
              GR_WHITE);
#endif

   }
}


/*******************************************************************************
**
** Unit Name:
**    display_board
**
** Description:
**    This function displays the Tetris playing area on the screen.
**
** Calling Convention:
**    void display_board(GAME_BOARD *game_board);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    game_board    Pointer to the Tetris game board structure.
**
** Return Value:
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY BOARD */
/* .  FOR (TOP ROW TO BOTTOM ROW) DO */
/* .  .  CALL DISPLAY_ROW */
/* .  END DO */
/* END DISPLAY BOARD */

void display_board(GAME_BOARD *game_board)
{
   INT1 y ;

   for(y=game_board->peak;y < NUMBER_BLOCKS_HI;y++)
   {
      display_row((UCHAR)y,NORMAL);
   };
}


/*******************************************************************************
**
** Unit Name:
**    display_main_menu
**
** Description:
**    This function displays the main menu in the menu display area.
**
** Calling Convention:
**    void display_main_menu(void);
**
** Parameters:
**    None
**
** Return Value
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY MAIN MENU */
/* .  DRAW THE MENU BACKDGROUND */
/* .  SELECT THE MENU FONT */
/* .  PRINT THE MENU TEXT */
/* END DISPLAY MAIN MENU */

void display_main_menu (void)
{
   /*
   ** Draw the rectangular border inside the bevel of the window and fill
   ** the window background.
   */
   gr_rect (MENU_START_X, MENU_START_Y,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			1,
			GR_BLACK, GR_WHITE);

   if (font_select (MENU_FONT) < 0) {
	  exit (-1);
   }

   /*
   ** Print the text centered in the window.
   */
   if (font_printf_centered (MENU_TEXT,
                             MENU_START_X + BORDER_WIDTH,
                             MENU_START_Y + BORDER_WIDTH,
                             MENU_START_X + WINDOW_WIDTH - BORDER_WIDTH,
                             MENU_START_Y + WINDOW_HEIGHT - BORDER_WIDTH)) {
      exit (-1);
   }

#ifdef LCD
   if (font_select (DEFAULT_FONT) < 0) {
	  exit (-1);
   }
#endif

}



/*******************************************************************************
**
** Unit Name:
**    display_options_menu
**
** Description:
**    This function displays the options menu in the menu display area.
**
** Calling Convention:
**    void display_options_menu(void);
**
** Parameters:
**    None
**
** Return Value
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY OPTIONS MENU */
/* .  DRAW THE MENU BORDER */
/* .  SELECT A FONT */
/* .  PRINT THE OPTIONS TEXT AND THEIR CURRENT SETTINGS */
/* END DISPLAY OPTIONS MENU */

void display_options_menu (void)
{
   char d_ray[5];

   /*
   ** Draw the rectangular border for the window to distinguish it
   ** from the white bevel.
   */

   gr_rect (MENU_START_X, MENU_START_Y,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			1,
			GR_BLACK,GR_WHITE);

   /*
   ** Select the font to print the text in.
   */
   if (font_select (MENU_FONT) < 0) {
	  exit (-1);
   }

   if (font_printf_centered (SKILL_TEXT,
							 MENU_START_X,
							 SKILL_TEXT_START_Y,
							 MENU_START_X + WINDOW_WIDTH,
							 SKILL_NUM_START_Y)) {
	  exit (-1);
   }

   /*
   ** Print the current skill level.
   */

   sprintf(d_ray,"%u",Options.skill_lev);
   if (font_printf_centered (d_ray,
                             MENU_START_X,
                             SKILL_NUM_START_Y,
                             MENU_START_X + WINDOW_WIDTH,
                             START_TEXT_START_Y)) {
      exit (-1);
   }

   if (font_printf_centered (START_TEXT,
                             MENU_START_X,
                             START_TEXT_START_Y,
                             MENU_START_X + WINDOW_WIDTH,
                             START_NUM_START_Y)) {
	  exit (-1);
   }

   /*
   ** Print the current start level.
   */

   sprintf(d_ray,"%u",Options.start_lev);
   if (font_printf_centered (d_ray,
                             MENU_START_X,
			                 START_NUM_START_Y,
                             MENU_START_X + WINDOW_WIDTH,
                             PIECE_PRE_TEXT_START_Y)) {
      exit (-1);
   }

   if (font_printf_centered (PREV_TEXT,
                             MENU_START_X,
                             PIECE_PRE_TEXT_START_Y,
			                 MENU_START_X + WINDOW_WIDTH,
                             PIECE_PRE_NUM_START_Y)) {
	  exit (-1);
   }

   /*
   ** Print the current status of Piece Preview. (On or Off)
   */

   sprintf(d_ray,"%s",Options.piece_pre);
   if (font_printf_centered (d_ray,
                             MENU_START_X,
                             PIECE_PRE_NUM_START_Y,
                             MENU_START_X + WINDOW_WIDTH,
                             PIECE_PRE_NUM_START_Y + OPTIONS_ENTRY_HEIGHT)) {
      exit (-1);
   }

   if (font_select (DEFAULT_FONT) < 0) {
	  exit (-1);
   }

}


/*******************************************************************************
**
** Unit Name:
**    display_end_game
**
** Description:
**    This function displays a message inicating that the game is over.
**
** Calling Convention:
**    void display_end_game(void);
**
** Parameters:
**    None
**
** Return Value
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY_END_GAME */
/* .  SELECT A FONT */
/* .  PRINT GAME OVER MESSAGE */
/* END DISPLAY_END_GAME */

void display_end_game (void)
{
   UINT2 valid_keys[1] = {
    RETURN_KEY
   };

   /*
   ** Select the font to print the text in.
   */
   if (font_select (DEFAULT_FONT) < 0) {
      exit (-1);
   }

   pop_up_masked (END_GAME_TEXT,
                  AREA_START_X,
                  AREA_START_Y,
                  AREA_START_X + PLAYING_AREA_WIDTH,
                  AREA_START_Y + PLAYING_AREA_HEIGHT,
                  valid_keys,
                  1);
}


/*******************************************************************************
**
** Unit Name:
**    display_paused_game
**
** Description:
**    This function displays a message inicating that the game is paused.
**
** Calling Convention:
**    void display_paused_game(void);
**
** Parameters:
**    None
**
** Return Value
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY_PAUSED_GAME */
/* .  ERASE MENU BACKGROUND */
/* .  SELECT A FONT */
/* .  PRINT GAME OVER MESSAGE */
/* END DISPLAY_PAUSED_GAME */

void display_paused_game (void)
{
   char d_ray[5];

   /*
   ** Erase the background before displaying the message.
   */
   gr_rect (MENU_START_X, MENU_START_Y,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            1,
            GR_BLACK,GR_WHITE);

   /*
   ** Select the font to print the text in.
   */
   if (font_select (DEFAULT_FONT) < 0) {
      exit (-1);
   }

   if (font_printf_centered (PAUSED_MESSAGE,
			                 MENU_START_X,
                             MENU_START_Y,
                             MENU_START_X + WINDOW_WIDTH,
                             MENU_START_Y + WINDOW_HEIGHT)) {
      exit (-1);
   }

}


/*******************************************************************************
**
** Unit Name:
**    display_score_window
**
** Description:
**    This function displays all of the parts of the scoring window. This
**    includes the score, skill level, number of lines cleared, and the next
**    Tetris piece if piece preview is turned on.
**
** Calling Convention:
**    void display_score_window();
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    None
**
** Return Value
**   None
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY SCORE WINDOW */
/* .  DRAW THE PIECE PREVIEW WINDOW */
/* .  SELECT THE FONT FOR THE SCORE WINDOW */
/* .  PRINT THE SCORE */
/* .  PRINT THE SKILL LEVEL */
/* .  PRINT THE NUMBER OF ROWS CLEARED */
/* END DISPLAY SCORE WINDOW */

void display_score_window (void)
{
   char lines[5];
   char score[10];
   char skill[3];

   /*
   ** Draw the piece preview window
   */
#ifndef LCD
   gr_bevel_rect (IN_SCORE_START_X + BORDER_WIDTH,
				  PIECE_WINDOW_START_Y,
				  WINDOW_WIDTH,
				  PIECE_WINDOW_HEIGHT,
				  BORDER_WIDTH,
				  GR_GRAY, BEVEL_IN);
#endif

#ifdef LCD
   gr_rect (SCORE_START_X,
			PIECE_WINDOW_START_Y,
			WINDOW_WIDTH,
			PIECE_WINDOW_HEIGHT,
			PIECE_WINDOW_BORDER_WIDTH,
			GR_BLACK,
            GR_WHITE);
   /*
   ** window shadow
   */

   gr_line( SCORE_START_X + SHADOW_THICKNESS,
            SCORE_START_Y + WINDOW_HEIGHT,
            SCORE_START_X + WINDOW_WIDTH  + (SHADOW_THICKNESS -1),
            SCORE_START_Y + WINDOW_HEIGHT,
            SHADOW_THICKNESS,
            GR_BLACK);

   gr_line( SCORE_START_X + WINDOW_WIDTH,
            SCORE_START_Y + SHADOW_THICKNESS,
            SCORE_START_X + WINDOW_WIDTH,
            SCORE_START_Y + WINDOW_HEIGHT + (SHADOW_THICKNESS -1),
            SHADOW_THICKNESS,
            GR_BLACK);
#endif

   /*
   ** Select the font to print the text in.
   */
   if (font_select (SCORE_FONT) < 0) {
	  exit (-1);
   }

   sprintf(score,"%u",Game_stats.tot_score);

   /*
   ** Erase the previous score.
   */
#ifdef LCD
	  gr_rect(SCORE_START_X + PIECE_WINDOW_BORDER_WIDTH,
			  SCORE_NUM_START_Y + 2,
			  WINDOW_WIDTH - 2 * PIECE_WINDOW_BORDER_WIDTH,
			  SCORE_ENTRY_HEIGHT,
			  0,
			  GR_WHITE,
			  GR_WHITE);
#else
	  gr_rect(SCORE_START_X + BORDER_WIDTH,
			  SCORE_NUM_START_Y,
			  WINDOW_WIDTH - 2 * BORDER_WIDTH,
			  SCORE_ENTRY_HEIGHT,
			  0,
			  GR_WHITE,
			  GR_WHITE);
#endif

   /*
   ** Print the current score.
   */

   if (font_printf_centered (score,
                             SCORE_START_X,
                             SCORE_NUM_START_Y,
							 SCORE_START_X + WINDOW_WIDTH,
                             LEVEL_TEXT_START_Y)) {
      exit (-1);
   }

   sprintf(skill,"%u",Options.skill_lev);

   /*
   ** Erase the previous skill level.
   */
#ifdef LCD
      gr_rect(SCORE_START_X + BORDER_WIDTH,
              LEVEL_NUM_START_Y +2,
              WINDOW_WIDTH - 2 * BORDER_WIDTH,
              SCORE_ENTRY_HEIGHT,
              0,
              GR_WHITE,
              GR_WHITE);
#else
      gr_rect(SCORE_START_X + BORDER_WIDTH,
              LEVEL_NUM_START_Y,
              WINDOW_WIDTH - 2 * BORDER_WIDTH,
              SCORE_ENTRY_HEIGHT,
              0,
              GR_WHITE,
              GR_WHITE);
#endif

   /*
   ** Print the current skill level.
   */

   if (font_printf_centered (skill,
                             SCORE_START_X,
                             LEVEL_NUM_START_Y,
                             SCORE_START_X + WINDOW_WIDTH,
							 LINES_TEXT_START_Y)) {
      exit (-1);
   }

   sprintf(lines,"%u",Row_data.tot_rows_cleared);

   /*
   ** Erase the previous nuber of lines cleared.
   */

   gr_rect(SCORE_START_X + BORDER_WIDTH,
   	       LINES_NUM_START_Y,
   	       WINDOW_WIDTH - 2 * BORDER_WIDTH,
   	       SCORE_ENTRY_HEIGHT,
   	       0,
   	       GR_WHITE,
   	       GR_WHITE);

   /*
   ** Print the current number of cleared rows.
   */

   if (font_printf_centered (lines,
                             SCORE_START_X,
                             LINES_NUM_START_Y,
                             SCORE_START_X + WINDOW_WIDTH,
                             LINES_NUM_START_Y + SCORE_ENTRY_HEIGHT)) {
      exit (-1);
   }

}


/******************************************************************************
**
** Unit Name:
**    display_game
**
** Description:
**    This function puts all components of the Tetris game area on the screen.
**    The components of the game area are: the backdrop, the title, the playing
**    area, the menu window and the scoring window.
**
** Calling Convention:
**    void display_game();
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    game_board    Pointer to the Tetris game board structure.
**
** Return Value:
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN DISPLAY_GAME */
/* .  CLEAR THE SCREEN */
/* .  DISPLAY TITLE */
/* .  DISPLAY_SCORE_WINDOW BORDERS */
/* .  CALL DISPLAY_SCORE_WINDOW */
/* .  DISPLAY MENU_WINDOW(MAIN) BORDERS */
/* .  IF (GAME IS NOT PAUSED) THEN */
/* .  .  CALL DISPLAY MENU_WINDOW(MAIN) */
/* .  END IF */
/* .  DISPLAY_GAMEBOARD BORDERS */
/* END DISPLAY_GAME */

void display_game(void)
{
   gr_clear_scr(GR_GRAY);

   if (font_select (DEFAULT_FONT) < 0) {
	  exit (-1);
   }

   window_title(Banner_name);

#ifndef LCD
/*
** Bevels around the scoring window.
*/
   gr_bevel_rect(OUT_SCORE_START_X,OUT_SCORE_START_Y,OUT_WINDOW_WIDTH,
				 OUT_WINDOW_HEIGHT,BORDER_WIDTH,GR_NONE,BEVEL_OUT);

   gr_bevel_rect(IN_SCORE_START_X,IN_SCORE_START_Y,IN_WINDOW_WIDTH,
				 IN_WINDOW_HEIGHT,BORDER_WIDTH,GR_NONE,BEVEL_IN);

/*
** Scoring window text and background.
*/
   gr_rect (SCORE_START_X, SCORE_START_Y,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			1,
			GR_BLACK, GR_WHITE);

   if (font_printf_centered (SCORE_TEXT,
							 SCORE_START_X,
							 SCORE_START_Y + BORDER_WIDTH,
							 SCORE_START_X + WINDOW_WIDTH,
							 (SCORE_START_Y + BORDER_WIDTH) + SCORE_ENTRY_HEIGHT)) {
	  exit (-1);
   }

   if (font_printf_centered (LEVEL_TEXT,
							 SCORE_START_X,
							 LEVEL_TEXT_START_Y,
							 SCORE_START_X + WINDOW_WIDTH,
							 LEVEL_NUM_START_Y)) {
	  exit (-1);
   }

   if (font_printf_centered (LINES_TEXT,
							 SCORE_START_X,
							 LINES_TEXT_START_Y,
							 SCORE_START_X + WINDOW_WIDTH,
							 LINES_NUM_START_Y)) {
	  exit (-1);
   }
   display_score_window();

/*
** Bevels around the menu area.
*/
   gr_bevel_rect(OUT_MENU_START_X,OUT_MENU_START_Y,OUT_WINDOW_WIDTH,
				 OUT_WINDOW_HEIGHT,BORDER_WIDTH,GR_NONE,BEVEL_OUT);

   gr_bevel_rect(IN_MENU_START_X,IN_MENU_START_Y,IN_WINDOW_WIDTH,
				 IN_WINDOW_HEIGHT,BORDER_WIDTH,GR_NONE,BEVEL_IN);

   if (! Game_paused)
   {
      display_main_menu();
   }

/*
** Bevels around the playing area.
*/
   gr_bevel_rect(OUTER_BOARD_START_X,OUTER_BOARD_START_Y,OUTER_BOARD_WIDTH,
				 OUTER_BOARD_HEIGHT,BORDER_WIDTH,GR_NONE,BEVEL_OUT);

   gr_bevel_rect(BOARD_START_X,BOARD_START_Y,GAME_BOARD_WIDTH,GAME_BOARD_HEIGHT,
				 BORDER_WIDTH,GR_NONE,BEVEL_IN);
#endif

#ifdef LCD
/*
** Scoring window text and background.
*/
   gr_rect (SCORE_START_X,
			SCORE_START_Y,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			PIECE_WINDOW_BORDER_WIDTH,
			GR_BLACK,
			GR_WHITE);

   if (font_printf_centered (SCORE_TEXT,
							 SCORE_START_X,
							 SCORE_START_Y,
							 SCORE_START_X + WINDOW_WIDTH,
							 SCORE_START_Y + SCORE_ENTRY_HEIGHT)) {
	  exit (-1);
   }

   if (font_printf_centered (LEVEL_TEXT,
							 SCORE_START_X,
							 LEVEL_TEXT_START_Y,
							 SCORE_START_X + WINDOW_WIDTH,
							 LEVEL_NUM_START_Y)) {
	  exit (-1);
   }

   if (font_printf_centered (LINES_TEXT,
							 SCORE_START_X,
							 LINES_TEXT_START_Y,
							 SCORE_START_X + WINDOW_WIDTH,
							 LINES_NUM_START_Y)) {
	  exit (-1);
   }

   display_score_window();

   if (!Game_paused)
   {
	  display_main_menu();
   }

   gr_rect(BOARD_START_X,BOARD_START_Y,GAME_BOARD_WIDTH,GAME_BOARD_HEIGHT,
				 PIECE_WINDOW_BORDER_WIDTH,GR_BLACK,GR_WHITE);

   /*
   ** main menu window shadow
   */

   gr_line( MENU_START_X + SHADOW_THICKNESS,
			MENU_START_Y + WINDOW_HEIGHT,
			MENU_START_X + WINDOW_WIDTH  + (SHADOW_THICKNESS - 1),
			MENU_START_Y + WINDOW_HEIGHT,
			SHADOW_THICKNESS,
			GR_BLACK);

   gr_line( MENU_START_X + WINDOW_WIDTH,
			MENU_START_Y + SHADOW_THICKNESS,
			MENU_START_X + WINDOW_WIDTH,
			MENU_START_Y + WINDOW_HEIGHT + (SHADOW_THICKNESS - 1),
			SHADOW_THICKNESS,
			GR_BLACK);

#endif


}


/******************************************************************************
**
** Unit Name:
**    get_event
**
** Descripion:
**    This function returns the next event to process during the game. Events
**    consist of key presses and timer ticks.
**
** Calling Convention:
**    EVENT_TYPE get_event();
**
** Parameters:
**    None
**
** Return Value:
**    Event depicting a keystroke or a timer expiring.
**
** Notes:
**
*******************************************************************************/
/* BEGIN GET_EVENT */
/* .  WHILE (EVENT HAS NOT OCCURED) DO */
/* .  .  IF (A KEY HAS BEEN PRESSED) THEN */
/* .  .  .  RETRIEVE THE KEY */
/* .  .  ELSE */
/* .  .  .  READ TIMER */
/* .  .  .  IF (TIMER_NOW < LAST_TICK) THEN */
/* .  .  .  .  ELAPSED = (TIMER_NOW) + (MAX-TICK - LAST_TICK) */
/* .  .  .  ELSE */
/* .  .  .  .  ELAPSED = (TIMER_NOW - LAST_TICK) */
/* .  .  .  END IF */
/* .  .  .  IF ((ELAPSED - LAST_TICK) > TIMER_LENGTH) THEN */
/* .  .  .  .  EVENT HAS OCCURRED */
/* .  .  .  .  LAST_TICK = TIMER_NOW */
/* .  .  .  .  SET EVENT TO TIMER_TICK EVENT */
/* .  .  .  ELSE */
/* .  .  .  .  EVENT HAS NOT OCCURRED */
/* .  .  .  END IF */
/* .  END WHILE */
/* .  RETURN EVENT */
/* END GET_EVENT */

EVENT_TYPE get_event (void)
{
   static INT4 last_time = 0;
   INT4 now_time;
   INT4 elapsed_time;
   EVENT_TYPE key_pressed;

   do
   {
	  if (inp_kbhit())
		 key_pressed = inp_getch();
	  else
	  {
		 /*
		 ** Check the timer.  Store the current value in a
		 ** variable so that it will not keep changing while
		 ** we look at it.
		 */
		 now_time = TIMER_NOW;

		 /*
		 ** See how long it has been since the timer was last
		 ** checked.
		 */
		 if (now_time < last_time) {
			elapsed_time = TIMER_MAX - last_time + now_time;
		 }
		 else {
			elapsed_time = now_time - last_time;
		 }
		 /*
		 ** If the timer has ticked then that is the event that
		 ** will be returned.  Key presses will have to wait.
		 */
		 if (elapsed_time > Game_stats.timer_length)
		 {
			key_pressed = TIMER_KEY;
			last_time = now_time;
		 }
		 else
			key_pressed = FALSE;
	  }
   } while (key_pressed == FALSE);

   return (key_pressed);
}


/*****************************************************************************
**
** Unit Name: mostly_in_bounds
**
** Description:
**    This function determines whether the position for the piece is within
**    bounds of the bottom of the gameboard and the two sides.
**
** Calling Convention:
**    UCHAR mostly_in_bounds(GAME_BOARD *game_board,
**                           ACTIVE_TETRIS_PIECE *moved_piece);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    moved_piece   Pointer to the current active Tetris piece in its
**                  proposed new position on the game board
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    1 if piece lies within the 3 borders, 0 otherwise.
**
** Notes:
**
*****************************************************************************/
/* BEGIN MOSTLY_IN_BOUNDS */
/* .  IF (REFERENCE BLOCK IS IN BOUNDS) THEN */
/* .  FOR (EACH BLOCK ON THE POSITION LIST) DO */
/* .  .  IF (BLOCK IS IN BOUNDS */
/* .  .  .  PIECE IS IN BOUNDS */
/* .  .  END IF */
/* .  END DO */
/* .  RETURN (PIECE MOSTLY IN BOUNDS) */
/* END MOSTLY_IN_BOUNDS */


UCHAR mostly_in_bounds(GAME_BOARD *game_board, ACTIVE_TETRIS_PIECE *moved_piece)
{

   INT1 x;
   INT1 is_valid;
   INT1 is_valid_ref;
   char y_pos;
   char x_pos;

   is_valid_ref = ((moved_piece->position.x >=0) &&
                   (moved_piece->position.x < BOARD_WIDTH) &&
                   (moved_piece->position.y < BOARD_HEIGHT));

   if (is_valid_ref)
   {

      for(x=0;x<3;x++)
      {
         y_pos = moved_piece->position.y + moved_piece->data->block_map
                 [moved_piece->rotation][x].y;
         x_pos = moved_piece->position.x + moved_piece->data->block_map
                 [moved_piece->rotation][x].x;

         is_valid = ((x_pos >= 0) &&
                     (x_pos < BOARD_WIDTH) &&
                     (y_pos < BOARD_HEIGHT));

         if(!is_valid)
            break;
      }
   }
   return(is_valid_ref && is_valid);

}


/*****************************************************************************
**
** Unit Name: piece_in_bounds
**
** Description:
**    This function determines whether the piece lies within all bounds
**    of the gameboard.
**
** Calling Convention:
**    UCHAR piece_in_bounds(GAME_BOARD *game_board,
**                          ACTIVE_TETRIS_PIECE *moved_piece);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    moved_piece   Pointer to the current active Tetris piece in its
**                  proposed new position on the game board
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    1 if piece in bounds, 0 otherwise.
**
** Notes:
**
*****************************************************************************/
/* BEGIN PIECE_IN_BOUNDS */
/* .  IF (REFERENCE BLOCK IS IN BOUNDS) THEN */
/* .  FOR (EACH BLOCK ON THE POSITION LIST) DO */
/* .  .  IF (BLOCK IS IN BOUNDS) */
/* .  .  .  PIECE IS IN BOUNDS */
/* .  .  END IF */
/* .  END DO */
/* .  RETURN (PIECE IN BOUNDS) */
/* END PIECE_IN_BOUNDS */


UCHAR piece_in_bounds(GAME_BOARD *game_board, ACTIVE_TETRIS_PIECE *moved_piece)
{

   INT1 x;
   INT1 is_valid;
   INT1 is_valid_ref;
   char y_pos;

   is_valid_ref = (moved_piece->position.y >= 0);

   if (is_valid_ref)
   {

      for(x=0;x<3;x++)
      {
         y_pos = moved_piece->position.y + moved_piece->data->block_map
                 [moved_piece->rotation][x].y;

         is_valid = (y_pos >= 0);

         if(!is_valid)
            break;
      }
   }
   return(is_valid_ref && is_valid && mostly_in_bounds(game_board, moved_piece));

}


/*****************************************************************************
**
** Unit Name:  no_collision
**
** Description:
**    This function determines whether the new position for the piece will 
**    interfere with a piece currently on the gameboard.
**
** Calling Convention:
**    UCHAR no_collision(GAME_BOARD *game_board,
**                       ACTIVE_TETRIS_PIECE *moved_piece);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    moved_piece   Pointer to the current active Tetris piece in its
**                  proposed new position on the game board
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    1 if no collision would occur, 0 otherwise.
**
** Notes:
**
*****************************************************************************/
/* BEGIN NO_COLLISION */
/* .  IF (REFERENCE BLOCK DOES NOT COLLIDE */
/* .  FOR (EACH BLOCK ON THE POSITION LIST) DO */
/* .  .  IF (BLOCK DOES NOT COLLIDE */
/* .  .  .  PIECE DOES NOT COLLIDE */
/* .  .  END IF */
/* .  END DO */
/* .  RETURN (PIECE DOES NOT COLLIDE) */
/* END NO_COLLISION */


UCHAR no_collision(GAME_BOARD *game_board, ACTIVE_TETRIS_PIECE *moved_piece)
{
   INT1 x;
   INT1 is_valid;
   INT1 is_valid_ref;
   char y_pos;
   char x_pos;

   is_valid_ref = (((game_board->blocks_board[moved_piece->position.y]
                    [moved_piece->position.x] == EMPTY) &&
                    (moved_piece->position.y >= 0)) ||
                    (moved_piece->position.y < 0));

   if (is_valid_ref)
   {

      for(x=0;x<3;x++)
      {
         y_pos = moved_piece->position.y + moved_piece->data->block_map
                 [moved_piece->rotation][x].y;
         x_pos = moved_piece->position.x + moved_piece->data->block_map
                 [moved_piece->rotation][x].x;

         is_valid = (((game_board->blocks_board[y_pos][x_pos] == EMPTY) &&
                      y_pos >= 0) ||
                      y_pos < 0);

         if(!is_valid)
            break;
      }
   }
   return(is_valid_ref && is_valid);
}


/*****************************************************************************
**
** Unit Name:  is_valid_move
**
** Description:
**    This function determines whether the position for the piece is valid.
**
** Calling Convention:
**    UCHAR is_valid_move(GAME_BOARD *game_board,
**                        ACTIVE_TETRIS_PIECE *moved_piece);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    moved_piece   Pointer to the current active Tetris piece in its
**                  proposed new position on the game board
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    1 if move is valid, 0 otherwise.
**
** Notes:
**
*****************************************************************************/
/* BEGIN IS_VALID_MOVE */
/* .  RETURN (MOSTLY_IN_BOUNDS AND NO_COLLISION) */
/* END IS_VALID_MOVE */


UCHAR is_valid_move(GAME_BOARD *game_board, ACTIVE_TETRIS_PIECE *moved_piece)
{
   return(mostly_in_bounds(game_board, moved_piece) &&
      no_collision(game_board, moved_piece));
}


/*******************************************************************************
**
** Unit Name:
**    move_piece
**
** Description:
**    This function moves the Tetris piece from one location to the next
**    by erasing the blocks of an active Tetris piece and redrawing it's new
**    component blocks at the new position.
**
**
** Calling Convention:
**    void move_piece(ACTIVE_TETRIS_PIECE *current_piece,
**                    *moved_piece);
**
** Parameters:
**    Name              Description
**    --------------------------------------------------------------------------
**    current_piece     Pointer to the active current Tetris piece.
**    moved_piece       Pointer to the current active Tetris piece in its new
**                      position
**
** Return Value:
**    None
**
** Notes:
**    This function does not erase blocks that are shared by the new and old
**    positions.
*******************************************************************************/
/* BEGIN MOVE PIECE */
/* .  FOR (EACH BLOCK OF THE CURRENT PIECE) DO */
/* .  .  BLOCK IN NEW PIECE AND CURRENT PIECE = FALSE; */
/* .  .  FOR (EACH BLOCK IN THE NEW PIECE) DO */
/* .  .  .  IF (NEW BLOCK = CURRENT BLOCK) THEN */
/* .  .  .  .  BLOCK IN NEW PIECE AND IN CURRENT PIECE = TRUE; */
/* .  .  .  .  BREAK; */
/* .  .  .  END IF */
/* .  .  END DO */
/* .  .  IF (BLOCK NOT SHARED) THEN */
/* .  .  .  ERASE BLOCK AT CURRENT POSITION */
/* .  .  END IF */
/* .  END DO */
/* .  FOR (EACH BLOCK IN THE NEW PIECE) DO */
/* .  .  FOR (EACHBLOCK IN THE CURRENT PIECE) DO */
/* .  .  .  IF (NEW BLOCK = CURRENT BLOCK) THEN */
/* .  .  .  .  BLOCK IN NEW PIECE AND IN CURRENT PIECE = TRUE; */
/* .  .  .  .  BREAK; */
/* .  .  .  END IF */
/* .  .  END DO */
/* .  .  IF (BLOCK NOT SHARED) THEN */
/* .  .  .  DISPLAY BLOCK AT NEW POSITION */
/* .  .  END IF */
/* .  END DO */
/* .  CURRENT PIECE IS NEW PIECE */
/* END MOVE PIECE */

void move_piece(ACTIVE_TETRIS_PIECE *current_piece,
                ACTIVE_TETRIS_PIECE *moved_piece)
{
   register INT1 x;
   register INT1 y;
   UCHAR piece_in_both;
   char y_pos_mov;
   char x_pos_mov;
   char y_pos_cur;
   char x_pos_cur;

/*
** Determine which blocks will not exist in the moved piece and erase
** accordingly.
*/
   piece_in_both = TRUE;
   if ((current_piece->position.x != moved_piece->position.x) ||
       (current_piece->position.y != moved_piece->position.y))
   {
   /*
   ** Check the reference (pivot) block first.
   */
      for(x=0;x<3;x++)
      {
    	 x_pos_mov = (moved_piece->data->block_map[moved_piece->rotation][x].x +
                      moved_piece->position.x);
         y_pos_mov = (moved_piece->data->block_map[moved_piece->rotation][x].y +
                      moved_piece->position.y);

         piece_in_both = ((current_piece->position.x == x_pos_mov) &&
                          (current_piece->position.y == y_pos_mov));

         if (piece_in_both)
	        break;
      }
      if ((!piece_in_both) && (current_piece->position.y >= 0))
      {
      /*
      ** Erase this block for it is not common to the new and old positions.
      */
#ifdef LCD
         gr_rect((current_piece->position.x * BLOCK_WIDTH)+AREA_START_X,
                 (current_piece->position.y * BLOCK_HEIGHT)+AREA_START_Y,
                 BLOCK_WIDTH,BLOCK_HEIGHT,0,GR_WHITE,GR_WHITE);
#else
         gr_rect((current_piece->position.x * BLOCK_WIDTH)+AREA_START_X,
                 (current_piece->position.y * BLOCK_HEIGHT)+AREA_START_Y,
                 BLOCK_WIDTH,BLOCK_HEIGHT,0,GR_GRAY,GR_GRAY);
#endif
      }
   }

   /*
   ** Check the remaining blocks.
   */
   for(x=0;x<3;x++)
   {
      piece_in_both = TRUE;

      x_pos_cur = (current_piece->data->block_map[current_piece->rotation][x].x
                  + current_piece->position.x);
      y_pos_cur = (current_piece->data->block_map[current_piece->rotation][x].y
                  + current_piece->position.y);

      if ((x_pos_cur != moved_piece->position.x) ||
          (y_pos_cur != moved_piece->position.y))
      {
         for(y=0;y<3;y++)
         {
            x_pos_mov = (moved_piece->data->block_map[moved_piece->rotation][y].x +
                         moved_piece->position.x);
            y_pos_mov = (moved_piece->data->block_map[moved_piece->rotation][y].y +
                         moved_piece->position.y);


            piece_in_both = ((x_pos_cur == x_pos_mov) &&
                             (y_pos_cur == y_pos_mov));

            if (piece_in_both)
               break;
         }

         if ((!piece_in_both) && (y_pos_cur >= 0))
         {
         /*
         ** Erase this block for it is not common to the new and old positions.
         */
#ifdef LCD
            gr_rect((x_pos_cur * BLOCK_WIDTH)+AREA_START_X,
                    (y_pos_cur * BLOCK_HEIGHT)+AREA_START_Y,
                     BLOCK_WIDTH,BLOCK_HEIGHT,0,GR_WHITE,GR_WHITE);
#else
            gr_rect((x_pos_cur * BLOCK_WIDTH)+AREA_START_X,
                    (y_pos_cur * BLOCK_HEIGHT)+AREA_START_Y,
                     BLOCK_WIDTH,BLOCK_HEIGHT,0,GR_GRAY,GR_GRAY);
#endif
         }
      }
   }

/*
** Determine which blocks in the new piece are displayed on the board and draw
** the rest accordingly.
*/
   piece_in_both = TRUE;
   if ((current_piece->position.x != moved_piece->position.x) ||
       (current_piece->position.y != moved_piece->position.y))
   {
   /*
   ** Check the reference (pivot) block.
   */
      for(x=0;x<3;x++)
      {
         x_pos_cur = (current_piece->data->block_map[current_piece->rotation][x].x
                      + current_piece->position.x);
         y_pos_cur = (current_piece->data->block_map[current_piece->rotation][x].y
                      + current_piece->position.y);

         piece_in_both = ((moved_piece->position.x == x_pos_cur)
                       && (moved_piece->position.y == y_pos_cur));

         if (piece_in_both)
            break;
      }
      if ((!piece_in_both) && (moved_piece->position.y >= 0))
      {   gr_pixmap((moved_piece->position.x * BLOCK_WIDTH)+AREA_START_X,
		            (moved_piece->position.y * BLOCK_HEIGHT)+AREA_START_Y,
                    Pix_type[moved_piece->data->block_style],GROP_SRC);
      }
   }

   for(x=0;x<3;x++)
   {
      piece_in_both = TRUE;

      x_pos_mov = (moved_piece->data->block_map[moved_piece->rotation][x].x +
		   moved_piece->position.x);
      y_pos_mov = (moved_piece->data->block_map[moved_piece->rotation][x].y +
                   moved_piece->position.y);

      if (((current_piece->position.x) != x_pos_mov) ||
          ((current_piece->position.y) != y_pos_mov))
      {
         for(y=0;y<3;y++)
         {
            x_pos_cur = current_piece->data->block_map[current_piece->rotation]
                        [y].x + current_piece->position.x;
            y_pos_cur = current_piece->data->block_map[current_piece->rotation]
                        [y].y + current_piece->position.y;

            piece_in_both = ((x_pos_cur == x_pos_mov) &&
                             (y_pos_cur == y_pos_mov));

            if (piece_in_both)
               break;
         }

         if ((!piece_in_both) && (y_pos_mov >= 0))
         {
         /*
         ** Display this block of the Tetris piece.
         */
            gr_pixmap((x_pos_mov * BLOCK_WIDTH)+AREA_START_X,
                      (y_pos_mov * BLOCK_HEIGHT)+AREA_START_Y,
                      Pix_type[moved_piece->data->block_style],GROP_SRC);
         }
      }
   }
}


/*******************************************************************************
**
** Unit Name:
**    rotate_piece
**
** Desription:
**    This function attempts to rotate the Tetris piece counter-clockwise 90
**    degrees. If sucessful, the position of the active Tetris piece is
**    incremented by 1 and the number of rotations is also incremented by 1.
**
**
** Calling Convention:
**    UCHAR rotate_piece(ACTIVE_TETRIS_PIECE *current_piece,
**                      GAME_BOARD *game_board);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    current_piece Contains information about the current active Tetris piece.
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    1 upon success, else 0.
**
** Notes:
**
*******************************************************************************/
/* BEGIN ROTATE_PIECE */
/* .  MAKE COPY OF CURRENT PIECE */
/* .  UPDATE CURRENT PIECE TO SHOW NEW POSITION */
/* .  IF NEW POSTION IS VALID */
/* .  .  MOVE PIECE TO NEW POSITON */
/* .  .  RETURN 1 */
/* .  ELSE */
/* .  .  RESTORE CURRENT PIECE FROM COPY */
/* .  .  RETURN 0 */
/* .  END IF */
/* END ROTATE_PIECE */

UCHAR rotate_piece(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board)
{
   ACTIVE_TETRIS_PIECE moved_piece;
   size_t size_of_blocks;
   UCHAR move_valid;

   size_of_blocks = sizeof(moved_piece);
   memcpy(&moved_piece,current_piece,size_of_blocks);
   if ((current_piece->rotation += 1) >3)
      current_piece->rotation = 0;
   move_valid = is_valid_move(game_board,current_piece);
   if (move_valid)
      move_piece(&moved_piece,current_piece);
   else
      memcpy(current_piece,&moved_piece,size_of_blocks);
   return(move_valid);
}

/*******************************************************************************
**
** Unit Name:
**    move_piece_left
**
** Description:
**    This function attempts to shift the Tetris piece to the left a distance
**    of one block. If successful, the horizontal position of the active Tetris
**    piece is decremented by 1 and the number of moves is incremented by 1.
**
** Calling Convention:
**    UCHAR move_piece_left(ACTIVE_TETRIS_PIECE *current_piece,
**                         GAME_BOARD *game_board);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    current_piece Pointer to the current active Tetris piece.
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    1 upon success, else 0.
**
** Notes:
**
*******************************************************************************/
/* BEGIN MOVE_PIECE_LEFT */
/* .  MAKE COPY OF CURRENT PIECE */
/* .  CURRENT PIECE IS GIVEN NEW POSITION */
/* .  IF (NEW POSITION IS VALID) THEN */
/* .  .  MOVE PIECE TO NEW POSTION */
/* .  .  RETURN 1 */
/* .  ELSE */
/* .  .  RESTORE CURRENT PIECE FROM COPY */
/* .  .  RETURN 0 */
/* .  END IF */
/* END MOVE_PIECE_LEFT */

UCHAR move_piece_left(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board)
{
   ACTIVE_TETRIS_PIECE moved_piece;
   size_t size_of_blocks;
   UCHAR move_valid;

   size_of_blocks = sizeof(moved_piece);
   memcpy(&moved_piece,current_piece,size_of_blocks);
   current_piece->position.x -= 1;
   move_valid = is_valid_move(game_board,current_piece);
   if (move_valid)
      move_piece(&moved_piece,current_piece);
   else
      memcpy(current_piece,&moved_piece,size_of_blocks);
   return(move_valid);
}


/*******************************************************************************
**
** Unit Name:
**    move_piece-right
**
** Description:
**    This function attempts to shift the Tetris piece to the right a distance
**    of one block. If successful, the horizontal position of the active Tetris
**    piece is incremented by one and the number of moves is incremented by 1.
**
**
** Calling Convention:
**    UCHAR move_piece_right(ACTIVE_TETRIS_PIECE *current_piece,
**                          GAME_BOARD *game_board);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    current_piece Pointer to the current Tetris piece.
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    1 upon success, else 0.
**
** Notes:
**
*******************************************************************************/
/* BEGIN MOVE_PIECE_RIGHT */
/* .  MAKE COPY OF CURRENT PIECE */
/* .  CURRENT PIECE IS GIVEN NEW POSITION */
/* .  IF (NEW POSITION IS VALID) THEN */
/* .  .  MOVE PIECE TO NEW POSITION */
/* .  .  RETURN 1 */
/* .  ELSE */
/* .  .  RESTORE CURRENT PIECE FROM COPY */
/* .  .  RETURN 0 */
/* .  END IF */
/* END MOVE_PIECE_RIGHT */

UCHAR move_piece_right(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board)
{
   ACTIVE_TETRIS_PIECE moved_piece;
   size_t size_of_blocks;
   UCHAR move_valid;

   size_of_blocks = sizeof(moved_piece);
   memcpy(&moved_piece,current_piece,size_of_blocks);
   current_piece->position.x += 1;
   move_valid = is_valid_move(game_board,current_piece);
   if (move_valid)
       move_piece(&moved_piece,current_piece);
   else
      memcpy(current_piece,&moved_piece,size_of_blocks);
   return(move_valid);
}


/*******************************************************************************
**
** Unit Name:
**    move_piece_down
**
** Description:
**    This function attempts to move the Tetris piece down a distance of 1 block
**    vertically. If successful, the vertical position (y-value) is incremented
**    by 1. If unsuccessful, the block has landed. The score for the individual
**    piece is calculated and then added to the total score.
**
** Calling Convention:
**    UCHAR move_piece_down(ACTIVE_TETRIS_PIECE *current_piece,
**                         GAME_BOARD *game_board);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    current_piece Points to the current active Tetris piece.
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    1 upon success, else 0.
**
** Notes:
**
*******************************************************************************/
/* BEGIN MOVE_PIECE_DOWN */
/* .  MAKE COPY OF CURRENT PIECE */
/* .  CURRENT PIECE IS GIVEN NEW POSITION */
/* .  IF (NEW POSITION IS VALID) THEN  */
/* .  .  MOVE PIECE TO NEW POSITION  */
/* .  .  RETURN 1 */
/* .  ELSE */
/* .  .  RESTORE CURRENT PIECE FROM COPY */
/* .  .  RETURN 0 */
/* .  END IF */
/* END MOVE-PIECE-DOWN */

UCHAR move_piece_down(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board)
{
   ACTIVE_TETRIS_PIECE moved_piece;
   size_t size_of_blocks;
   UCHAR move_valid;

   size_of_blocks = sizeof(moved_piece);
   memcpy(&moved_piece,current_piece,size_of_blocks);
   current_piece->position.y += 1;
   move_valid = is_valid_move(game_board,current_piece);
   if (move_valid)
       move_piece(&moved_piece,current_piece);
   else
      memcpy(current_piece,&moved_piece,size_of_blocks);
   return(move_valid);
}


/*******************************************************************************
**
** Unit Name:
**    piece_landed
**
** Description:
**    This function updates the Tetris playing area and redraws it if necessary.
**
** Calling Convention:
**    void piece_landed(ACTIVE_TETRIS_PIECE *current_piece,
**                      GAME_BOARD *game_board);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    current_piece Pointer to the current active Tetris piece.
**    game_board    Pointer to Tetris game board structure.
**
** Return Value:
**
** Notes:
**
*******************************************************************************/
/* BEGIN PEICE LANDED PROCESING */
/* .  ADD CURRENT PIECE TO THE GAME BOARD STRUCTURE */
/* .  RESET NUMBER OF COMPLETED ROWS */
/* .  LO-ROW IS LOWEST ROW UPATED WHEN PIECE LANDED */
/* .  HI-ROW IS HIGHEST ROW UPDATED WHEN PIECE LANDED */
/* .  REPEAT */
/* .  .  IF (LO-ROW IS FULL) THEN */
/* .  .  .  COUNT FULL ROW */
/* .  .  .  DELETE FULL ROW */
/* .  .  .  DECREMENT HI-ROW */
/* .  .  ELSE */
/* .  .  .  INCREMENT LO-ROW */
/* .  .  END IF */
/* .  END REPEAT WHEN (HI-ROW = LO-ROW) */
/* .  IF (NUMBER OF COMPLETED ROWS IS GREATER THAN 0) THEN */
/* .  .  FLASH COMPLTETED ROWS */
/* .  .  FOR (LOWEST ROW UPDATED BY PIECE LANDING TO PEAK ROW) DO */
/* .  .  .  REDRAW ROWS ON THE GAME BOARD */
/* .  .  END FOR */
/* .  .  PEAK ROW =  PEAK ROW - NUMBER OF ROWS COMPLETED */
/* .  END IF */
/* END PIECE HAS LANDED PROCESSING */

void piece_landed(ACTIVE_TETRIS_PIECE *current_piece,
                  GAME_BOARD *game_board)
{
   register INT1 x;
   register INT1 y;
   register INT1 lo_row;
   register INT1 hi_row;
   UINT2 time_delay;
   UCHAR color;
   UCHAR rows_filled =0;
   char lowest_row;
   char y_pos;
   char x_pos;
   char d_ray[5];
   float time_level_tick;
   UCHAR cleared_rows[5] = {
      NUMBER_BLOCKS_HI,
      NUMBER_BLOCKS_HI,
      NUMBER_BLOCKS_HI,
      NUMBER_BLOCKS_HI,
      NUMBER_BLOCKS_HI
   };

   hi_row = current_piece->position.y;
   lo_row = current_piece->position.y;

   game_board->blocks_board[current_piece->position.y][current_piece->
         position.x] = current_piece->data->block_style;
/*
** Finds the high and low row of the range of rows that were updated when the
** Tetris piece landed.
*/

   for (x=0;x<3;x++)
   {
      y_pos = (current_piece->position.y + current_piece->data->block_map
               [current_piece->rotation][x].y);
      x_pos = (current_piece->position.x + current_piece->data->block_map
               [current_piece->rotation][x].x);

      game_board->blocks_board[y_pos][x_pos] = current_piece->data->block_style;

      if (y_pos < game_board->peak)
          game_board->peak = y_pos;

      if (y_pos > lo_row)
         lo_row = y_pos;
      else if (y_pos < hi_row)
         hi_row = y_pos;

   }

   lowest_row = lo_row;

/*
** Checks each row in the range of rows to see if it is full. If full, the row
** is counted, eliminated, and the rows above are moved down.
*/
   do
   {
      if (row_is_full(game_board, (UCHAR)lo_row))
      {
      /*
      ** Add cleared row to a list of cleared rows.
      */
         cleared_rows[rows_filled] = (UCHAR)lo_row;
         rows_filled++;

      /*
      ** Hi-lite the row that will be removed.
      */
         display_row((UCHAR)lo_row,HI_LITED);
      }
      lo_row -=1;
   }
   while(lo_row >= hi_row);

   if (rows_filled > 0)
   {
      color = GR_GRAY;
      for(x=0;x < HILITE_FACTOR;x++)
      {
          for(y=0;y < (rows_filled);y++)
          {
             gr_rect(AREA_START_X,
                     AREA_START_Y + (cleared_rows[y] * BLOCK_HEIGHT),
                     NUMBER_BLOCKS_WIDE * BLOCK_WIDTH,
                     BLOCK_HEIGHT,
                     0,
                     color,
                     color);
          }

          time_delay = 0;
       /*
       ** Wait.
       */
          do
          {
             time_delay++;
          }
          while (time_delay < ((ROW_DELAY/rows_filled) <<3));

       /*
       ** Alternate colors.
       */
          if (color == GR_WHITE)
             color = GR_GRAY;
          else
             color = GR_WHITE;
      }
   }
   
   lo_row = lowest_row;
   rows_filled = 0;

   do
   {
      if (row_is_full(game_board, (UCHAR)lo_row))
      {
         rows_filled++;
         for (x=lo_row;x>=game_board->peak + 1;x--)
         {
            memcpy(game_board->blocks_board[x],
                   game_board->blocks_board[x -1],
                   BOARD_WIDTH);

         }
         
         for(x=0;x< BOARD_WIDTH;x++)
         {
	         game_board->blocks_board[game_board->peak][x] = EMPTY;
         }

         hi_row +=1;
      }
      else
         lo_row -=1;
   }
   while(lo_row >= hi_row);

   if (rows_filled > 0)
   {
      /*
      ** Eliminate completed rows and shift rows above downward.
      */
      Row_data.tot_rows_cleared += rows_filled;
      for (y=lowest_row;y>=game_board->peak;y--)
      {
         display_row((UCHAR)y,NORMAL);
      }
      game_board->peak += rows_filled;
      current_piece->rows_cleared = rows_filled;
      Row_data.more_rows_cleared = TRUE;

/*
** Update the number of rows cleared and display the change in the scoring
** window.
*/
      gr_rect(SCORE_START_X + BORDER_WIDTH,
              LINES_NUM_START_Y,
              WINDOW_WIDTH - 2 * BORDER_WIDTH,
              SCORE_ENTRY_HEIGHT,
              0,
              GR_WHITE,
              GR_WHITE);

              sprintf(d_ray,"%u",Row_data.tot_rows_cleared);
              if (font_printf_centered (d_ray,
                                        SCORE_START_X,
                                        LINES_NUM_START_Y,
                                        SCORE_START_X + WINDOW_WIDTH,
                                        LINES_NUM_START_Y + SCORE_ENTRY_HEIGHT))
              {
                 exit (-1);
              }

/*
** Determine if skill level should be increased based on number of rows cleared.
*/
      if (((Row_data.tot_rows_cleared /12) >
          ((Row_data.tot_rows_cleared - rows_filled) /12)) && (Options.skill_lev < 10))
      {
         Options.skill_lev +=1;
         time_level_tick = ((float)Timer_table[Options.skill_lev -1] *
                            (float)TIMER_LEVEL);
         Game_stats.timer_length = ONE_SECOND - (UINT2)time_level_tick;
/*
** Display increased skill level inside the scoring window.
*/
#ifdef LCD
         gr_rect(SCORE_START_X + BORDER_WIDTH,
                 LEVEL_NUM_START_Y +2,
                 WINDOW_WIDTH - 2 * BORDER_WIDTH,
                 SCORE_ENTRY_HEIGHT,
                 0,
                 GR_WHITE,
                 GR_WHITE);
#else
         gr_rect(SCORE_START_X + BORDER_WIDTH,
                 LEVEL_NUM_START_Y,
                 (WINDOW_WIDTH - (BORDER_WIDTH << 1)),
                 SCORE_ENTRY_HEIGHT,
                 0,
                 GR_WHITE,
                 GR_WHITE);
#endif

         sprintf(d_ray,"%u",Options.skill_lev);
         if (font_printf_centered (d_ray,
                                   SCORE_START_X,
                				   LEVEL_NUM_START_Y,
                                   SCORE_START_X + WINDOW_WIDTH,
                                   LEVEL_NUM_START_Y + SCORE_ENTRY_HEIGHT)) {
         exit (-1);}
      }
      else

         Row_data.more_rows_cleared = FALSE;
   }
}


/*******************************************************************************
**
** Unit Name:
**    update_score
**
** Description:
**    This function returns the amount that should be added to the total score.
**
** Calling Convention:
**    INT4 update_score(OPTIONS_TYPE *options,ACTIVE_TETRIS_PIECE *current_piece);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    skill_level   Current skill level of the game.
**    current_piece Pointer to current active Tetris piece.
**    rows_cleared  Number of rows cleared when current piece landed.
**    piece_pre     Piece_preview status.
**
** Return Value:
**    Added score
**
** Notes:
**
*******************************************************************************/
/* BEGIN UPDATE SCORE */
/* .  BASE SCORE IS SKILL LEVEL MULTIPLIED BY 6 */
/* .  DROP DISTANCE IS DISTANCE FALLEN + 1 IF DISTANCE GREATER THAN 0 */
/* .  IF (SKILL LEVEL IS GREATER THAN 1)THEN */
/* .  .  IF (DROP DISTANCE IS LESS THAN NUMBER OF MOVES AND ROTATIONS) THEN */
/* .  .  .  DROP DISTANCE IS ZERO */
/* .  .  ELSE */
/* .  .  .  SUBTRACT NUMBER OF MOVES + NUMBER OF ROTATIONS FROM DROP DISTANCE */
/* .  .  END IF */
/* .  ELSE */
/* .  .  DROP DISTANCE IS UNAFFECTED BY MOVES AND ROTATIONS */
/* .  END IF */
/* .  PIECE SCORE =(BASE SCORE) + DROP DISTANCE */
/* .  CLEARING SCORE = 100 * (2 RAISED TO THE (# OF ROWS CLEARED -1)) */
/* .  RETURN(TOTAL SCORE + PIECE SCORE + CLEARING SCORE)/(PIECE_PRE +1)*/
/* UPDATE SCORE */

INT4 update_score(OPTIONS_TYPE *options,ACTIVE_TETRIS_PIECE *current_piece)
{
   UCHAR base;
   UCHAR drop_distance;
   UCHAR piece_score;
   UINT2 clear_score;

   base = options->skill_lev * 6;
   drop_distance = current_piece->drop_distance + (1 && (current_piece->
                   drop_distance > 0));
   if (options->skill_lev >1)
   {
      if (drop_distance < current_piece->number_moves)
      {
         drop_distance = 0;
      }
      else
      {
         drop_distance -= current_piece->number_moves;
      }
   }
   piece_score = base + drop_distance;
   clear_score = 100 * (current_piece->rows_cleared > 0 ) *
		(1 << (current_piece->rows_cleared-1));
   return((INT4)(((UINT2)(piece_score + clear_score)) /
                 ((UINT2)(options->piece_pre[1] == 'n') +1)));
}


/*******************************************************************************
**
** Unit Name:
**    init_display_piece
**
** Description:
**    This function draws all of the blocks that make up the active Tetris
**    piece on the game board at it's current postion.
**
** Calling Convention:
**    void init_display_piece(ACTIVE_TETRIS_PIECE *current_piece,
**                       GAME_BOARD *game_board);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    curent_piece  Pointer to the current active Tetris piece.
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    None
**
** Notes:
**
*******************************************************************************/
/* BEGIN INIT_DISPLAY_PIECE */
/* .  IF (PIECE CAN BE DISPLAYED) THEN */
/* .  .  CALL DISPLAY_PIECE */
/* .  END IF */
/* END INIT_DISPLAY_PIECE */


UCHAR init_display_piece(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board)
{
   INT1 x;
   UCHAR move_valid;

   current_piece->position.x = current_piece->data->start_position.x;
   current_piece->position.y = current_piece->data->start_position.y;
   current_piece->rotation = 0;
   current_piece->number_moves = 0;
   current_piece->drop_distance = 0;
   current_piece->rows_cleared = 0;

   move_valid = no_collision(game_board,current_piece);

   if (move_valid)
   {
      display_piece(current_piece,
         (current_piece->data->start_position.x * BLOCK_WIDTH)+AREA_START_X,
         (current_piece->data->start_position.y * BLOCK_HEIGHT)+AREA_START_Y);
   }

   return(move_valid);
}


/*******************************************************************************
**
** Unit Name:
**    drop_distance
**
** Description:
**    This function calculates the distance between a block in the active
**    Tetris piece and the closest block directly below.
**
** Calling Convention:
**    CHAR drop_distance(char piece_block_y,
**                       char piece_block_x,
**                       GAME_BOARD *game_board);
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    piece_block_y Y-coord of the block
**    piece_block_x X-coord of the block
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    Distance measured in blocks.
**
** Notes:
**
*******************************************************************************/
/* BEGIN DROP_DISTANCE */
/* .  WHILE (NEXT BLOCK BELOW HAS NOT BEEN REACHED) DO */
/* .  .  INCREMENT DISTANCE */
/* .  END WHILE */
/* .  RETURN DISTANCE */
/* END DROP_DISTANCE */

char drop_distance(char piece_block_y, char piece_block_x,
                    GAME_BOARD *game_board)
{
   char block_y;

   block_y = piece_block_y;
   while (((game_board->blocks_board[block_y][piece_block_x]
            == EMPTY) || (block_y < 0)) && (block_y < BOARD_HEIGHT))
   {
      block_y +=1;
   }
   return(block_y - piece_block_y -1 );
}


/*******************************************************************************
**
** Unit Name:
**    drop_piece
**
** Description:
**    This function moves the active Tetris piece from it's curent position
**    to a valid position closest to the bottom of the playing area without
**    shifting in either direction.
**
** Calling Convention:
**    UCHAR drop_piece(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    current_piece Information about the current active Tetris piece.
**    game_board    Pointer to the game board structure.
**
** Return Value:
**    Distance piece fell measured in blocks.
**
** Notes:
**
*******************************************************************************/
/* BEGIN DROP_PIECE */
/* .  DISTANCE = BOARD_HEIGHT */
/* .  FOR EACH BLOCK WHICH MAKES THE PIECE) DO */
/* .  .  CALL DROP_DISTANCE */
/* .  .  IF DISTANCE > DROP_DISTANCE) THEN */
/* .  .  DISTANCE = DROP_DISTANCE */
/* .  .  END IF */
/* .  END DO */
/* .  NEW POSITION IS NEW POSITION + DROP DISTANCE */
/* .  CALL MOVE_PIECE */
/* .  RETURN DROP_DISTANCE */
/* END DROP_PIECE */

UCHAR drop_piece(ACTIVE_TETRIS_PIECE *current_piece,GAME_BOARD *game_board)
{
   char drop_dis;
   char drop;
   INT1 x;
   ACTIVE_TETRIS_PIECE moved_piece;
   size_t size_of_blocks;

   drop_dis = drop_distance(current_piece->position.y,current_piece->
                             position.x,game_board);
   for (x=0;x<3;x++)
      {
         drop = drop_distance((char)(current_piece->data->block_map
                               [current_piece->rotation][x].y +
                               current_piece->position.y),
                              (char)(current_piece->data->block_map
                               [current_piece->rotation][x].x +
                               current_piece->position.x),
                               game_board);
         if (drop_dis > drop)
            drop_dis = drop;

      }

   size_of_blocks = sizeof(moved_piece);
   memcpy(&moved_piece,current_piece,size_of_blocks);
   current_piece->position.y += drop_dis;
   move_piece(&moved_piece,current_piece);
   return((UCHAR)drop_dis);

}


/*******************************************************************************
**
** Unit Name:
**    alter_options
**
** Description:
**    This function displays the options menu and allows for changes to be made
**    to the options
**
** Calling Convention:
**    UCHAR alter_options(OPTIONS_TYPE *options);
**
** Parameters:
**    Name          Description
**    --------------------------------------------------------------------------
**    options       Pointer to a structure that contains the settings of each
**                  option of the game.
**
** Return Value:
**
** Notes:
**
*******************************************************************************/
/* BEGIN ALTER_OPTIONS */
/* .  GET KEYPRESS */
/* .  WHILE (KEYPRESS IS NOT THE EXIT KEY OR RETURN KEY) DO */
/* .  .  CASE BASED ON KEYPRESS */
/* .  .  .  CASE KEYPRESS IS FOR SKILL LEVEL */
/* .  .  .  .  IF ((SKILL_LEVEL +1) > MAX_SKILL_LEVEL) THEN */
/* .  .  .  .  .  SKILL_LEVEL = MIN_SKILL_LEVEL */
/* .  .  .  .  END IF */
/* .  .  .  .  DISPLAY NEW SKILL LEVEL */
/* .  .  .  .  OPTIONS_CHANGED IS TRUE */
/* .  .  .  CASE KEYPRESS IS FOR START LEVEL */
/* .  .  .  .  IF ((START_LEVEL +1) > MAX_START_LEVEL) THEN */
/* .  .  .  .  .  START_LEVEL = MIN_START_LEVEL */
/* .  .  .  .  END IF */
/* .  .  .  .  DISPLAY NEW START LEVEL */
/* .  .  .  .  OPTIONS_CHANGED IS TRUE */
/* .  .  .  CASE KEYPRESS IS FOR PIECE PREVIEW */
/* .  .  .  .  TOGGLE PIECE PREVIEW STATUS */
/* .  .  .  .  DISPLAY NEW PREVIEW STATUS */
/* .  .  .  .  OPTIONS_CHANGED IS TRUE */
/* .  .  END CASE */
/* .  .  GET KEYPRESS */
/* .  END WHILE */
/* .  RETURN (OPTIONS_CHANGED) */
/* END ALTER OPTIONS */

UCHAR alter_options(OPTIONS_TYPE *options)
{
   UINT2 c;
   char     level[3];
   char     rows[3];
   char     piece_p[4];
   UCHAR    change_of_options;

   change_of_options = FALSE;
   c = inp_getch();

   if (font_select (MENU_FONT) < 0) {
	  exit (-1);
   }

   while((c != EXIT_KEY)&&(c != RETURN_KEY))
   {
	  switch(c)
      {
		 case KEY_SKILL_LEVEL:
			if (options->skill_lev >= MAX_SKILL_LEV)
			{
               options->skill_lev = 1;
            }
			else
            {
			   options->skill_lev +=1;
            }

           change_of_options = TRUE;

           sprintf(level,"%u",options->skill_lev);

           gr_rect(MENU_START_X + BORDER_WIDTH,
				   SKILL_NUM_START_Y + 2,
				   WINDOW_WIDTH - 2 * BORDER_WIDTH,
				   OPTIONS_ENTRY_HEIGHT - 2,
                   0,
                   GR_WHITE,
                   GR_WHITE);

           if (font_printf_centered (level,
                                     MENU_START_X,
		                             SKILL_NUM_START_Y,
           						     MENU_START_X + WINDOW_WIDTH,
		                             START_TEXT_START_Y)) {
               exit (-1);
           }
           Game_stats.timer_length = (ONE_SECOND -
              (UINT2)((float)Timer_table[options->skill_lev -1] *
                            (float)TIMER_LEVEL));
           break;

        case KEY_START_ROWS:
		   if (Options.start_lev >= MAX_START_LEV)
			  Options.start_lev = 0;
		   else
			  Options.start_lev +=1;

           sprintf(rows,"%u",options->start_lev);

           change_of_options = TRUE;
		   gr_rect(MENU_START_X + BORDER_WIDTH,
				   START_NUM_START_Y +2,
				   WINDOW_WIDTH - 2 * BORDER_WIDTH,
				   OPTIONS_ENTRY_HEIGHT - 2,
				   0,
                   GR_WHITE,
                   GR_WHITE);

           if (font_printf_centered (rows,
                                     MENU_START_X,
                                     START_NUM_START_Y,
           		                     MENU_START_X + WINDOW_WIDTH,
     	                             PIECE_PRE_TEXT_START_Y)) {
               exit (-1);
           }
           break;

        case KEY_PIECE_PRE:
          if (options->piece_pre[1] == 'n')
			 sprintf(options->piece_pre,"%s",OFF);
		  else
			 sprintf(options->piece_pre,"%s",ON);

          change_of_options = TRUE;
		  sprintf(piece_p,"%s",options->piece_pre);

		  gr_rect(MENU_START_X + BORDER_WIDTH,
				  PIECE_PRE_NUM_START_Y +2,
				  WINDOW_WIDTH - 2 * BORDER_WIDTH,
				  OPTIONS_ENTRY_HEIGHT -2,
				  0,
				  GR_WHITE,
				  GR_WHITE);

		  if (font_printf_centered (piece_p,
								   MENU_START_X,
								  PIECE_PRE_NUM_START_Y,
								 MENU_START_X + WINDOW_WIDTH,
							   PIECE_PRE_NUM_START_Y + OPTIONS_ENTRY_HEIGHT)) {
			 exit (-1);
		  }
		  break;

		default:
		  break;

	  }
	  c = inp_getch();
   }

   if (font_select (DEFAULT_FONT) < 0) {
	  exit (-1);
   }

   return(change_of_options);
}


/*******************************************************************************
**
** Unit Name:
**    display_preview
**
** Description:
**    This function displays the next piece to enter the game centered in
**    the piece preview window.
**
** Calling convention:
**    display_preview_piece();
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**
** Return value:
**
** Notes:
*******************************************************************************/
/* BEGIN DISPLAY_PREVIEW_PIECE */
/* .  DRAW THE PIECE PREVIEW WINDOW */
/* .  DISPLAY THE NEXT PIECE CENTERED IN THE WINDOW */
/* END DISPLAY_PREVIEW_PIECE */

void display_preview_piece(void)
{
/*
** Display the previewed piece background.
*/
#ifdef LCD
   gr_rect (SCORE_START_X,
		  PIECE_WINDOW_START_Y,
		  WINDOW_WIDTH,
		  PIECE_WINDOW_HEIGHT,
		  PIECE_WINDOW_BORDER_WIDTH,
		  GR_BLACK,
            GR_WHITE);
#else
   gr_bevel_rect (IN_SCORE_START_X + BORDER_WIDTH,
				PIECE_WINDOW_START_Y,
				WINDOW_WIDTH,
				PIECE_WINDOW_HEIGHT,
				BORDER_WIDTH,
				GR_GRAY, BEVEL_IN);
#endif

   display_piece(Next_pc_ptr,
			   PIECE_WINDOW_CENTER_X +
                 (INT2)(Next_pc_ptr->data->h_center_offset
			          * BLOCK_HEIGHT),
                 PIECE_WINDOW_CENTER_Y +
			   (INT2)(Next_pc_ptr->data->v_center_offset
			          * BLOCK_HEIGHT));
}


/*******************************************************************************
**
** Unit Name:
**    display_score
**
** Description:
**    This function clears the old score and displays the new score.
**
** Calling convention:
**    display_score(OPTIONS_TYPE *options, ACTIVE_TETRIS_PIECE *current_piece_ptr);
**
** Parameters:
**
**    Name              Description
**    ----              -----------
**    options           Pointer to the options structure.
**    current_piece_ptr Pointer to the current piece structure.
**
** Return value:
**
** Notes:
*******************************************************************************/
/* BEGIN DISPLAY SCORE */
/* .  ERASE PREVIOUS SCORE */
/* .  DISPLAY UPDATED SCORE */
/* END DISPLAY_SCORE */

void display_score(OPTIONS_TYPE *options,
                   ACTIVE_TETRIS_PIECE *current_piece_ptr)
{
   char score[10];

/*
** Erase the old score.
*/
#ifdef LCD
    gr_rect(SCORE_START_X + PIECE_WINDOW_BORDER_WIDTH,
	        SCORE_NUM_START_Y + 2,
		    WINDOW_WIDTH - 2 * PIECE_WINDOW_BORDER_WIDTH,
		    SCORE_ENTRY_HEIGHT,
		    0,
		    GR_WHITE,
		    GR_WHITE);
#else
    gr_rect(SCORE_START_X + BORDER_WIDTH,
	        SCORE_NUM_START_Y,
		    WINDOW_WIDTH - 2 * BORDER_WIDTH,
		    SCORE_ENTRY_HEIGHT,
		    0,
		    GR_WHITE,
		    GR_WHITE);
#endif

    Game_stats.tot_score += update_score(options,current_piece_ptr);
    sprintf(score,"%u",Game_stats.tot_score);

    if (font_printf_centered (score,
	                          SCORE_START_X,
							  SCORE_NUM_START_Y,
							  SCORE_START_X + WINDOW_WIDTH,
							  LEVEL_TEXT_START_Y)) {
	 exit (-1);
    }
}
/********** This is the end...my only friend, the end. ************************/

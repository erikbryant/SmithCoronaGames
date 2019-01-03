/*************************************************************************
** Module Name:
**     capture
**
** Description:
**
**     This is the header file for the capture module
**
** Change Log:
**
**     Date           Description
**     ----           -----------
**
**     02/06/92       Creation date
**
**************************************************************************/
#ifndef capture_h
#define capture_h

/*************************************************************************/
/**** time ****/

/*
** Define macros for accessing the clock
*/
#ifdef PWP
   #include "varstab.j"
   #define  TICKS                ((*(unsigned long *)vtab[VT_ticks]) / 63)
   #define  POINTER_TICKS        ((*(unsigned long *)vtab[VT_ticks]) / 31)
#endif


#define  ANY_NUMBER              ((INT2) (TICKS & 0x0000FFFF))

/*************************************************************************/
/**** colors ****/

   #define  BACKGROUND              GR_GRAY
   #define  BLACK                   GR_BLACK
   #define  WHITE                   GR_WHITE
   #define  FREE                    GR_NONE

/*************************************************************************/
/**** event keys ****/

/* main */
#define  NEW_MAIN                SELECT_KEY|'n'    /* New game key      */

#define  CLUE_MAIN               SELECT_KEY|'c'    /* Hint Key          */

#define  OPTIONS_MAIN            SELECT_KEY|'o'    /* Option menu key   */

#define  HELP_MAIN               SELECT_KEY|'h'    /* Help key          */
#define  HELP_NUM_KEY            '0'
#ifdef LCD
#define  PWP_HELP_KEY            CODE_KEY|'1'
#else
#define  PWP_HELP_KEY            HELP_KEY
#endif

#define  QUIT_MAIN               SELECT_KEY|'q'    /* Exit game key     */
#define  PWP_EXIT_KEY            EXIT_KEY

/* selection */

#define  MAKE_MOVE_SPACE         SPACE_KEY
#define  MAKE_MOVE_RETURN        RETURN_KEY

/* directional keys */
/* arrow keys are in KEYS.H */

#define  UP_NUM_KEY              '8'
#define  DOWN_NUM_KEY            '2'
#define  RIGHT_NUM_KEY           '6'
#define  LEFT_NUM_KEY            '4'


/* options */

/* skill */

#define  BEGIN_OPT               SELECT_KEY|'b'
#define  NOVICE_OPT              SELECT_KEY|'n'
#define  EXPERT_OPT              SELECT_KEY|'e'

/* players */

#define  ONE_OPT                 SELECT_KEY|'o'
#define  TWO_OPT                 SELECT_KEY|'t'
#define  DEMO_OPT                SELECT_KEY|'d'
#define  EXIT_OPT                EXIT_KEY

#define  DEMO_DELAY              2        /* delay between new games in demo */
/*************************************************************************/
/**** player defines ****/

#define  PLAYER1                 BLACK    /*DEFINES COLOR OF PLAYER ONE AS WHITE AS*/
                                          /*WELL AS IDENTIFIES PLAYER ONE*/
#define  PLAYER2                 WHITE

#define  OTHER_PLAYER(color)     ((INT1)(16-color))

/*************************************************************************/
/**** skill levels ****/

#define  LVL_BEGIN               1
#define  LVL_NOVICE              3
#define  LVL_EXPERT              5

/*************************************************************************/
/**** window defines ****/

#ifdef LCD
#define  BANNER_WIDTH            32

#define  STATUS_WIN_FONT         "Helvetica 16"
#define  STATUS_WIN_TEXT_OFFSET  (4)
#define  STATUS_WIN_CENTER_X     ((BRD_START_X + BANNER_WIDTH) >> 1)
#define  STATUS_WIN_CENTER_Y     (gr_scr_height() >> 1)
#define  STATUS_WIN_X            ((STATUS_WIN_CENTER_X) - (STATUS_WIN_WIDTH  >>1) - WIN_BORDER_THICKNESS)
#define  STATUS_WIN_Y            ((STATUS_WIN_CENTER_Y) - (STATUS_WIN_HEIGHT >>1) - WIN_BORDER_THICKNESS)
#define  STATUS_WIN_WIDTH        (120 + (WIN_BORDER_THICKNESS<<1))
#define  STATUS_WIN_HEIGHT       (NUM_STATUS_REGIONS * STATUS_REGION_HEIGHT)
#define  STATUS_REGION_HEIGHT    (16)
#define  NUM_STATUS_REGIONS      (4)

#define  STATUS_WIN_OUTPUT_COLUMN (55)
#define  STATUS_WIN_SCORE_COLUMN  (STATUS_WIN_OUTPUT_COLUMN + 45)

#else

#define  STATUS_WIN_FONT         "Helvetica 20"
#define  STATUS_WIN_TEXT_OFFSET  10
#define  STATUS_WIN_X            (BRD_START_X)
#define  STATUS_WIN_Y            (BRD_START_Y + (BOARD_Y*SQUARE_Y) + 24 + WIN_FRAME_WIDTH)
#define  STATUS_WIN_WIDTH        (gr_scr_width()-((BRD_START_X)<<1)) 
#define  STATUS_WIN_HEIGHT       ((f_attr.height<<1)+STATUS_WIN_TEXT_OFFSET)
#define  STATUS_REGION1_WIDTH    130
#define  STATUS_REGION2_WIDTH    130
#define  STATUS_REGION3_WIDTH    175
#define  STATUS_REGION_HEIGHT    (f_attr.height)
#endif

#ifdef LCD

#define  THINK_WIN_X             (STATUS_WIN_X + ((STATUS_WIN_WIDTH - THINK_WIN_WIDTH)>>1))
#define  THINK_WIN_Y             ((STATUS_WIN_Y + STATUS_WIN_HEIGHT) + 8)
#define  THINK_WIN_WIDTH         STATUS_WIN_WIDTH
#define  THINK_WIN_HEIGHT        20
#define  THINK_FONT              "Helvetica 16"

#else

#define  THINK_WIN_X             (MENU_START_X)

/* this define centers the thinking window between the menu and the bottom */
/* of the playing board */
#define  THINK_WIN_Y             ((Menu_y + Menu_height +\
                                    (((BRD_START_Y + (BOARD_Y*SQUARE_Y) - (Menu_y + Menu_height) - THINK_WIN_HEIGHT) >> 1))) +\
                                    (WIN_BORDER_THICKNESS<<1) + WIN_FRAME_WIDTH)
#define  THINK_WIN_WIDTH         (Menu_width)
#define  THINK_WIN_HEIGHT        30
#define  THINK_FONT              "Helvetica 20"

#endif
#endif

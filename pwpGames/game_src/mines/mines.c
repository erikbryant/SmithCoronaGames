/*****************************************************************************
**
**  Module Name:
**      Mines Alive!
**  
**  Description:
**      This module contains the bulk of the Mines Alive! program.
**
**  Exported Units:
**
**  Local Units:
**      manage_buffer
**      pix_type
**      init_board
**      display_board
**      redisplay_screen
**      display_pointer
**      move_pointer
**      blink_pointer
**      timer
**      display_mines
**      expose_tile
**      mark_tile
**      display_status
**      display_main_menu
**      display_options_menu
**      process_options
**      app_main
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


/*
** Included files for module.
*/
#include <stdio.h>                  /* free is defined there            */
#include "sc_def.h"                 /* Special definitions for the PWPs */
#include "helplib.h"                /* Constants, macros, types         */
#include "fontlib.h"                /* Fonts, font routines             */
#include "keys.h"                   /* Definition of key codes          */
#include "inp.h"                    /* Keyboard access, etc.            */
#include "gr.h"                     /* Graphics definitions             */
#include "mines.h"                  /* Type definitions                 */

#ifdef LCD
   #include "lcdpix.h"
   #include "mine_lcd.hlp"          /* Help text for the LCD            */
   #include "boardlcd.h"            /* Different LCD playing boards     */
#else
   #include "crtpix.h"
   #include "mine_crt.hlp"          /* Help text for the CRT            */
   #include "boardcrt.h"            /* Different CRT playing boards     */
#endif

/*
** Global data
*/
BOOL Paused     = FALSE;
BOOL Game_start = FALSE;


/*
** Routines.
*/


/****************************************************************************
**
**  Unit Name:
**      manage_buffer
**
**  Description: 
**
**  Calling Convention:
**      (INT1) type = pix_type (GAME_BOARD *board, INT2 y, INT2 x)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      op              What is to be done with the buffer?
**
**  Return Value:
**      BOOLean status of success of operation.
**
**  Notes:
**      none
**
*****************************************************************************/
BOOL manage_buffer (MANAGE_TYPE op, UINT1 *data1, UINT1 *data2)
{
   INT2   temp;
   BOOL   status = FALSE;
   static QUEUE *buffer;

   switch (op) {
      case ALLOCATE:
         buffer = (QUEUE *) malloc (sizeof (QUEUE)); 
         status = (buffer == NULL);
         break;
      case DEALLOCATE:
         free (buffer);
         status = TRUE;
         break;
      case INIT:
         buffer->head = 0; 
         buffer->tail = 0;
         status = TRUE;
         break;
      case PUSH:
         buffer->data[buffer->head  ][0] = (*data1);
         buffer->data[buffer->head++][1] = (*data2);
         if (temp >= BUFF_LEN) {
            temp = 0;
         }
         status = TRUE;
         break;
      case POP:
         *data1 = buffer->data[buffer->tail  ][0];
         *data2 = buffer->data[buffer->tail++][1];
         if (temp >= BUFF_LEN) {
            temp = 0;
         }
         status = TRUE;
         break;
      case EMPTY:
         status = (buffer->head == buffer->tail);
         break;
      case IN:
         temp   = buffer->tail;
         status = FALSE;
         while ((temp != buffer->head) && !status) {
            status = (buffer->data[temp][0] == (*data1) &&
                      buffer->data[temp][1] == (*data2));
            temp++;
            if (temp >= BUFF_LEN) {
               temp = 0;
            }
         }
         break;
   }

   return (status);
}

/****************************************************************************
**
**  Unit Name:
**      pix_type
**
**  Description: 
**      Given a coordinate on the board, this function returns the
**      index value of the pixmap that should be printed to reflect
**      the contents of that coordinate.
**
**  Calling Convention:
**      (INT1) type = pix_type (GAME_BOARD *board, INT2 y, INT2 x)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to a game board
**      y               coordinate on the board
**      x               coordinate on the board
**
**  Return Value:
**      The index value of the pixmap that should be displayed at
**      the (x, y) coordinate of the board.
**
**  Notes:
**      none
**
*****************************************************************************/

INT1 pix_type (GAME_BOARD *board, INT2 y, INT2 x) 
{

   if (board->board[y][x] == NO_TILE) {
      return (NO_TILE);
   }

   if (COVERED(board->board[y][x])) {
      if (MARKED(board->board[y][x])) {
         return (MARKED_PIXMAP);
      }
      if (TEST(board->board[y][x])) {
         return (TEST_PIXMAP);
      }

      return (COVERED_PIXMAP);
   }

   if (MINE(board->board[y][x])) {
      return (MINE_PIXMAP);
   }

   return (NEIGHBORS (board->board,y,x));
}




/****************************************************************************
**
**  Unit Name:
**      init_board
**
**  Description: 
**      Given a board structure and a number, take the board with
**      that number out of storage and unpack it into the structure.
**      Put a number of mines on it relative to the level.
**
**  Calling Convention:
**      status = init_board (&board, &pointer, board_number, level)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to a game board
**      pointer         pointer to the pointer
**      board_number    number of the board to select
**      level           difficulty level to set board to
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN INIT_BOARD                                                       */
/*      UNPACK INFORMATION ABOUT THE BOARD                                  */
/*      CENTER THE BOARD                                                    */
/*      UNPACK THE SELECTED BOARD INTO THE GAME BOARD                       */
/*      WHILE (NOT ENOUGH MINES ON BOARD)                                   */
/*      .  RANDOMLY PLACE A MINE ON THE BOARD                               */
/*      END WHILE                                                           */
/*   END                                                                    */

void init_board (GAME_BOARD *board, GAME_POINTER *pointer, 
                 UINT1 board_number, UINT1 level)
{
   BOOL valid;
   INT2 number_mines;
   INT2 i;
   INT2 j;
   INT2 x;
   INT2 y;
   char text[100];

   /*
   ** Copy all of the information from the packed board to the
   ** game board.
   */
   strcpy (board->name, Board_list[board_number]->name);
   board->start_x      = Board_list[board_number]->start_x;
   board->start_y      = Board_list[board_number]->start_y;
   board->board_number = board_number;
   board->level        = level;
   board->width        = Board_list[board_number]->width;
   board->height       = Board_list[board_number]->height;
   board->mines[0]     = Board_list[board_number]->mines[0];
   board->mines[1]     = Board_list[board_number]->mines[1];
   board->mines[2]     = Board_list[board_number]->mines[2];

   /*
   ** Center the board on the playing field.
   */
   board->screen_x = ((BOARD_MAX_X - BOARD_MIN_X + 1 - board->width * 
                       gr_pixmap_width (Pixmaps[COVERED_PIXMAP][0])) >> 1) + 
                     BOARD_MIN_X;
   board->screen_y = ((BOARD_MAX_Y - BOARD_MIN_Y + 1 - board->height * 
                       gr_pixmap_height (Pixmaps[COVERED_PIXMAP][0])) >> 1) + 
                     BOARD_MIN_Y;

   /*
   ** Get the location on the board that the pointer is to start at.
   */
   pointer->board_x = board->start_x;
   pointer->board_y = board->start_y;

   /*
   ** Unpack the board itself.
   */
   for (i = 0; i < MAX_BOARD_HEIGHT; i++) {
      x = 0;
      for (j = 0; j < (MAX_BOARD_WIDTH >> 1); j++) {
         board->board[i][x] = (Board_list[board_number]->board[i][j] >> 4) & 
                              0x0F;
         if (board->board[i][x] == 0) {
            board->board[i][x] = NO_TILE;
         }
         else {
            COVERED_SET (board->board[i][x]);
         }
         x++;
         board->board[i][x] = Board_list[board_number]->board[i][j] & 0x0F;
         if (board->board[i][x] == 0) {
            board->board[i][x] = NO_TILE;
         }
         else {
            COVERED_SET (board->board[i][x]);
         }
         x++;
      }
   }

   /*
   ** Initialize the number of mines.
   */
   number_mines = board->mines[board->level];
   
   /*
   ** Populate the board with the mines.  Pick a random square.
   ** If there is a mine there then pick another one, else
   ** put a mine there.  Keep doing this until there are enough
   ** mines on the board.
   */
   for (i = 0; i < number_mines; i++) {
      valid = FALSE;
      do {
         x = rand () % (board->width  + 1);
         y = rand () % (board->height + 1);
         if (board->board[y][x] != NO_TILE) {
            if (COVERED (board->board[y][x]) && !MINE (board->board[y][x])) {
               MINE_SET (board->board[y][x]);
               valid = TRUE;
            }
         }
      } while (!valid);
   }

   return;
}



/****************************************************************************
**
**  Unit Name:
**      display_board
**
**  Description: 
**      Given a board, draw it on the screen.
**
**  Calling Convention:
**      display_board (&board, &pointer, wipe_screen)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to a game board
**      pointer         pointer to the pointer
**      wipe_screen     should the screen be wiped to grey before
**                      drawing the board on it?
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN DISPLAY_BOARD                                                    */
/*      IF (GAME_PAUSED OR WIPE_SCREEN) THEN                                */
/*      .  WIPE THE PLAYING FIELD WITH A RECTANGLE                          */
/*      END IF                                                              */
/*                                                                          */
/*      IF (GAME_PAUSED) THEN                                               */
/*      .  DISPLAY A MESSAGE SAYING GAME IS PAUSED                          */
/*      ELSE                                                                */
/*      .  FOR EACH TILE ON BOARD                                           */
/*      .  .  DISPLAY THAT TILE                                             */
/*      .  END FOR                                                          */
/*      .  CALL DISPLAY_POINTER                                             */
/*      END IF                                                              */
/*   END                                                                    */

void display_board (GAME_BOARD *board, GAME_POINTER *pointer, BOOL wipe_screen)
{
   INT1  i;
   INT1  j;
   INT2  x;
   INT2  y;
   INT2  width;
   INT2  ascender;
   INT2  descender;
   UINT1 pixmap_type;

   /*
   ** Clean off the screen where the board goes.
   */
   if (Paused || wipe_screen) {
      gr_rect (BOARD_MIN_X,
               BOARD_MIN_Y,
               BOARD_MAX_X - BOARD_MIN_X + 1,
               BOARD_MAX_Y - BOARD_MIN_Y + 1,
               0, GR_GRAY, GR_GRAY);
   }

   /*
   ** If the game is paused then put up a message to that
   ** effect.  Otherwise, display the board.
   */
   if (Paused) {
      font_string_size (PAUSE_MSG, &width, &ascender, &descender);

      x = ((BOARD_MAX_X - BOARD_MIN_X - width - EXTRA_SPACE) >> 1) + 
          BOARD_MIN_X;
      y = ((BOARD_MAX_Y - BOARD_MIN_Y - ascender - 
            descender - EXTRA_SPACE) >> 1) + BOARD_MIN_Y;

      bevel_window (PAUSE_MSG,
                    x,
                    y,
                    width + EXTRA_SPACE,
                    ascender + descender + EXTRA_SPACE);
   }
   else {
      /*
      ** Display all of the tiles on the board. 
      */
      for (i = 1; i <= board->height; i++) {
         for (j = 1; j <= board->width; j++) {
            if (board->board[i][j] != NO_TILE) {
               pixmap_type = pix_type (board, i, j);
               PLOT (board->screen_y, board->screen_x, i, j,
                     Pixmaps[pixmap_type][COLOR (board->board[i][j])]);
            }
         }
      }
     
      display_pointer (pointer, board, TRUE);
   }

   return;
}

/****************************************************************************
**
**  Unit Name:
**      redisplay_screen 
**
**  Description: 
**      Assume that the screen is in an unknown state, so put everything
**      back on it.
**
**  Calling Convention:
**      redisplay_screen (&board, &pointer)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to the board
**      pointer         pointer to the pointer
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   CLEAR THE SCREEN GRAY                                                  */
/*   CALL WINDOW_TITLE                                                      */
/*   CALL DISPLAY_MAIN_MENU                                                 */
/*   CALL DISPLAY_STATUS                                                    */
/*   CALL DISPLAY_BOARD                                                     */

void redisplay_screen (GAME_BOARD *board, GAME_POINTER *pointer)
{
   gr_clear_scr (GR_GRAY);

   window_title (TITLE_NAME);

   display_main_menu (FALSE);

   display_status (DISPLAY_WINDOW, board->level);

   display_board (board, pointer, FALSE);

   return;
}

/****************************************************************************
**
**  Unit Name:
**      display_pointer
**
**  Description: 
**      Display a pixmap representing the pointer on the board.  If the
**      pointer is ON then display the pointer pixmap.  If the pointer is
**      OFF then display the underlying pixmap.  This allows the pointer
**      to flash.
**
**  Calling Convention:
**      display_pointer (&pointer, &board)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      pointer         pointer to the pointer
**      board           pointer to the board
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN DISPLAY_POINTER                                                  */
/*      IF (POINTER IS ON) THEN                                             */
/*      .  DISPLAY THE POINTER                                              */
/*      ELSE                                                                */
/*      .  DISPLAY THE PIXMAP OF THAT COORDINATE                            */
/*      END IF                                                              */
/*   END                                                                    */

void display_pointer (GAME_POINTER *pointer, GAME_BOARD *board, BOOL on)
{
   INT1 pixmap_type;
   INT1 color;

   if (on) {
      pixmap_type = POINTER_PIXMAP;
      color = 0;
   }
   else {
      pixmap_type = pix_type (board, pointer->board_y, pointer->board_x);
      color = COLOR (board->board[pointer->board_y][pointer->board_x]);
   }

   PLOT (board->screen_y, board->screen_x, 
         pointer->board_y, pointer->board_x,
         Pixmaps[pixmap_type][color]);
}

/****************************************************************************
**
**  Unit Name:
**      move_pointer
**
**  Description: 
**      Move the pointer to a new location.
**
**  Calling Convention:
**      (INT1) state = move_pointer (&board, &pointer, delta_x, delta_y)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to the board
**      pointer         pointer to the pointer
**      delta_x         x distance to move the pointer
**      delta_y         y distance to move the pointer
**
**  Return Value:
**      Value indicating whether the move was successful or not.
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN MOVE_POINTER                                                     */
/*      IF (THE COORDINATE IS NOT OFF THE SCREEN) THEN                      */
/*      .  UNPLOT THE POINTER AT THE OLD COORDINATE                         */
/*      .  PLOT THE POINTER AT THE NEW COORDINATE                           */
/*      ELSE                                                                */
/*      .  RETURN AN ERROR                                                  */
/*      END IF                                                              */
/*   END                                                                    */

INT1 move_pointer (GAME_BOARD *board, GAME_POINTER *pointer, 
                   INT1 delta_x, INT1 delta_y)
{
   INT1 x;
   INT1 y;
   INT1 status;

   x = pointer->board_x;
   y = pointer->board_y;

   /*
   ** Don't let the pointer go off of the screen.
   */
   if (board->board[y + delta_y][x + delta_x] != NO_TILE) {
      display_pointer (pointer, board, FALSE);

      pointer->board_x += delta_x;
      pointer->board_y += delta_y;

      display_pointer (pointer, board, TRUE);

      status = MINES_SUCCESS;
   }
   else {
      /*
      ** The pointer could not be moved.
      */
      status = MINES_NOT_MOVED;
   }

   return (status);
}

/****************************************************************************
**
**  Unit Name:
**      blink_pointer
**
**  Description: 
**      Toggle the pointer on and off at given intervals so it blinks.
**
**  Calling Convention:
**      blink_pointer (&pointer, &board)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      pointer         pointer to the pointer
**      board           pointer to the board
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN BLINK_POINTER                                                    */
/*      IF (NOT GAME_PAUSED) THEN                                           */
/*      .  IF (ENOUGH TIME HAS ELAPSED SINCE LAST BLINK) THEN               */
/*      .  .  TOGGLE POINTER STATUS                                         */
/*      .  .  CALL DISPLAY_POINTER                                          */
/*      .  END IF                                                           */
/*      END IF                                                              */
/*   END                                                                    */

void blink_pointer (GAME_POINTER *pointer, GAME_BOARD *board)
{
   static BOOL pointer_on = TRUE;
   static INT4 last_tick  = 0;

   /*
   ** Only flash the pointer if the game is not paused.
   */
   if (!Paused) {
      if (POINTER_TICKS != last_tick) {
         last_tick = POINTER_TICKS;
         pointer_on = !pointer_on;
         display_pointer (pointer, board, pointer_on);
      }
   }

   return;
}

/****************************************************************************
**
**  Unit Name:
**      timer
**
**  Description: 
**      Emulate a stopwatch.  Allow operations of start, stop, reset.
**      Report the current time.
**
**  Calling Convention:
**      (INT4) time = timer (op)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      op              what the timer should do
**                             TIMER_START
**                             TIMER_STOP
**                             TIMER_RESET
**                             TIMER_REPORT
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/

INT4 timer (INT1 op)
{
   static INT4 last_time    = 0;
   static INT4 start_time   = 0;
   static INT4 pause_time   = 0;
   static INT4 pause_start  = 0;
   static BOOL timer_paused = FALSE;
   INT4 now_time;

   now_time = TICKS;

   switch (op) {
      case TIMER_START:
         if (timer_paused) {
            pause_time   += now_time - pause_start;
            timer_paused  = FALSE;
         }
         break;
      case TIMER_STOP:
         if (!timer_paused) {
            pause_start  = now_time;
            timer_paused = TRUE;
         }
         break;
      case TIMER_RESET:
         start_time   = now_time;
         pause_time   = 0;
         timer_paused = FALSE;
         break;
      default:
         break;
   }

   if (!timer_paused) {
      last_time = (now_time - start_time) - pause_time;
   }

   return (last_time);
}

/****************************************************************************
**
**  Unit Name:
**      display_mines
**
**  Description: 
**      Assume the game is over.  Remove all the covers from tiles that are
**      not marked (even the ones that have test marks).  If any are marked
**      incorrectly then display a "wrong" pixmap to tell the user that.
**
**  Calling Convention:
**      display_mines (&board)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to the board
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN DISPLAY_MINES                                                    */
/*      FOR EACH CELL IN THE BOARD                                          */
/*      .  IF (CELL IS ACTUALLY A TILE) THEN                                */
/*      .  .  IF (TILE IS MARKED) THEN                                      */
/*      .  .  .  IF (TILE IS NOT A MINE) THEN                               */
/*      .  .  .  .  USER MARKED IT INCORRECTLY.  PLOT "WRONG" PIXMAP        */
/*      .  .  .  END IF                                                     */
/*      .  .  ELSE                                                          */
/*      .  .  .  TILE WAS NOT MARKED.  UNCOVER IT                           */
/*      .  .  .  IF (TILE WAS A MINE) THEN                                  */
/*      .  .  .  .  PLOT A MINE PIXMAP                                      */
/*      .  .  .  ELSE                                                       */
/*      .  .  .  .  PLOT THE NUMBER OF NEIGHBORS IT HAS                     */
/*      .  .  .  END IF                                                     */
/*      .  .  END IF                                                        */
/*      .  END IF                                                           */
/*      END FOR                                                             */
/*   END                                                                    */

void display_mines (GAME_BOARD *board)
{
   INT1  j;
   INT1  i;
   UINT1 pixmap_type;

   /*
   ** Iterate through each cell of the board array.
   */
   for (j = 1; j <= board->height; j++) {
      for (i = 1; i <= board->width; i++) {
         /*
         ** If this is part of the board then plot it.
         */
         if (board->board[j][i] != NO_TILE) {
            if (MARKED (board->board[j][i])) {
               if (!MINE (board->board[j][i])) {
                  /*
                  ** If the square was marked but did not have a mine
                  ** under it then the user got it wrong.  Display
                  ** a "wrong" pixmap.
                  */
                  PLOT (board->screen_y, board->screen_x, j, i,
                        Pixmaps[WRONG_PIXMAP][COLOR (board->board[j][i])]);
               }
            }
            else {
               /*
               ** Since the tile was not marked it can be revealed.
               */
               COVERED_UNSET (board->board[j][i]);
               if (MINE (board->board[j][i])) {
                  /*
                  ** There was a mine under the tile.  The user did not
                  ** find it, so put up a "mine" pixmap.
                  */
                  pixmap_type = MINE_PIXMAP;
               }
               else {
                  /*
                  ** There was nothing under the tile.  Put up a
                  ** count of the neighboring mines.
                  */
                  pixmap_type = NEIGHBORS (board->board, j, i);
               }
               /*
               ** Now that the pixmap type has been decided on,
               ** plot if.
               */
               PLOT (board->screen_y, board->screen_x, j, i,
                     Pixmaps[pixmap_type][0]);
            }
         }
      }
   }

   return;
}

/****************************************************************************
**
**  Unit Name:
**      expose_tile
**
**  Description: 
**      Given the board and the pointer uncover the square the pointer
**      points to.  If there is a mine under it then the user just lost.
**      If not, then the game continues.
**
**  Calling Convention:
**      (INT1) state = expose_tile (&board, &pointer)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to the board
**      pointer         pointer to the pointer
**
**  Return Value:
**      State of the game (whether the person just won, lost, or
**      if nothing happened).
**
**  Notes:
**      none
**
*****************************************************************************/
/*   BEGIN EXPOSE_TILE                                                      */
/*      IF (TILE IS MARKED) THEN                                            */
/*      .  RETURN (NEUTRAL)                                                 */
/*      END IF                                                              */
/*      IF (TILE WAS A MINE) THEN                                           */
/*      .  RETURN (LOSE)                                                    */
/*      END IF                                                              */
/*                                                                          */
/*      INITIALIZE FLOOD FILL BUFFER                                        */
/*      PUSH COORDINATES OF TILE ONTO BUFFER                                */
/*      DO                                                                  */
/*      .  POP COORDINATES                                                  */
/*      .  IF (NOT MARKED) THEN                                             */
/*      .  .  IF (NO NEIGHBORING MINES) THEN                                */
/*      .  .  .  FOR EACH NEIGHBORING TILE                                  */
/*      .  .  .  .   IF (NOT IN BUFFER ALREADY) THEN                        */
/*      .  .  .  .   .  PUSH CORRDINATES OF TILE ONTO BUFFER                */
/*      .  .  .  .   END IF                                                 */
/*      .  .  .  END FOR                                                    */
/*      .  .  END IF                                                        */
/*      .  END IF                                                           */
/*      WHILE (BUFFER IS NOT EMPTY)                                         */
/*                                                                          */
/*      COUNT = NUMBER OF TILES LEFT TO EXPOSE                              */
/*      IF (COUNT == NUMBER OF MINES) THEN                                  */
/*      .  RETURN (WIN)                                                     */
/*      ELSE                                                                */
/*      .  RETURN (NEUTRAL)                                                 */
/*      END IF                                                              */
/*   END                                                                    */

INT1 expose_tile (GAME_BOARD *board, GAME_POINTER *pointer)
{
   UINT1 x;
   UINT1 y;
   UINT1 x_tmp;
   UINT1 y_tmp;
   UINT1 neighboring;
   UINT1 pixmap_type;
   INT1  i;
   INT1  j;
   INT2  count;
   BOOL  flag;

   x = pointer->board_x;
   y = pointer->board_y;

   /*
   ** If the tile is marked then do not let the user expose it.
   */
   if (MARKED (board->board[y][x])) {
      return (NEUTRAL);
   }
   if (TEST (board->board[y][x])) {
      return (NEUTRAL);
   }

   /*
   ** If the user exposed a mine then they lose!
   */
   if (MINE (board->board[y][x])) {
      return (LOSE);
   }

   /*
   ** There was not a mine under the tile.  This might be a tile that
   ** has no neighboring mines, in which case we will have to do a 
   ** "flood fill" expose.
   */

   /*
   ** Initialize the tile buffer.
   */
   INIT_BUFF ();

   /*
   ** Push the current tile onto the buffer.
   */
   PUSH_BUFF (&y, &x);

   /*
   ** Grab tiles from the buffer and process them.  If they
   ** have no mines neighboring them then push all of their
   ** neighbors onto the buffer for processing.  Keep going
   ** until the buffer is empty.
   */
   do {
      /*
      ** Get the next tile to process.
      */
      POP_BUFF (&y, &x);

      /*
      ** If the tile is marked then we can not expose it.
      ** Skip over it.
      */
      if (!MARKED (board->board[y][x])) {
         /*
         ** If the tile has no neighboring mines then
         ** queue all of its neighbors (but not itself)
         ** for exposing processing.
         */
         neighboring = NEIGHBORS (board->board, y, x);
         if (neighboring == 0) {
            for (i = -1; i <= 1; i++) {
               for (j = -1; j <= 1; j++) {
                  /*
                  ** Don't put the current tile back in the buffer.
                  */
                  if (!(i == 0 && j == 0)) {
                     x_tmp = x + j;
                     y_tmp = y + i;
                     /*
                     ** If we have not gone off of the board, and
                     ** the tile is still covered and it has not
                     ** already been put in the buffer, then put
                     ** it in there now.
                     */
                     if ((board->board[y_tmp][x_tmp] != NO_TILE) &&
                        (COVERED (board->board[y_tmp][x_tmp]))) {
                        if (!IN_BUFF (&y_tmp, &x_tmp)) {
                           PUSH_BUFF (&y_tmp, &x_tmp);
                        }
                     }
                  }
               }
            }
         }

         /*
         ** Uncover the tile.  Update the screen to reflect this.
         */
         COVERED_UNSET (board->board[y][x]);
         pixmap_type = neighboring;
         PLOT (board->screen_y, board->screen_x, y, x,
               Pixmaps[pixmap_type][COLOR (board->board[y][x])]);
      }
   } while (!EMPTY_BUFF ());

   /*
   ** Check to see if the user just won.  The user won if all of
   ** the tiles that are on the board and that are still covered
   ** have mines under them.  The user does not have to mark all
   ** of the tiles, just to expose all the tiles that do not have
   ** mines under them.
   */
   count = 0;
   for (i = 1; i <= board->height; i++) {
      for (j = 1; j <= board->width; j++) {
         if (board->board[i][j] != NO_TILE) {
            count += COVERED (board->board[i][j]);
         }
      }
   }

   /*
   ** If the user won then return something to that effect.
   */
   if (count == board->mines[board->level]) {
      return (WIN);
   }
   else {
      return (NEUTRAL);
   }

}

/****************************************************************************
**
**  Unit Name:
**      mark_tile
**
**  Description: 
**      Toggle a mark on a tile.  It handles both regular marks and test marks.
**
**  Calling Convention:
**      mark_tile (&board, &pointer, type)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to the board
**      pointer         pointer to the pointer
**      type            type of mark this is (test mark or definite mark)
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN MARK_TILE                                                        */
/*      IF (TILE IS UNCOVERED) THEN                                         */
/*      .  RETURN                                                           */
/*      END IF                                                              */
/*                                                                          */
/*      IF (MARK_TYPE IS MINE MARKER) THEN                                  */
/*      .  UNMARK TEST MARKER                                               */
/*      .  TOGGLE MINE MARKER                                               */
/*      ELSE IF (MARK_TYPE IS TEST MARKER) THEN                             */
/*      .  IF (TILE IS MARKED WITH MINE MARKER) THEN                        */
/*      .  .  RETURN;                                                       */
/*      .  ELSE                                                             */
/*      .  .  TOGGLE TEST MARKER                                            */
/*      .  END IF                                                           */
/*      END IF                                                              */
/*                                                                          */
/*      PLOT NEW PIXMAP                                                     */
/*   END                                                                    */
/*                                                                          */

void mark_tile (GAME_BOARD *board, GAME_POINTER *pointer, INT1 type)
{
   UINT1 y;
   UINT1 x;
   UINT1 pixmap_type;

   y = pointer->board_y;
   x = pointer->board_x;

   /*
   ** If this tile is uncovered you can not mark it.
   */
   if (!COVERED (board->board[y][x])) {
      return;
   }

   /*
   ** If the tile is already marked with a mine marker then
   ** remove the marker.  (Make sure that the test marker
   ** is not set.)
   */
   if (type == MINE_MARKER) {
      TEST_UNSET (board->board[y][x]);
      MARKED_TOGGLE (board->board[y][x]);   
   } 
   else if (type == TEST_MARKER) {
      if (MARKED (board->board[y][x])) {
         /*
         ** The user is trying to put a test marker on a square
         ** that is already marked as a mine.  Don't let him do this.
         */
         return;
      }
      /*
      ** The user is toggling a test marker.
      */
      TEST_TOGGLE (board->board[y][x]);   
   }

   /*
   ** Update the screen.
   */
   pixmap_type = pix_type (board, y, x);
   PLOT (board->screen_y, board->screen_x, y, x,
         Pixmaps[pixmap_type][COLOR (board->board[y][x])]);
}

/****************************************************************************
**
**  Unit Name:
**      display_status
**
**  Description: 
**      Perform any display or update needed for the status window.
**
**  Calling Convention:
**      display_status (op, level)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      op              what needs to be updated on the display?
**                              DISPLAY_WINDOW
**                              UPDATE_TIME
**                              UPDATE_LEVEL   
**      level           what level is the game at?
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN DISPLAY_STATUS                                                   */
/*      SELECT THE PROPER FONT                                              */
/*                                                                          */
/*      SET THE LEVEL STRING TO THE PROPER VALUE                            */
/*                                                                          */
/*      IF (OP == DISPLAY) THEN                                             */
/*      .  DRAW THE BORDERS OF THE MENU                                     */
/*      .  DISPLAY THE TIME                                                 */
/*      .  DISPLAY THE LEVEL                                                */
/*      ELSE IF (OP == UPDATE_TIME) THEN                                    */
/*      .  UPDATE THE TIME WINDOW                                           */
/*      ELSE IF (OP == UPDATE_LEVEL) THEN                                   */
/*      .  UPDATE THE LEVEL WINDOW                                          */
/*      END IF                                                              */
/*   END                                                                    */

void display_status (INT1 op, INT1 level)
{
   char level_string[10];
   static char time_string[10] = "0";
   static INT4 last_time = 0;
   INT4 time;

   font_select (MINES_FONT);
   
   /*
   ** Initialize the level string.
   */
   switch (level) {
      case SKILL_BEGINNER:
         strcpy (level_string, "Beginner");
         break;
      case SKILL_NOVICE:
         strcpy (level_string, "Novice");
         break;
      case SKILL_EXPERT:
         strcpy (level_string, "Expert");
         break;
   }

   /*
   ** If the entire window needs to be displayed then put up all of the
   ** bevels, etc.
   */
   if (op == DISPLAY_WINDOW) {
#ifndef LCD
      gr_bevel_rect (STATUS_X, 
                     STATUS_Y, 
                     STATUS_WIDTH, 
                     STATUS_HEIGHT, 
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_OUT);
      
      gr_bevel_rect (STATUS_X + FLAT_WIDTH + BORDER_WIDTH, 
                     STATUS_Y + FLAT_WIDTH + BORDER_WIDTH, 
                     STATUS_WIDTH  - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 1), 
                     STATUS_HEIGHT - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 1), 
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_IN);
#endif

      gr_rect (LEVEL_X + FLAT_WIDTH + (BORDER_WIDTH << 1), 
               LEVEL_Y + FLAT_WIDTH + (BORDER_WIDTH << 1), 
               LEVEL_WIDTH  - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 2), 
               LEVEL_HEIGHT - FLAT_WIDTH - (BORDER_WIDTH << 1), 
               1,
               GR_BLACK,
               GR_WHITE);

      gr_rect (TIME_X + FLAT_WIDTH + (BORDER_WIDTH << 1), 
               TIME_Y,
               TIME_WIDTH  - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 2), 
               TIME_HEIGHT - FLAT_WIDTH - (BORDER_WIDTH << 1), 
               1,
               GR_BLACK,
               GR_WHITE);

      font_printf_centered (time_string,
                            TIME_X,
                            TIME_Y,
                            TIME_X + TIME_WIDTH,
                            TIME_Y + TIME_HEIGHT - FLAT_WIDTH - (BORDER_WIDTH << 1));

      font_printf_centered (level_string,
                            LEVEL_X,
                            LEVEL_Y,
                            LEVEL_X + LEVEL_WIDTH,
                            LEVEL_Y + LEVEL_HEIGHT + FLAT_WIDTH + (BORDER_WIDTH << 1));
   }

   /*
   ** If the time is to be updated then do so.  Blank out the square
   ** and print the new value.
   */
   if (op == UPDATE_TIME) {
      time = timer (TIMER_REPORT);

      if (last_time != time) {
         last_time = time;

         gr_rect (TIME_X + FLAT_WIDTH + (BORDER_WIDTH << 1), 
                  TIME_Y,
                  TIME_WIDTH  - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 2), 
                  TIME_HEIGHT - FLAT_WIDTH - (BORDER_WIDTH << 1),
                  1,
                  GR_BLACK,
                  GR_WHITE);
  
         sprintf (time_string, "%u", time);
         font_printf_centered (time_string,
                               TIME_X,
                               TIME_Y,
                               TIME_X + TIME_WIDTH,
                               TIME_Y + TIME_HEIGHT - (BORDER_WIDTH * 3));
      }
   }

   /*
   ** If the level is to be updated the do so.  Blank out the square
   ** and print the new value.
   */ 
   if (op == UPDATE_LEVEL) {
      gr_rect (LEVEL_X + FLAT_WIDTH + (BORDER_WIDTH << 1), 
               LEVEL_Y + FLAT_WIDTH + (BORDER_WIDTH << 1), 
               LEVEL_WIDTH  - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 2), 
               LEVEL_HEIGHT - FLAT_WIDTH - (BORDER_WIDTH << 1), 
               1,
               GR_BLACK,
               GR_WHITE);

      font_printf_centered (level_string,
                            LEVEL_X,
                            LEVEL_Y,
                            LEVEL_X + LEVEL_WIDTH,
                            LEVEL_Y + LEVEL_HEIGHT + FLAT_WIDTH + (BORDER_WIDTH << 1));

   }

   return;
}

/****************************************************************************
**
**  Unit Name:
**      display_main_menu
**
**  Description: 
**      Display the main menu.
**
**  Calling Convention:
**      display_main_menu (text_only)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      text_only       if FALSE then do not draw the beveled borders
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN DISPLAY_MAIN_MENU                                                */
/*      SELECT THE PROPER FONT                                              */
/*      IF (NOT TEXT_ONLY) THEN                                             */
/*      .  DISPLAY THE BORDERS OF THE WINDOW                                */
/*      END IF                                                              */
/*      DISPLAY THE TEXT                                                    */
/*   END                                                                    */

void display_main_menu (BOOL text_only)
{
   INT2      old_leading;
   FONT_ATTR attr;

   font_select (MINES_FONT);
   
   /*
   ** If the entire window has to be displayed then put up all of the
   ** bevels, etc.
   */
   if (!text_only) {
#ifndef LCD
      gr_bevel_rect (MENU_X, 
                     MENU_Y, 
                     MENU_WIDTH, 
                     MENU_HEIGHT, 
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_OUT);
 
      gr_bevel_rect (MENU_X + FLAT_WIDTH + BORDER_WIDTH, 
                     MENU_Y + FLAT_WIDTH + BORDER_WIDTH, 
                     MENU_WIDTH  - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 1), 
                     MENU_HEIGHT - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 1), 
                     BORDER_WIDTH,
                     GR_NONE,
                     BEVEL_IN);
#endif
   }
   
   /*
   ** White out the center of the menu.
   */
   gr_rect (MENU_X + FLAT_WIDTH + (BORDER_WIDTH << 1), 
            MENU_Y + FLAT_WIDTH + (BORDER_WIDTH << 1), 
            MENU_WIDTH  - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 2), 
            MENU_HEIGHT - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 2), 
            1,
            GR_BLACK,
            GR_WHITE);

   /*
   ** Increase the leading a little to space out the menu.
   */
   font_get_attributes (&attr);
   old_leading = attr.leading;
   attr.leading += LEADING_ADJUSTMENT;
   font_set_attributes (&attr);

   /*
   ** Print the menu choices in the window.
   */
   font_printf_centered (MAIN_MENU_TEXT,
                         MENU_X,
                         MENU_Y,
                         MENU_X + MENU_WIDTH,
                         MENU_Y + MENU_HEIGHT);

   /*
   ** Set the leading back.
   */
   attr.leading = old_leading;
   font_set_attributes (&attr);

   return;
}

/****************************************************************************
**
**  Unit Name:
**      display_options_menu
**
**  Description: 
**      Display the options menu.  If told to then only redisplay the
**      one line that has the picture name.
**
**  Calling Convention:
**      display_options_menu (&picture, picture_only)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      picture         string containing the name of the current picture
**      picture_only    should only the picture name be updated?
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN DISPLAY_OPTIONS_MENU                                             */
/*      SELECT THE PROPER FONT                                              */
/*      IF (NOT PICTURE_ONLY) THEN                                          */
/*      .  DISPLAY THE TEXT OF THE MENU                                     */
/*      END IF                                                              */
/*      DISPLAY THE CURRENTLY SELECTED PICTURE AT THE BOTTOM OF THE MENU    */
/*   END                                                                    */

void display_options_menu (UCHAR *picture, BOOL picture_only)
{
   FONT_COORD   pos;
   FONT_ATTR    attr;
   INT2         old_leading;
   static INT2  x = 0;
   static INT2  y = 0;
   static UINT1 height = 0;
   static UINT1 width  = 0;

   font_select (MINES_FONT);
   
   /*
   ** If the entire text is to be displayed then white out
   ** the whole area and display it.
   */
   if (!picture_only) {
      gr_rect (MENU_X + FLAT_WIDTH + (BORDER_WIDTH << 1), 
               MENU_Y + FLAT_WIDTH + (BORDER_WIDTH << 1), 
               MENU_WIDTH  - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 2), 
               MENU_HEIGHT - (FLAT_WIDTH << 1) - (BORDER_WIDTH << 2), 
               1,
               GR_BLACK,
               GR_WHITE);

      /*
      ** Increase the leading a little to space out the text.
      */
      font_get_attributes (&attr);
      old_leading = attr.leading;
      attr.leading += LEADING_ADJUSTMENT;
      font_set_attributes (&attr);

      font_printf_centered (OPTIONS_MENU_TEXT,
                            MENU_X,
                            MENU_Y,
                            MENU_X + MENU_WIDTH,
                            MENU_Y + MENU_HEIGHT);
 
      /*
      ** Set the leading back to what it was.
      */
      attr.leading = old_leading;
      font_set_attributes (&attr);

      font_get_pos (&pos);
 
      x = MENU_X + (MENU_WIDTH >> 1);
      y = pos.y - LEADING_ADJUSTMENT;

      /*
      ** Get the attributes of the largest possible string that
      ** can fit in this area, not just the height of the current
      ** string.
      */
      height = attr.height + attr.underline_offset;
      width  = MENU_WIDTH - (BORDER_WIDTH << 2) - (FLAT_WIDTH << 1) - 2;
   }
   
   /*
   ** Display the name of the current picture at the bottom of the
   ** window.  White out the square first.
   */
   gr_rect (x - (width >> 1), 
            y - (height >> 1), 
            width, 
            height, 
            0, GR_WHITE, GR_WHITE);
   
   /*
   ** Print the text in italics to set it off.
   */
#ifndef LCD
   font_printf ("\\i");
#endif
   font_printf_centered ((char *) picture, x, y, x, y);
#ifndef LCD
   font_printf ("\\i");
#endif

   return;
}

/****************************************************************************
**
**  Unit Name:
**      process_options
**
**  Description: 
**      Read in keystrokes and process the choices that are in the
**      options menu.
**
**  Calling Convention:
**      process_options (&board, &pointer)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      board           pointer to the board
**      pointer         pointer to the pointer
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN PROCESS_OPTIONS                                                  */
/*      CALL DISPLAY_OPTIONS_MENU                                           */
/*                                                                          */
/*      EXIT = FALSE                                                        */
/*      DO                                                                  */
/*         DO                                                               */
/*            CALL BLINK POINTER                                            */
/*            CALL DISPLAY_STATUS                                           */
/*         WHILE (NOT INP_KBHIT)                                            */
/*                                                                          */
/*         SWITCH (INP_GETCH)                                               */
/*            CASE 'b'                                                      */
/*               LEVEL = BEGINNER                                           */
/*               CALL DISPLAY_STATUS                                        */
/*               BREAK                                                      */
/*            CASE 'n'                                                      */
/*               LEVEL = NOVICE                                             */
/*               CALL DISPLAY_STATUS                                        */
/*               BREAK                                                      */
/*            CASE 'e'                                                      */
/*               LEVEL = EXPERT                                             */
/*               CALL DISPLAY_STATUS                                        */
/*               BREAK                                                      */
/*            CASE 'p'                                                      */
/*               GET NEXT BOARD NAME IN LIST                                */
/*               CALL DISPLAY_OPTIONS_MENU                                  */
/*               BREAK                                                      */
/*            CASE EXIT_KEY                                                 */
/*               EXIT = TRUE                                                */
/*               BREAK                                                      */
/*      WHILE (NOT EXIT)                                                    */
/*                                                                          */
/*      IF (ANY OPTIONS CHANGED) THEN                                       */
/*      .  CALL INIT_BOARD                                                  */
/*      END IF                                                              */
/*                                                                          */
/*      CALL DISPLAY_MAIN_MENU                                              */
/*                                                                          */
/*   END                                                                    */

void process_options (GAME_BOARD *board, GAME_POINTER *pointer) 
{
   BOOL exit = FALSE;
   UINT2 key;
   UINT1 board_num = board->board_number;
   UINT1 level = board->level;

   /*
   ** Display the entire options menu.
   */
   display_options_menu (board->name, FALSE);
   
   /*
   ** As long as the user does not exit the menu, keep processing
   ** his input.
   */
   do {
      /*
      ** Just to make things look pretty, blink the pointer and
      ** update the time.
      */
      do {
         blink_pointer (pointer, board);
         if (Game_start) {
            display_status (UPDATE_TIME, board->level);
         }
      } while (!inp_kbhit ());

      key = inp_getch ();

      switch (key) {
         case ('b' | SELECT_KEY):
            level = SKILL_BEGINNER;
            display_status (UPDATE_LEVEL, level);
            break;
         case ('n' | SELECT_KEY):
            level = SKILL_NOVICE;
            display_status (UPDATE_LEVEL, level);
            break;
         case ('e' | SELECT_KEY):
            level = SKILL_EXPERT;
            display_status (UPDATE_LEVEL, level);
            break;
         case ('p' | SELECT_KEY):
            /*
            ** Cycle through the available board names.
            */
            board_num++;
            if (board_num > MAX_BOARD) {
               board_num = 0;
            }
            display_options_menu (Board_list[board_num]->name, TRUE);
            break;
         case RETURN_KEY:
         case EXIT_KEY:
            exit = TRUE;
            break;
         default:
            break;
      }
   } while (!exit);

   /*
   ** If the user actually changed anything while in the menu
   ** a new game will be started.
   */
   if ((board_num != board->board_number) || (board->level != level)) {
      init_board (board, pointer, board_num, level);
   }

   /*
   ** Put the main menu back.
   */
   display_main_menu (TRUE);

   return;
}

/****************************************************************************
**
**  Unit Name:
**      main
**
**  Description: 
**      This is the main function of the program.
**
**  Calling Convention:
**
**  Parameters:
**      none
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/
/*                                                                          */
/*   BEGIN MAIN                                                             */
/*      CALL DISPLAY_SPLASH_SCREEN                                          */
/*      CALL INIT_BOARD                                                     */
/*      CALL REDISPLAY_SCREEN                                               */
/*      ALLOCATE SPACE FOR THE BUFFER USED IN THE EXPOSE ROUTINE            */
/*      RESET THE TIMER                                                     */
/*                                                                          */
/*      QUIT = FALSE                                                        */
/*      GAME_OVER = FALSE                                                   */
/*                                                                          */
/*      DO                                                                  */ 
/*      .  DO                                                               */
/*      .  .  CALL BLINK_POINTER                                            */
/*      .  .  IF (GAME_START) THEN                                          */
/*      .  .  .  CALL DISPLAY_STATUS                                        */
/*      .  .  END IF                                                        */
/*      .  WHILE (NOT INP_KBHIT)                                            */
/*      .                                                                   */
/*      .  GET A KEY PRESS                                                  */
/*      .  SWITCH (KEY_PRESSED)                                             */ 
/*      .  .  CASE HELP:                                                    */
/*      .  .  .  CALL DISPLAY_HELP                                          */
/*      .  .  .  CALL REDISPLAY_SCREEN                                      */
/*      .  .  CASE NEW:                                                     */
/*      .  .  .  CALL INIT_BOARD                                            */
/*      .  .  .  CALL DISPLAY_BOARD                                         */
/*      .  .  .  RESET THE TIMER                                            */
/*      .  .  .  CALL DISPLAY_STATUS                                        */
/*      .  .  CASE OPTIONS:                                                 */
/*      .  .  .  CALL OPTIONS_MENU                                          */
/*      .  .  .  IF (OPTIONS HAVE CHANGED) THEN                             */
/*      .  .  .  .  START A NEW GAME                                        */
/*      .  .  .  END IF                                                     */
/*      .  .  CASE PAUSE:                                                   */
/*      .  .  .  PAUSE = TRUE                                               */
/*      .  .  CASE QUIT:                                                    */
/*      .  .  .  QUIT = TRUE                                                */
/*      .  .  CASE MOVE_LEFT:                                               */
/*      .  .  .  MOVE POINTER LEFT ONE TILE                                 */
/*      .  .  CASE MOVE_RIGHT:                                              */
/*      .  .  .  MOVE POINTER RIGHT ONE TILE                                */
/*      .  .  CASE MOVE_UP:                                                 */
/*      .  .  .  MOVE POINTER UP ONE TILE                                   */
/*      .  .  CASE MOVE_DOWN:                                               */
/*      .  .  .  MOVE POINTER DOWN ONE TILE                                 */
/*      .  .  CASE MOVE_UP_LEFT:                                            */
/*      .  .  .  IF (CAN NOT MOVE POINTER UP_LEFT) THEN                     */
/*      .  .  .  .  IF (CAN NOT MOVE POINTER UP) THEN                       */
/*      .  .  .  .  .  MOVE POINTER LEFT ONE TILE                           */
/*      .  .  .  .  END IF                                                  */
/*      .  .  .  END IF                                                     */
/*      .  .  CASE MOVE_UP_RIGHT:                                           */
/*      .  .  .  IF (CAN NOT MOVE POINTER UP_RIGHT) THEN                    */
/*      .  .  .  .  IF (CAN NOT MOVE POINTER UP) THEN                       */
/*      .  .  .  .  .  MOVE POINTER RIGHT ONE TILE                          */
/*      .  .  .  .  END IF                                                  */
/*      .  .  .  END IF                                                     */
/*      .  .  CASE MOVE_DOWN_LEFT:                                          */
/*      .  .  .  IF (CAN NOT MOVE POINTER DOWN_LEFT) THEN                   */
/*      .  .  .  .  IF (CAN NOT MOVE POINTER DOWN) THEN                     */
/*      .  .  .  .  .  MOVE POINTER LEFT ONE TILE                           */
/*      .  .  .  .  END IF                                                  */
/*      .  .  .  END IF                                                     */
/*      .  .  CASE MOVE_DOWN_RIGHT:                                         */
/*      .  .  .  IF (CAN NOT MOVE POINTER DOWN_RIGHT) THEN                  */
/*      .  .  .  .  IF (CAN NOT MOVE POINTER DOWN) THEN                     */
/*      .  .  .  .  .  MOVE POINTER RIGHT ONE TILE                          */
/*      .  .  .  .  END IF                                                  */
/*      .  .  .  END IF                                                     */
/*      .  .  CASE MARK_TILE_TEST:                                          */
/*      .  .  .  CALL MARK_TILE                                             */
/*      .  .  CASE MARK_TILE_MINE:                                          */
/*      .  .  .  CALL MARK_TILE                                             */
/*      .  .  CASE EXPOSE_TILE:                                             */
/*      .  .  .  IF (NOT GAME_OVER && TILE THERE && TILE NOT MARKED) THEN   */
/*      .  .  .  .  STATE = CALL EXPOSE_TILE (BOARD)                        */
/*      .  .  .  .  IF (STATE == WIN) THEN                                  */
/*      .  .  .  .  .  GAME_OVER = TRUE                                     */
/*      .  .  .  .  .  CALL DISPLAY_WINNING_MESSAGE                         */
/*      .  .  .  .  END IF                                                  */
/*      .  .  .  .  IF (STATE == LOSE) THEN                                 */
/*      .  .  .  .  .  GAME_OVER = TRUE                                     */
/*      .  .  .  .  .  CALL DISPLAY_MINES                                   */
/*      .  .  .  .  END IF                                                  */
/*      .  .  .  END IF                                                     */
/*      .  END SWITCH                                                       */
/*      WHILE (NOT QUIT)                                                    */
/*                                                                          */
/*      DEALLOCATE EXPOSE TILE BUFFER                                       */
/*   END                                                                    */

app_main ()
{
   BOOL  quit      = FALSE;
   BOOL  game_over = FALSE;
   INT1  state;
   UINT1 board_number;
   UINT1 level;
   UINT2 key;
   GAME_BOARD board;
   GAME_POINTER pointer;
   INT2  return_mask_len = 1;
   UINT2 return_mask[1] = {
      RETURN_KEY
   };
#if DEBUG
   UINT1 i;
   UINT1 j;
   UINT1 pixmap_type;
#endif

   display_splash_scr (APP_NAME);
   
   srand (ANY_NUMBER);
   
   init_board (&board, &pointer, FIRST_BOARD, SKILL_BEGINNER);

   redisplay_screen (&board, &pointer);

   /*
   ** Set aside space for the buffer that is used in the expose_tile
   ** routine.
   */
   ALLOC_BUFF ();
   
   timer (TIMER_RESET);

   do {
      /*
      ** While the user is not doing anything blink the pointer so
      ** that it is easy to see.  Update the time so the user knows
      ** how long all of this is taking him.
      */
      do {
         blink_pointer (&pointer, &board);
         if (Game_start) {
            display_status (UPDATE_TIME, board.level);
         }
      } while (!inp_kbhit ());

      key = inp_getch ();
      switch (key) {
         case HELP_KEY:
#ifdef LCD
         case ('1' | CODE_KEY):
#else
         case '0':
#endif
         case ('h' | SELECT_KEY):
            /*
            ** If the game is not paused then pause the timer
            ** while the user is in help.
            */
            if ((!Paused) && (!game_over) && Game_start) {
               timer (TIMER_STOP);
            }
            help_display (GAME_HELP);
            redisplay_screen (&board, &pointer);
            if ((!Paused) && (!game_over) && Game_start) {
               timer (TIMER_START);
            }
            break;
         case ('n' | SELECT_KEY):
            if (!Paused) {
               game_over = FALSE;
               Game_start = FALSE;
               init_board (&board, &pointer, board.board_number, board.level);
               display_board (&board, &pointer, FALSE);
               timer (TIMER_RESET);
               display_status (UPDATE_TIME, board.level);
            }
            break;
         case ('o' | SELECT_KEY):
            if (!Paused) {
               if ((!game_over) && Game_start) {
                  timer (TIMER_STOP);
               }
               board_number = board.board_number;
               level = board.level;
               process_options (&board, &pointer);
               if ((!game_over) && Game_start) {
                  timer (TIMER_START);
               }
               if ((board.board_number != board_number) || 
                   (board.level != level)) {
                  game_over = FALSE;
                  Game_start = FALSE;
                  display_board (&board, &pointer, 
                                 (BOOL) !(board.board_number == board_number));
                  timer (TIMER_RESET);
                  display_status (UPDATE_TIME, board.level);
               }
            }
            break;
         case ('p' | SELECT_KEY):
            /*
            ** There are two calls to display_board here, instead
            ** of the calls being factored out of the if statement.
            ** This is so that the user is not charged time for the
            ** screen redraw when they un-pause.
            */
            if (!game_over && Game_start) {
               if (Paused) {
                  Paused = FALSE;
                  display_board (&board, &pointer, TRUE);
                  timer (TIMER_START);
               }
               else {
                  Paused = TRUE;
                  timer (TIMER_STOP);
                  display_board (&board, &pointer, TRUE);
               }
            }
            break;
         case ('q' | SELECT_KEY):
         case EXIT_KEY:
            if (!Paused) {
               if ((!Paused) && (!game_over) && Game_start) {
                  timer (TIMER_STOP);
               }
               key = prompt_exit (((BOARD_MAX_X - BOARD_MIN_X) >> 1) + 
                                  BOARD_MIN_X,
                                  ((BOARD_MAX_Y - BOARD_MIN_Y) >> 1) + 
                                  BOARD_MIN_Y);
               
               quit = ((key == ('y' | SELECT_KEY)) || 
                       (key == 'y') ||
                       (key == 'Y'));
               if ((!Paused) && (!game_over) && Game_start) {
                  timer (TIMER_START);
               }
            }
            break;
      }

      if (!game_over && !Paused) {
         display_pointer (&pointer, &board, TRUE);

         switch (key) {
            case '4':
            case CURSLEFT_KEY:
               move_pointer (&board, &pointer, LEFT);
               break;
            case '6':
            case CURSRIGHT_KEY:
               move_pointer (&board, &pointer, RIGHT);
               break;
            case '8':
            case CURSUP_KEY:
               move_pointer (&board, &pointer, UP);
               break;
            case '2':
            case CURSDOWN_KEY:
               move_pointer (&board, &pointer, DOWN);
               break;
            case '7':
               /*
               ** If the user is trying to move diagonally and can not then
               ** try to move him in some other direction.
               */
               if (move_pointer (&board, &pointer, UP_LEFT) == MINES_NOT_MOVED) {
                  if (move_pointer (&board, &pointer, UP) == MINES_NOT_MOVED) {
                      move_pointer (&board, &pointer, LEFT);
                  }
               }
               break;
            case '9':
               /*
               ** If the user is trying to move diagonally and can not then
               ** try to move him in some other direction.
               */
               if (move_pointer (&board, &pointer, UP_RIGHT) == MINES_NOT_MOVED) {
                  if (move_pointer (&board, &pointer, UP) == MINES_NOT_MOVED) {
                      move_pointer (&board, &pointer, RIGHT);
                  }
               }
               break;
            case '1':
               /*
               ** If the user is trying to move diagonally and can not then
               ** try to move him in some other direction.
               */
               if (move_pointer (&board, &pointer, DN_LEFT) == MINES_NOT_MOVED) {
                  if (move_pointer (&board, &pointer, DOWN) == MINES_NOT_MOVED) {
                      move_pointer (&board, &pointer, LEFT);
                  }
               }
               break;
            case '3':
               /*
               ** If the user is trying to move diagonally and can not then
               ** try to move him in some other direction.
               */
               if (move_pointer (&board, &pointer, DN_RIGHT) == MINES_NOT_MOVED) {
                  if (move_pointer (&board, &pointer, DOWN) == MINES_NOT_MOVED) {
                      move_pointer (&board, &pointer, RIGHT);
                  }
               }
               break;
            case '?': /* Allow the '?' key... */
            case '/': /* but do not require the shift key */
            case ' ':
               if (!Game_start) {
                  timer (TIMER_RESET);
                  timer (TIMER_START);
                  Game_start = TRUE;
               }
               mark_tile (&board, &pointer, TEST_MARKER);
               break;
            case 'm':
            case 'M':
               if (!Game_start) {
                  timer (TIMER_RESET);
                  timer (TIMER_START);
                  Game_start = TRUE;
               }
               mark_tile (&board, &pointer, MINE_MARKER);
               break;
            case 'x':
            case 'X':
               if (!Game_start) {
                  timer (TIMER_RESET);
                  timer (TIMER_START);
                  Game_start = TRUE;
               }
               switch (expose_tile (&board, &pointer)) {
                   case WIN:
                       game_over = TRUE;
                       timer (TIMER_STOP);
                       pop_up_masked (WIN_MSG, 
                                      BOARD_MIN_X,
                                      BOARD_MIN_Y,
                                      BOARD_MAX_X,
                                      BOARD_MAX_Y,
                                      return_mask,
                                      return_mask_len);
                       break;
                   case LOSE:
                       game_over = TRUE;
                       timer (TIMER_STOP);
                       display_mines (&board);
                       pop_up_masked (LOSE_MSG,
                                      BOARD_MIN_X,
                                      BOARD_MIN_Y,
                                      BOARD_MAX_X,
                                      BOARD_MAX_Y,
                                      return_mask,
                                      return_mask_len);
                       break;
                   case NEUTRAL:
                       break;
               }
               break;
#if DEBUG
            case 'c':
            case 'C':
               /*
               ** Turn off/on the cover of a tile.
               */
               COVERED_TOGGLE (board.board[pointer.board_y][pointer.board_x]);
               pixmap_type = pix_type(&board, pointer.board_y, pointer.board_x);
               PLOT (board.screen_y, board.screen_x, 
                     pointer.board_y, pointer.board_x,
                     Pixmaps[pixmap_type][COLOR (board.board[pointer.board_y]
                                                            [pointer.board_x])]);
               break;
            case 'd':
            case 'D':
               /*
               ** Toggle whether there is a mine at this square.
               */
               MINE_TOGGLE (board.board[pointer.board_y][pointer.board_x]);
               pixmap_type = pix_type(&board, pointer.board_y, pointer.board_x);
               PLOT (board.screen_y, board.screen_x, 
                     pointer.board_y, pointer.board_x,
                     Pixmaps[pixmap_type][COLOR (board.board[pointer.board_y]
                                                            [pointer.board_x])]);
               break;
#endif
         }
      }

   } while (!quit);

   /*
   ** Deallocate the buffer that is used by the expose_tile routine.
   */
   DEALLOC_BUFF ();

   return (0);
}


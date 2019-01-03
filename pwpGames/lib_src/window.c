/*****************************************************************************
**
**  Module Name:
**      window.c
**
**  Description:
**      This module provides functions for pop-up windows.
**
**  Exported Units:
**      pop_up_window
**      backing_store
**      backing_restore
**      bevel_window
**      prompt_exit
**
**  Local Units:
**      in_list
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
**      05/20/92    Initial version
**
**
*****************************************************************************/

   #include "varstab.j"
#include <stdio.h>
#include "fontlib.h"
#include "window.h"
#include "keys.h"
#include "inp.h"

#define YES_OR_NO_TXT           "\\n\\uY\\ues or \\uN\\uo"
#define QUIT_TXT                "Are you sure\\nyou want to quit?" YES_OR_NO_TXT

#ifdef LCD
   #define EXIT_FONT            "Helvetica 16"
#else 
   #define EXIT_FONT            "Helvetica 30"
#endif

   #define TICKS                (*(unsigned long *) vtab[VT_ticks])
   #define CLOCKS_PER_SEC       63

#ifdef LCD
   #define POP_UP_BORDER        2
   #define POP_UP_SHADOW_WIDTH  3
#else
   #define POP_UP_BORDER        3
#endif

#define POP_UP_FLAT             5
#define POP_UP_SPACING          8

#define SHADOW_WIDTH            POP_UP_FLAT

#define BACKING_MAGIC_NUMBER    0xEB

static BOOL in_list (UINT2 key, UINT2 *mask, INT2 mask_len)
{
   INT2 x;

   for (x = 0; x < mask_len; x++) {
      if (mask[x] == key) {
         return (TRUE);
      }
   }

   return (FALSE);
}



/*****************************************************************************
**
**  Unit Name:
**      pop_up_window
**
**  Description:
**      Pops up a window that contains the given text.  The region behind
**      the window is restored when the window is removed.  See the macros
**      that have been defined to access this more easily.
**
**  Calling convention:
**      (INT2) status = pop_up_window (char *text, INT2 x1, INT2 y1, 
**                                     INT2 x2, INT2 y2, INT1 op,
**                                     UINT1 time_out_val, UINT2 *mask,
**                                     INT2 mask_len)
**
**  Parameters:
**
**      Name          Description
**      ----          -----------
**      text          Text to be displayed in the window.
**      x1            Upper left corner of rectangle to center window in.
**      y1            Upper left corner of rectangle to center window in.
**      x2            Lower right corner of rectangle to center window in.
**      y2            Lower right corner of rectangle to center window in.
**      op            What kind of window this is.
**                       TIME - window stays up for a given time
**                       KEY  - window stays up until a key is pressed
**                       TIME_KEY - window dies at timeout or keypress
**      time_out_val  How long before the window kills itself.
**      mask          Pointer to a list of valid keys a user can press.
**      mask_len      Number of valid keys in list.
**
**  Return Value:
**      If a key was pressed to pull down the window, then the value
**      of that key, else nothing.
**
**  Notes:
**      None
**
*****************************************************************************/

INT2 pop_up_window (char *text, INT2 x1, INT2 y1, INT2 x2, INT2 y2, 
                    INT1 op, UINT1 time_out_val, UINT2 *mask, INT2 mask_len)
{
   INT2 i;
   INT2 width;
   INT2 ascender;
   INT2 descender;
   INT2 in_x;
   INT2 in_y;
   INT2 in_width;
   INT2 in_height;
   INT2 out_x1;
   INT2 out_y1;
   INT2 out_x2;
   INT2 out_y2;
   INT2 out_width;
   INT2 out_height;
   INT2 key;
   BOOL time_is_up;
   BOOL key_pressed;
   INT4 start_time;
   BACKING_HANDLE *backing;

   /*
   ** Get the size of the string to be printed so that the window
   ** can be sized to fit it.
   */
   if (font_string_size (text, &width, &ascender, &descender)) {
      return (WINDOW_ERROR);
   }

   /*
   ** Width of the window not including the borders.
   */
   in_width  = width + (POP_UP_SPACING << 1);
   in_height = ascender + descender + (POP_UP_SPACING << 1);

   /*
   ** Coordinates of the upper left corner of the window 
   ** (not including the borders).
   */
   in_x = ((x2 - x1 + 1 - in_width ) >> 1) + x1;
   in_y = ((y2 - y1 + 1 - in_height) >> 1) + y1;

   /*
   ** Coordinates of the upper left corner of the window.
   */
#ifdef LCD
   /*
   ** On the LCD the borders are just lines.
   */
   out_x1 = in_x - (POP_UP_BORDER << 1);
   out_y1 = in_y - (POP_UP_BORDER << 1);

   out_x2 = in_x + in_width  + (POP_UP_BORDER << 1) + SHADOW_WIDTH;
   out_y2 = in_y + in_height + (POP_UP_BORDER << 1) + SHADOW_WIDTH;
#else
   /*
   ** On the CRT the borders are bevels.
   */
   out_x1 = in_x - (POP_UP_BORDER << 1) - POP_UP_FLAT - 1;
   out_y1 = in_y - (POP_UP_BORDER << 1) - POP_UP_FLAT - 1;

   out_x2 = in_x + in_width  - 1 + (POP_UP_BORDER << 1) + 
            POP_UP_FLAT + 1 + SHADOW_WIDTH;
   out_y2 = in_y + in_height - 1 + (POP_UP_BORDER << 1) + 
            POP_UP_FLAT + 1 + SHADOW_WIDTH;
#endif
   
   /*
   ** Width of the window including the borders.
   */
   out_width  = out_x2 - out_x1 + 1;
   out_height = out_y2 - out_y1 + 1;
   
   /*
   ** Store what is behind the window.
   */
   backing = backing_store (out_x1, out_y1, out_x2, out_y2);
   if (backing == NULL) {
      return (WINDOW_ERROR);
   }

   /*
   ** Draw the window.
   */
   bevel_window (text, 
                 out_x1, 
                 out_y1, 
                 out_width  - SHADOW_WIDTH, 
                 out_height - SHADOW_WIDTH);

   /*
   ** If this is a timed window, pull it down after the elapsed time.
   ** If this is a key-press window then pull it down after a key is
   ** pressed (other than the select key).
   */
   if (op == TIME) {
      /*
      ** This pop-up window stays on the screen for a given time.
      ** Wait that long and then go away.
      */
      waitsecs (time_out_val);
   }
   else if (op == KEY) {
      /*
      ** Flush the keyboard buffer.
      */
      inp_flush ();

      /*
      ** If there is a mask then only allow keys from that mask.
      */
      if (mask_len > 0) {
         do {
            key = inp_getch ();
         } while (!in_list (key, mask, mask_len));
      }
      else {
         /*
         ** When you release the select key it generates a code.
         ** Consume this just in case the pop-up was activated
         ** by a select key combnation.
         */
         do {
            key = inp_getch ();
         } while (key == SELECT_OUT_KEY);
      }
   }
   else if (op = TIME_KEY) {
      /*
      ** Start the timer.
      */
      start_time = TICKS;

      /*
      ** Just in case the user times out, don't return junk.
      */
      key = TIME_OUT_VAL;

      /*
      ** Flush the keyboard buffer.
      */
      inp_flush ();

      /*
      ** If there is a mask then only allow keys from that mask.
      */
      if (mask_len > 0) {
         time_is_up = FALSE;
         do {
            if (inp_kbhit ()) {
               key = inp_getch ();
            }
            else {
               if (TICKS > (start_time + (time_out_val * CLOCKS_PER_SEC))) {
                  time_is_up = TRUE;
               }
            }
         } while ((!in_list (key, mask, mask_len)) && !time_is_up);
      }
      else {
         /*
         ** When you release the select key it generates a code.
         ** Consume this just in case the pop-up was activated
         ** by a select key combnation.
         */
         time_is_up = FALSE;
         key_pressed = FALSE;
         do {
            if (inp_kbhit ()) {
               key = inp_getch ();
               if (key == SELECT_OUT_KEY) {
                  key = TIME_OUT_VAL;
               }
               else {
                  key_pressed = TRUE;
               }
            }
            else {
               if (TICKS > (start_time + (INT4) (time_out_val * CLOCKS_PER_SEC))) {
                  time_is_up = TRUE;
               }
            }
         } while ((!key_pressed) && !time_is_up);
      }
   }

   /*
   ** Restore what was behind the window.
   */
   if (backing_restore (backing)) {
      return (WINDOW_ERROR);
   }

   return (key);
}



/*****************************************************************************
**
**  Unit Name:
**      backing_store
**
**  Description:
**      This function performs a backing store on the specified area.
**
**  Calling convention:
**      (BACKING_HANDLE *) handle = backing_store (INT2 x1, INT2 y1,
**                                                 INT2 x2, INT2 y2)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      x1          Upper left corner of region to store.
**      y1          Upper left corner of region to store.
**      x2          Lower right corner of region to store.
**      y2          Lower right corner of region to store.
**
**  Return Value:
**      Pointer to the structure that stores the backing information.
**
**  Notes:
**      None
**
*****************************************************************************/

BACKING_HANDLE * backing_store (INT2 x1, INT2 y1, INT2 x2, INT2 y2)
{
   INT4 byte_x1;
   INT4 byte_y1;
   INT4 byte_x2;
   INT4 byte_y2;
   INT4 index;
   INT4 offset;
   INT4 i;
   INT4 j;
   INT4 backing_size;
   BACKING_HANDLE *backing;

   /*
   ** Malloc space for the backing handle structure.
   */
   backing = (BACKING_HANDLE *) malloc (sizeof (BACKING_HANDLE));

   /*
   ** If the space could not be allocated then abort.
   */
   if (backing == NULL) {
      return (NULL);
   }

   /*
   ** Find the bounding bytes of the area to be stored.  Err on the 
   ** side of being too large.
   */
   byte_x1 = (x1 >> 3) + LEFT_VID_BUF;
   byte_x2 = (x2 >> 3) + 1 + LEFT_VID_BUF;
   if (byte_x2 > (MAX_X >> 3)) {
      byte_x2--;
   }
   byte_y1 = y1;
   byte_y2 = y2;

   /*
   ** Malloc space to store the backing.
   */
   backing_size  = (byte_x2 - byte_x1 + 1) * (byte_y2 - byte_y1 + 1);
   backing->data = (UCHAR *) malloc (backing_size);

   /*
   ** If the space could not be allocated then abort.
   */
   if (backing->data == NULL) {
      return (NULL);
   }

   /*
   ** Copy the area from the screen into the malloced space.
   ** This could probably be sped up with a memcpy.
   */
   index = 0;
   for (i = byte_y1; i <= byte_y2; i++) {
      for (j = byte_x1; j <= byte_x2; j++) {
         offset = (i * VID_BUF_WIDTH) + j;
         backing->data[index++] = vidbuf[offset];
      }
   }
   
   /*
   ** If we got this far then assume everything was successful.
   ** Mark the handle as being valid.  Save information about the
   ** bounding region.
   */
   backing->valid   = BACKING_MAGIC_NUMBER;
   backing->byte_x1 = byte_x1;
   backing->byte_y1 = byte_y1;
   backing->byte_x2 = byte_x2;
   backing->byte_y2 = byte_y2;

   /*
   ** Return the handle to the caller.
   */
   return (backing);
}




/*****************************************************************************
**
**  Unit Name:
**      backing_restore
**
**  Description:
**      This function restores a backing that has already been stored.
**      A backing is freed after it is restored, and can thus only be
**      restored once.
**
**  Calling convention:
**      (INT1) status = backing_restore (BACKING_HANDLE *backing)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      backing     Pointer to a handle that was created by backing_store.
**
**  Return Value:
**      0 if successful, non-zero otherwise.
**
**  Notes:
**      None
**
*****************************************************************************/

INT1 backing_restore (BACKING_HANDLE *backing)
{
   INT4 index;
   INT4 offset;
   INT4 i;
   INT4 j;
   
   /*
   ** If this is not a valid handle then abort.
   */
   if (backing->valid != BACKING_MAGIC_NUMBER) {
      return (WINDOW_ERROR);
   }
   
   /*
   ** Copy the information from the storage to the screen.
   ** This could probably be sped up with a memcpy.
   */
   index = 0;
   for (i = backing->byte_y1; i <= backing->byte_y2; i++) {
      for (j = backing->byte_x1; j <= backing->byte_x2; j++) {
         offset = (i * VID_BUF_WIDTH) + j;
         vidbuf[offset] = backing->data[index++];
      }
   }

   /*
   ** Return the space to the OS.
   */
   free (backing->data);
   free (backing);

   return (WINDOW_SUCCESS);
}



/*****************************************************************************
**
**  Unit Name:
**      bevel_window
**
**  Description:
**      This function draws a beveled window with the given text 
**      centered in it.
**
**  Calling convention:
**      bevel_window (char *text, INT2 x1, INT2 y1, INT2 width, INT2 height)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      text        Pointer to a string to be printed
**      x1          Upper left corner of the window
**      y1          Upper left corner of the window
**      width       Width of the window
**      height      Height of the window
**
**  Return Value:
**      None
**
**  Notes:
**      None
**
*****************************************************************************/

void bevel_window (char *text, INT2 x1, INT2 y1, INT2 width, INT2 height)
{

#ifdef LCD
   /*
   ** On the LCD the borders are just lines.  Put shadow lines
   ** on the right and bottom sides of the window so that it
   ** looks raised.
   */
   gr_rect( x1,
            y1,
            width  - POP_UP_SHADOW_WIDTH,
            height - POP_UP_SHADOW_WIDTH,
            1,
            GR_BLACK,
            GR_WHITE);

   gr_line( x1 + (POP_UP_SHADOW_WIDTH<<1),
            y1 + height - POP_UP_SHADOW_WIDTH,
            x1 + width  - 1,
            y1 + height - POP_UP_SHADOW_WIDTH,
            POP_UP_SHADOW_WIDTH,
            GR_BLACK);

   gr_line( x1 + width  - POP_UP_SHADOW_WIDTH,
            y1 + (POP_UP_SHADOW_WIDTH<<1),
            x1 + width  - POP_UP_SHADOW_WIDTH,
            y1 + height - 1,
            POP_UP_SHADOW_WIDTH,
            GR_BLACK);

#else
   /*
   ** On the CRT the borders are bevels.
   */
   gr_rect (x1, 
            y1, 
            width,
            height,
            1,
            GR_BLACK,
            GR_NONE);

   gr_bevel_rect (x1 + 1,
                  y1 + 1,
                  width  - 2,
                  height - 2,
                  POP_UP_BORDER,
                  GR_NONE,
                  BEVEL_OUT);


   gr_rect (x1 + 1 + POP_UP_BORDER, 
            y1 + 1 + POP_UP_BORDER, 
            width  - 2 - (POP_UP_BORDER << 1),
            height - 2 - (POP_UP_BORDER << 1),
            POP_UP_FLAT,
            GR_GRAY,
            GR_NONE);

   gr_bevel_rect (x1 + 1 + POP_UP_BORDER + POP_UP_FLAT,
                  y1 + 1 + POP_UP_BORDER + POP_UP_FLAT,
                  width  - 2 - (POP_UP_BORDER << 1) - (POP_UP_FLAT << 1),
                  height - 2 - (POP_UP_BORDER << 1) - (POP_UP_FLAT << 1),
                  POP_UP_BORDER,
                  GR_NONE,
                  BEVEL_IN);

   gr_rect (x1 + 1 + (POP_UP_BORDER << 1) + POP_UP_FLAT,
            y1 + 1 + (POP_UP_BORDER << 1) + POP_UP_FLAT,
            width  - 2 - (POP_UP_BORDER << 2) - (POP_UP_FLAT << 1),
            height - 2 - (POP_UP_BORDER << 2) - (POP_UP_FLAT << 1),
            1,
            GR_BLACK,
            GR_WHITE);
#endif

   /*
   ** Center each line.
   */
   font_printf_centered_each (text, 
                              x1,
                              y1,
                              x1 + width,
                              y1 + height);
}



/*****************************************************************************
**
**  Unit Name:
**      prompt_exit
**
**  Description:
**      This function pops up a window with a message in it that asks the
**      user if they are sure they want to quit.  It returns the key that
**      is pressed.
**
**  Calling convention:
**      (UINT2) prompt_exit (INT2 x1, INT2 y1)
**
**  Parameters:
**
**      Name        Description
**      ----        -----------
**      x1          X coordinate of center of window
**      y1          Y coordinate of center of window
**
**  Return Value:
**      Key that was pressed.
**
**  Notes:
**      None
**
*****************************************************************************/

UINT2 prompt_exit (INT2 x1, INT2 y1)
{
   INT2  keys_len = 6;
   UINT2 keys[6] = {
      SELECT_KEY | 'y',
      'y',
      'Y',
      SELECT_KEY | 'n',
      'n',
      'N',
   };

   font_select (EXIT_FONT);
   return (pop_up_masked (QUIT_TXT, x1, y1, x1, y1, keys, keys_len));
}

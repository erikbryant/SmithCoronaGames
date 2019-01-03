/*****************************************************************************
**
**  Header Name:
**      window.h
**
**  Description:
**      This header has the defintions for the pop-up windows.
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

#ifndef window_h
#define window_h

#include "sc_def.h"    /* Miscellaneous type definitions */
#include "gr.h"        /* Graphics definitions */

/*
** Status values.
*/
#define WINDOW_SUCCESS  0
#define WINDOW_ERROR   -1

/*
** Different events that can cause a window to be pulled down.
*/
#define KEY             0
#define TIME            1
#define TIME_KEY        2

/*
** If the TIME_KEY window times out then there still has to be something
** returned as the keystroke.
*/
#define TIME_OUT_VAL    0xFFFF

/*
** A struct to hold backing information.
*/
typedef struct backing_handle {
   INT2 valid;
   INT2 byte_x1;
   INT2 byte_y1;
   INT2 byte_x2;
   INT2 byte_y2;
   UCHAR *data;
} BACKING_HANDLE;

/*
** Convenience routines for the different types of pop-up windows.
*/
#define pop_up(text,x1,y1,x2,y2)                            pop_up_window (text, x1, y1, x2, y2,      KEY,    0,    0,   0)
#define pop_up_masked(text,x1,y1,x2,y2,mask,len)            pop_up_window (text, x1, y1, x2, y2,      KEY,    0, mask, len)
#define pop_up_timed(text,time,x1,y1,x2,y2,mask,len)        pop_up_window (text, x1, y1, x2, y2, TIME_KEY, time, mask, len)
#define flash_up(text,time,x1,y1,x2,y2)              (void) pop_up_window (text, x1, y1, x2, y2,     TIME, time,    0,   0)

/*
** Function prototypes.
*/
INT2  pop_up_window (char *text, INT2 x1, INT2 y1, INT2 x2, INT2 y2, INT1 op, 
                     UINT1 time_out_val, UINT2 *mask, INT2 mask_len);
BACKING_HANDLE *backing_store (INT2 x1, INT2 y1, INT2 x2, INT2 y2);
INT1  backing_restore (BACKING_HANDLE *backing);
void  bevel_window (char *text, INT2 x1, INT2 y1, INT2 width, INT2 height);
UINT2 prompt_exit (INT2 x1, INT2 y1);

#endif

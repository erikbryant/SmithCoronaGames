/*****************************************************************************
**
**  Module Name:
**      inp.c
**
**  Description:
**      This module provides functions that are used to access input
**      from the keyboard.
**
**  Exported Units:
**      inp_open
**      inp_close
**      inp_getch
**      inp_kbhit
**      inp_flush
**
**  Local Units:
**      none
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
**      02/22/92    Initial version
**
*****************************************************************************/

/******************************  INCLUDE FILES  *****************************/


   #include "winmacro.h"

#include "inp.h"

 
/*********************************  GLOBALS  ********************************/


/**********************************  UNITS  *********************************/



/*****************************************************************************
**
**  Unit Name:
**      inp_open
**
**  Description:
**      This function performs the initialization that is necessary
**      before the keyboard can be accessed.
**
**  Calling convention:
**      inp_open ()
**
**  Parameters:
**      None
**
**  Return Value:
**      None
**
**  Notes:
**      This function is implemented only for PWP applications.  On
**      the PC version, it is implemented as a macro which expands to
**      nothing since initialization is not necessary for the PC.
**
*****************************************************************************/

void inp_open(void)
{
   commKeysOpen (20, 0, 0);
   winWTextCursorStop ();
}



/*****************************************************************************
**
**  Unit Name:
**      inp_getch
**
**  Description:
**      This function returns one keypress from the keyboard.  Keys
**      which have extended attributes are returned as a 16-bit
**      code.  The extended attribute (such as ALT on a PC, or
**      select on the PWP) occupies the upper byte, with the character
**      code in the lower byte.
**
**  Calling convention:
**      (int) char = inp_getch ()
**
**  Parameters:
**      None
**
**  Return Value:
**      Character code
**            - lower byte contains the character code
**            - upper byte contains 0 for normal codes, or
**              an extended code
**
**  Notes:
**      None
**
*****************************************************************************/




/*****************************************************************************
**
**  Unit Name:
**      inp_kbhit
**
**  Description:
**      This function determines whether a key has been hit on
**      the keyboard.
**
**  Calling convention:
**      (BOOL) key_waiting = inp_kbhit ()
**
**  Parameters:
**      None
**
**  Return Value:
**      Returns TRUE if a key has been hit, FALSE otherwise.
**
**  Notes:
**      None
**
*****************************************************************************/




/*****************************************************************************
**
**  Unit Name:
**      inp_flush
**
**  Description:
**      This function flushes the keyboard buffer.
**
**  Calling convention:
**      inp_flush ()
**
**  Parameters:
**      None
**
**  Return Value:
**      None
**
**  Notes:
**
*****************************************************************************/


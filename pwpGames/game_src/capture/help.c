/*************************************************************************
** Module Name:
**
**    help
**
** Description:
**
**    This module simply includes the help structure file
**    and calls the HELPLIB help routine.
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    04/06/92       Creation date
**
**************************************************************************/

/*
*************************  INCLUDED FILES  *******************************
*/

#include "help.h"
#include "helplib.h"


#ifdef LCD
#include "caplcd.hlp"   /* help file structured for the LCD */
#else
#include "capcrt.hlp"   /* help file structured for the CRT or PC */
#endif

/***************************************************************************
** Unit Name:
**    help
**
** Description:
**    Calls the HELPLIB help_display function with the game_help structure
**
** Parameters:
**    NONE
**
** Return Value:
**    NONE
**
** Notes:
**
***************************************************************************/
void  help(void)
{
   help_display(GAME_HELP);
}


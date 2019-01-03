/*****************************************************************************
**
**  Module Name:
**      pwp.c
**
**  Description:
**      This module provides application startup services, PWP machine
**      identification, and application shutdown services.
**
**  Exported Units:
**      main
**
**  Local Units:
**
**  Exported Data:
**
**  Notes:
**      This module contains an implementation of main() because the
**      entry point of applications must be app_main().
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      02/22/92    Initial version
**
*****************************************************************************/

/******************************  INCLUDE FILES  *****************************/


#include "sc_def.h"
#include "gr.h"
#include "inp.h"

/*****************************************************************************
**
**  Unit Name:
**      pwp_verify_model
**
**  Description:
**      Checks to make sure that this is running on the model that it
**      was compiled for.
**
**  Calling convention:
**      pwp_verify_model ()
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
BOOL pwp_verify_model ()
{
   UINT1 width;
   UINT1 height;
   UINT1 i;
   UINT1 j;
   UINT1 x;
   UINT1 y;
   char *msg[] = {
      "*** ERROR ***",
      "Application not designed to operate on this PWP model.",
      "Press any key to return to the Word Processor..."
   };
   UINT1 msg_len;

   /*
   ** Determine screen width, height, etc.
   */
   gr_get_info ();

   /*
   ** If this is not the model we are compiled for then signal
   ** an error and return to the word processor.
   */
   if ((gr_scr_width()  != gr_real_scr_width()) ||
       (gr_scr_height() != gr_real_scr_height())) {
      /*
      ** This needs keyboard input.
      */
      inp_open ();

      /*
      ** Open a text window to print a message in.
      */
      gr_init ();

      /*
      ** Get the size of the screen so the text can be centered.
      */
      width  = rawMaxCol ();
      height = rawMaxRow ();

      /*
      ** Find where the first line starts.
      */
      msg_len = sizeof (msg) / sizeof (char *);
      y = ((height - msg_len) >> 1) + 1;

      i = 0;
      while (i < msg_len) {
         /*
         ** Go to the next line.
         */
         rawSetRow (y++);
   
         /*
         ** Find where the message has to start to center it.
         ** Go there.
         */
         x = ((width - strlen (msg[i])) >> 1) + 1;
         rawSetCol (x);

         /*
         ** Display the message.
         */
         j = 0;
         while (msg[i][j] != '\0') {
            rawPutChr (msg[i][j]);
            j++;
         }

         i++;
      }

      /*
      ** Flush the keyboard buffer and then wait for a key.
      */
      inp_flush ();
      inp_getch ();

   #ifdef LCD
      /*
      ** If this is being compiled for the LCD then you will only
      ** get a compatibility problem if you run it on a CRT.
      ** The CRT models do not have to reboot to reset.
      */
      inp_close();
      
      rawClrEow();
      rawWinClose();

      execTRest ();

      return (FALSE);
   #else
      /*
      ** If this is not being compiled for the LCD then you will only
      ** get a compatibility problem if you run it on an LCD, 
      ** therefore you will need to reboot.  The CRT models do not have
      ** to reboot to reset.
      */
      warm_boot ();
   #endif
   }

   return (TRUE);
}

/*****************************************************************************
**
**  Unit Name:
**      main
**
**  Description:
**      Main function for all PWP applications.
**
**  Calling convention:
**      this function is called by the system
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

void main(void)
{
   /*
   ** Verify that this is running on the correct model PWP.
   ** If this is not the correct model, this call will not return.
   */
   if (pwp_verify_model ()) {
      /*
      ** Set up the environment in which to run the PWP application
      ** on the PC.
      */
      gr_init();
      inp_open();

      /*
      ** Invoke the application.
      */
      app_main();
   
      /*
      ** Close out the application environment.
      */
      inp_close();
      gr_close();
   }

   return;
}



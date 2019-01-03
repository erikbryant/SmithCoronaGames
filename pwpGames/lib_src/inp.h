/*****************************************************************************
**
**  Header Name:
**      inp.h
**
**  Description:
**      This the header file for the PWP keyboard input module.
**
**  Notes:
**      None
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      02/22/92    Initial version
**
*****************************************************************************/

#ifndef INP_H_INCLUDED
#define INP_H_INCLUDED

/*
** Define the interface function prototypes
*/

   /*
   ** These 2 functions already exist on the PWPs.
   */
   extern int commKeysGet (void);
   extern int commKeysHit (void);

   extern void inp_open (void);
   #define inp_getch()        commKeysGet ()
   #define inp_kbhit()        commKeysHit ()
   #define inp_flush()        for (; inp_kbhit(); inp_getch())
   #define inp_close()        commKeysClose ()


#endif

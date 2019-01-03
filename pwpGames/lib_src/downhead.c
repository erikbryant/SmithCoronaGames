/*****************************************************************************
**
**  Module Name:
**      downhead.c
**
**  Description:
**      This module defines the header for the downloadable application.
**
**  Exported Units:
**      none
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
**
*****************************************************************************/

/*
** The following pragma sets the data section name to downhead for this 
** source file so it can be forced as the first section linked.
*/
#pragma option -NIdownhead

#include "types.h"     /* Type definitions */
#include "download.h"  /* Defines "struct download_header" */

extern int main (int argc, char *argv[]);

struct download_header my_prog =
   {
      { "SMITH CORONA CORP" },            /* Downloadable header */
      sizeof (struct download_header),    /* Size of this header */
      0L,              /* Loader version */
      0L,              /* Fixup program version */
      0L,              /* Number of fixups */
      0L,              /* Fixup table start address */
      (ULONG *) main,  /* Program start address */
      0x20435243,      /* CRC */
      2048,            /* Download application stack size */
      100,             /* Download task priority */
      0,               /* 1 if dirty or hostile download application */
      0,               /* 1 if warmstart required after completetion */
      0,               /* Download application initial stack size */
      { 0 }            /* Reserved */
   };

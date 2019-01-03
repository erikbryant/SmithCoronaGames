/*****************************************************************************
**
**  Header Name:
**      fileio.h
**
**  Description:
**      This the header file for the PWP file I/O module.
**
**  Notes:
**      None
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      05/20/92    Initial version
**
*****************************************************************************/

#ifndef FILEIO_H_INCLUDED
#define FILEIO_H_INCLUDED

#include "types.h"
#include "disk.h"


/*
** The disk routines are asynchronous.  You have to tell them to wait for
** completion if you need the results right after the call.
*/
#define NO_SUSPEND               0
#define SUSPEND                  1

/*
** Define the interface function prototypes.  The PWP versions are 
** implemented as macros for efficiency.
*/
   #define fileio_open(name,mode,handle,suspend)     fileOpen  (name,mode,handle,suspend)
   #define fileio_close(handle,suspend)              fileClose (handle,suspend)
   #define fileio_seek(handle,offset,origin,suspend) fileSeek  (handle,offset,origin,suspend)
   #define fileio_read(handle,data,count,rc,suspend) fileRead  (handle,data,count,rc,suspend)
   #define fileio_clearerror(handle,suspend)         fileClearError (handle,suspend)



#endif

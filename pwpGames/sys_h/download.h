/****************************************************************************
*
*		$Workfile:   download.h  $	- Structure definition for downloadable application program header
*
*		$Revision:   1.7  $			$Modtime:   11 Sep 1991 10:31:00  $
*
*		$Author:   SteveP  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/download.h_v  $
* 
*    Rev 1.7   11 Sep 1991 10:42:38   SteveP
* Removed min stack size and init A5 values.
* 
*    Rev 1.6   10 Sep 1991 21:07:10   SteveP
* Added initial A5 value to header.
* 
*    Rev 1.5   10 Sep 1991 20:49:28   SteveP
* Changed dirty and warmstart flags to UCHAR.
* 
*    Rev 1.4   10 Sep 1991 15:55:00   SteveP
* Added Stack parameters.
* 
*    Rev 1.3   26 Aug 1991 10:37:00   SteveP
* Added cleardoc and leaveonepage fields to header.
* 
*    Rev 1.2   22 Aug 1991 15:40:18   SteveP
* Added warmstart bit to header.
* 
*    Rev 1.1   13 Aug 1991 19:21:10   JimK
* Added stack size, priority and dirty bits
* 
*    Rev 1.0   25 Jun 1991 10:39:04   JimK
* Initial revision.
*
*
*
****************************************************************************/

#define RESERVED       48

typedef struct download_header {

    CHAR    name[ 32 ];
    SHORT   header_size;
    ULONG   loader_version;
    ULONG   abs_fixer_version;      /* Changed by abs fixer program */
    ULONG   nof_fixups;             /* Changed by abs fixer program */
    ULONG   fixup_start;            /* Changed by abs fixer program */
    ULONG   *prog_start;
    ULONG   crc;                    /* Changed by abs fixer program */
    SHORT   stack_size;
    SHORT   priority;
    UCHAR   dirty_download ;
    UCHAR   warmstart ;
    SHORT   start_stack_size ;
    CHAR    reserved[ RESERVED ];

} *DOWNLOAD_HEADER;

/****************************************************************************
*
*		$Workfile:   bitdefs.h  $	- bitmap definitions for the display system
*
*		$Revision:   1.2  $			$Modtime:   11 Apr 1991 16:40:36  $
*
*		$Author:   ajw  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/bitdefs.h_v  $
* 
*    Rev 1.2   11 Apr 1991 16:40:44   ajw
* Deleted nested include - limits.h - and put in #define CHARBIT
* 
*    Rev 1.1   08 Apr 1991 11:15:30   rwh
* Once again, deleted definition of SCROLLFN!!!!!!!
* 
*    Rev 1.0   25 Mar 1991 17:10:54   ajw
* Initial revision.
* 
*    Rev 1.5   25 Mar 1991 16:26:16   rwh
* Eliminated definition of SCROLLFN.
* 
*    Rev 1.4   19 Mar 1991 14:15:54   JeffS
* Needed to add the additional argument to the typedef for SCROLLFN.
* 
*    Rev 1.3   09 Mar 1991 09:19:14   rwh
* removed embedded comment.
* 
*    Rev 1.2   07 Mar 1991 13:41:42   rwh
* Undo last change.
* 
*    Rev 1.1   07 Mar 1991 13:30:06   rwh
* Changed parameter list in typedef of SCROLLFN
* 
*    Rev 1.0   06 Mar 1991 11:12:20   ajw
* Initial revision.
* 
*    Rev 1.0   08 Feb 1991 12:03:30   rwh
* Initial revision.
*
*
*
****************************************************************************/


#ifndef BITDEFSH
#define BITDEFSH

/* These definitions are artificial */

/* this is normally taken from the file <limits.h> */
#define CHAR_BIT 8

#define  L2R
/* #undef	L2R */
#define	BITSPERROW		480
#define	NUMBEROFROWS	128
#define  PITCH BITSPERROW
#define  BITSPERUNIT 8
#define  BITSIZE  (sizeof(BITUNIT))
#define  DATASIZE (sizeof(DATAUNIT))
#define  WHOLE (sizeof(BITUNIT) % DATASIZE))
#define	WORDSIZE		(sizeof(BITUNIT)*BITSPERUNIT)
#define	WORDSPERROW	  (BITSPERROW/WORDSIZE)
#define  DISPMEMLEN	(PITCH/WORDSIZE)

/* definitions for use by scroll routines that treat memory as USHORT rather
	than BITUNIT */
#define  USSIZE		(sizeof(USHORT)*CHAR_BIT)
#define  USMEMLEN		(PITCH/USSIZE)

#define  LEFTMASK	0
#define  RIGHTMASK -1

typedef BITUNIT DISPMEM[NUMBEROFROWS][DISPMEMLEN];
typedef USHORT  USDISPM[][USMEMLEN];

#if 0
typedef VOID (*SCROLLFN)( USDISPM *, SHORT, SHORT, SHORT, SHORT, SHORT, USHORT *, SHORT );
#else
typedef VOID (*SCROLLFN)();
#endif

/* Scrolling directives */

#define SCRL_RIGHT  0
#define SCRL_LEFT   1
#define SCRL_UP     2
#define SCRL_DOWN   3

#endif



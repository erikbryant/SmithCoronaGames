/****************************************************************************
*
*		$Workfile:   macros.h  $	- System macros
*
*		$Revision:   1.8  $			$Modtime:   29 Jul 1991 22:14:56  $
*
*		$Author:   WayneM  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/macros.h_v  $
* 
*    Rev 1.8   29 Jul 1991 22:15:40   WayneM
* Added YN_BOX
* 
*    Rev 1.7   29 May 1991 11:10:54   WayneM
* Added inches/cm to pixel conversion macros.
* 
*    Rev 1.6   20 May 1991 11:54:46   WayneM
* Removed inch/cm conversion macros
* 
*    Rev 1.5   17 May 1991 11:18:12   WayneM
* Added conversion rtns. from cent. & inches to pixels.
* 
*    Rev 1.4   26 Apr 1991 16:33:48   JimK
* Added MAX() and MIN().
* 
*
*
*
****************************************************************************/


#ifndef MACROSH
#define MACROSH

#define WinSetBlock(pr,pbk) (pr->pblk = pbk)

/* Debugging macro */

#define DBG(X) debug(&taskb,X)
#define ENTR DBG(entering)
#define WAIT DBG(waiting)
#define GOTM DBG(gotmsg)

/* Macros used to call the functions defined in psos.c                       */

#define SendStatus(target,data,buffer) sendmsg(target,uiMsgStatus,data,buffer)
#define SendCmd(target,what,data,buffer) sendmsg(target,uiMsgCmd,what,data,buffer)
#define MyInput(x) t_getreg(0,0,&x)
#define WinInput(x) t_getreg(0,WINQ,&x)

/* Macros used to follow dlink chains */

#define DlinkNext(pdl) ((pdl)->dl_n)
#define DlinkPrev(pdl) ((pdl)->dl_p)

/* Macros used to follow pages chains */

#define PageGetFirst(pdp,ptr_newpg) (*ptr_newpg = pageGetPageByNumber(pdp,1))
#define PageGetLast(pdp,ptr_newpg) (*ptr_newpg = pageGetPageByNumber(pdp,pageTotalPages(pdp)))

/* Macro used to get the queue address of a task input queue */
/* tasks is a global array of structures containing task information. This */
/* macro must never fail, so there is no error checking.                   */
#define TaskInQueue(X) ((ULONG) tasks[X].qid)

/* macros to find the MAXimum and MINimum of any type */
#define	MAX(a,b)				(((a)>(b))?(a):(b))
#define	MIN(a,b)				(((a)<=(b))?(a):(b))

#define  HORZPIXELSPERCM 1181				/* 118.11 pixels per centimeter (horz.) */
#define  HORZPIXELSPERIN 3000				/* 300 pixels per inch (horz.) */

#define  VERTPIXELSPERCM 283				/* 28.346 pixels per centimeter (vert.) */
#define  VERTPIXELSPERIN 720				/* 72 pixels per inch (vert.) */

#define editCM2HorzPixels( cm, decPos) editUnits2Pixels( cm, decPos, HORZPIXELSPERCM )
#define editCM2VertPixels( cm, decPos) editUnits2Pixels( cm, decPos, VERTPIXELSPERCM )
#define editIN2VertPixels( in, decPos) editUnits2Pixels( in, decPos, VERTPIXELSPERIN )
#define editIN2HorzPixels( in, decPos) editUnits2Pixels( in, decPos, HORZPIXELSPERIN )

#define editHorzPixels2CM( cm, decPos) editPixels2Units( cm, decPos, HORZPIXELSPERCM )
#define editVertPixels2CM( cm, decPos) editPixels2Units( cm, decPos, VERTPIXELSPERCM )
#define editVertPixels2In( in, decPos) editPixels2Units( in, decPos, VERTPIXELSPERIN )
#define editHorzPixels2In( in, decPos) editPixels2Units( in, decPos, HORZPIXELSPERIN )

enum { YN_BOX, YN_ATTN_BOX, ATTN_BOX };

#define YNDBox( l1,l2,l3,classCmd )		YNBox( l1,l2,l3,classCmd,YN_BOX,0 )
#define YNattn( l1,l2,l3,classCmd )		YNBox( l1,l2,l3,classCmd,YN_ATTN_BOX,0 )
#define WaitAttn( l1,l2,l3,l4,timer )	YNBox( l1,l2,l3,l4,ATTN_BOX,timer )

#endif

/****************************************************************************
*
*		$Workfile:   winmacro.h  $	- Macros for window functions
*
*		$Revision:   1.22  $			$Modtime:   21 Oct 1991 15:31:18  $
*
*		$Author:   rwh  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/winmacro.h_v  $
* 
*    Rev 1.22   21 Oct 1991 16:07:44   rwh
* Changed winWGetLeftShift to a transparent function call
* 
*    Rev 1.21   27 Sep 1991 03:11:38   AJW
* 
*    Rev 1.20   16 Sep 1991 13:57:02   JoeM
* VARSTAB ifndef
* 
*    Rev 1.19   24 Aug 1991 10:35:28   rwh
* Added winWClearNoSave macro for Joe M.
* Removed jsys macros.
* 
* 
*    Rev 1.18   19 Aug 1991 14:43:54   JimK
* 
*    Rev 1.17   19 Aug 1991 14:16:16   JimK
* Changed winWCreate macro for no root window for download
* 
*    Rev 1.16   17 Aug 1991 13:19:36   SteveP
* Changed jump table references.
* 
*    Rev 1.15   23 Jul 1991 14:02:02   rwh
* Numerous changes for new cursor task, speedups, error fixes, etc.
* 
* 
*    Rev 1.14   24 Jun 1991 10:21:36   ajw
* Added window extension software.
* 
*    Rev 1.13   15 May 1991 09:41:26   ajw
* Converted calls to winPutChar to calls to winWPutChar;
* New functions to display characters/strings in bold/underln.
* 
*    Rev 1.12   25 Apr 1991 14:05:32   rwh
* changed parameter to winWDrawBitmap{Color}
* 
*    Rev 1.11   25 Apr 1991 13:12:42   rwh
* Changed calls to bullet, chkbox, icon, fnctbox.
* 
*    Rev 1.10   25 Apr 1991 13:03:40   ajw
* Just changed comments and stuff...
* 
*    Rev 1.9   25 Apr 1991 09:55:52   rwh
* changed calling conventions for some functions and removed others.
* 
*    Rev 1.8   19 Apr 1991 11:42:12   ajw
* Changed description of winWClear for placement of text focus.
* 
*    Rev 1.7   19 Apr 1991 11:24:56   ajw
* Added functions winWPClear(), winRPClear() to clear portions of win/rect
* to bgc.
* 
*    Rev 1.6   18 Apr 1991 09:00:22   ajw
* Deleted winWGetMax{Above,Below,Lead}() macros.
* 
*    Rev 1.5   19 Apr 1991 08:40:56   ajw
* Deleted duplicate XOffset().
* 
*    Rev 1.4   17 Apr 1991 13:56:10   ajw
* New macro winCCompl to return the complementary color of the one provided.
* 
*    Rev 1.3   11 Apr 1991 19:32:22   ajw
* New routines to handle scrolling a color other than bg in.
* New routine to measure the length of the first n chars of a string.
* 
*    Rev 1.2   04 Apr 1991 11:06:02   ajw
* Removed nested includes - bitdefs & windows, changed descriptions to require
* windows to be included as well as winmacro and deleted winEditwinCreate - put
* in edisplay.c instead.
* 
*    Rev 1.1   27 Mar 1991 15:43:30   ajw
* Fixed bug in winWSetBase, added new routine winWDecTxtFocus
* & added #include bitdefs.h
* 
*    Rev 1.0   25 Mar 1991 10:32:24   ajw
* Initial revision.
*
*
*
****************************************************************************/
#ifndef WINMACH
#define WINMACH

#ifndef VARSTAB
extern struct window rootwindow;
#endif

#define	SetInRange(X,LOW,HIGH)				((((X) < (LOW)) ? (LOW) : (X)) > (HIGH) ? (HIGH) : (X))
#define	ChkOnBaseline(X,P_BASE)				(((X) < (P_BASE)->tx0) ? FALSE : (((X) > (P_BASE)->tx0 + (P_BASE)->twid) ? FALSE : TRUE))

/*--------------------------------------------------------------------------*/
/* INITIALIZATION ROUTINES																	 */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWCreate

	Descript:	Create a window.

	Prototype:	WINDOW winWCreate (SHORT x, SHORT y, SHORT dx, SHORT dy,
					                 SYSCOLOR bgc, SYSCOLOR fgc, 
										  SYSCOLOR bdc, SHORT bdw, SYSWINTYPE type );

	Returns:		A handle for the window. 

	Args:			x    - The upper left horizontal coordinate in pixels.
					y    - The upper left vertical coordinate in pixels.
					dx   - The window width.
					dy   - The window height.
					bgc  - Background pattern of the window.
					fgc  - Foreground color. 
					bdc  - Border color.
					bdw  - Border width. 0 => no border.
					type - Type of window eg. character text.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Window's parent will be the root window.
---------------------------------------------------------------------------*/
#define	winWCreate(X,Y,DX,DY,BGC,FGC,BDC,BDW,TYPE)	winChildCreate(&rootwindow,X,Y,DX,DY,BGC,FGC,BDC,BDW,TYPE,TRUE)

/*---------------------------------------------------------------------------
	Function:	winWCreateNoSave

	Descript:	Create a window. Don't save the background.

	Prototype:	WINDOW winWCreateNoSave(SHORT x, SHORT y, SHORT dx, SHORT dy,
					                 SYSCOLOR bgc, SYSCOLOR fgc, 
										  SYSCOLOR bdc, SHORT bdw, SYSWINTYPE type );

	Returns:		A handle for the window. 

	Args:			x    - The upper left horizontal coordinate in pixels.
					y    - The upper left vertical coordinate in pixels.
					dx   - The window width.
					dy   - The window height.
					bgc  - Background pattern of the window.
					fgc  - Foreground color. 
					bdc  - Border color.
					bdw  - Border width. 0 => no border.
					type - Type of window eg. character text.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Window's parent will be the root window.
---------------------------------------------------------------------------*/
#define	winWCreateNoSave(X,Y,DX,DY,BGC,FGC,BDC,BDW,TYPE)	winChildCreate(&rootwindow,X,Y,DX,DY,BGC,FGC,BDC,BDW,TYPE,FALSE)

/*---------------------------------------------------------------------------
	Function:	winWChildCreate

	Descript:	Creates a window relative to a parent window.

	Prototype:	WINDOW winWChildCreate (WINDOW parent, SHORT x, SHORT y, SHORT dx, SHORT dy,
					SYSCOLOR bgc, SYSCOLOR fgc, SYSCOLOR bdc,
					SHORT bdw, SYSWINTYPE type,  BOOL saveback)

	Returns:		A handle for the window. 

	Args:			parent   - The window relative to which this window is created.
					x        - The upper left horizontal coordinate in pixels.
					y        - The upper left vertical coordinate in pixels.
					dx       - The window width.
					dy       - The window height.
					bgc      - Background color of the window.
					fgc      - Foreground color. 
					bdc      - Border color.
					bdw      - Border width. 0 => no border.
					type     - Type of window eg. character text.
					saveback - If TRUE, then save the display region being covered 
								  by the window.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWChildCreate(P,X,Y,DX,DY,BG,FG,BD,BDW,TYPE,SAV)		winChildCreate(P,X,Y,DX,DY,BG,FG,BD,BDW,TYPE,SAV)

/*---------------------------------------------------------------------------
	Function:	winRAssign

	Descript:	Assigns values to a previously-created rectangle structure.

	Prototype:	VOID winRAssign (WINDOW win, RECT rect, SHORT x, SHORT y, SHORT dx, SHORT dy,
					SYSCOLOR bgc, SYSCOLOR fgc, SYSCOLOR bdc, SHORT bdw)

	Returns:		none

	Args:			win  - the parent window of the rectangle to be assigned
					rect - the strucure to which the values will be assigned
					x    - x-origin of new rectangle (relative to window)
					y    - y-origin
					dx   - rectangle width
					dy   - rectangle height
					bgc  - background color of rectangle
					fgc  - foreground color
					bdc  - border color
					bdw  - border width. 0 => no border.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRAssign(W,R,X,Y,DX,DY,BGC,FGC,BDC,BDW)		winRectAssign(W,R,X,Y,DX,DY,BGC,FGC,BDC,BDW)

/*---------------------------------------------------------------------------
	Function:	winWDestroy

	Descript:	Destroys a specified window and all of its children. The
 					background is restored if required.
	            
	Prototype:	VOID winWDestroy (WINDOW *win);

	Returns:		none.

	Args:			window - Address of the handle of the window to be destroyed.
								After winDestroy returns, the handle is left NULL and
								therefore invalid.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWDestroy(P_W)						winDestroy(P_W)

/*---------------------------------------------------------------------------
	Function:	winWClear

	Descript:	Clears a window to its background color. The text focus is
					placed at the origin of the default baseline.

	Prototype:	VOID winWClear (WINDOW win);

	Returns:		none

	Args:			win - Window to clear.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWClear(W)		(winWFill(W, (W)->rect.bgc), winWSetBaseDefault(W))

/*---------------------------------------------------------------------------
	Function:	winRClear

	Descript:	Clears a rectangle to its background color. 

	Prototype:	VOID winRClear (RECT rect);

	Returns:		none.

	Args:			rect - Rectangle to clear.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRClear(R)	winRFill(R, (R)->bgc)

/*---------------------------------------------------------------------------
	Function:	winWPClear

	Descript:	Clears some portion of a window to its background color. 

	Prototype:	VOID winWPClear (WINDOW win, SHORT x, SHORT y, SHORT dx, SHORT dy);

	Returns:		none

	Args:			win	- window containing portion to clear.
					x		- x-offset of portion to clear.
					y		- y-offset
					dx		- width
					dy		- height

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 04/19/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWPClear(W,X,Y,DX,DY)		winWPFill(W, X, Y, DX, DY, (W)->rect.bgc)

/*---------------------------------------------------------------------------
	Function:	winRPClear

	Descript:	Clears some portion of a rectangle to its background color. 

	Prototype:	VOID winRPClear (RECT rect, SHORT x, SHORT y, SHORT dx, SHORT dy);

	Returns:		none.

	Args:			rect	- Rectangle containing portion to clear.
					x		- x-offset of portion to clear.
					y		- y-offset
					dx		- width
					dy		- height

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 04/19/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRPClear(R,X,Y,DX,DY)		winRPFill(R, X, Y, DX, DY, (R)->bgc)

/*--------------------------------------------------------------------------*/
/* FILLING ROUTINES   																		 */
/*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
	Function:	winWGetWinWidth

	Descript:	Get width of the specified window.

	Prototype:	SHORT	winWGetWinWidth( WINDOW win );

	Returns:		none

	Args:			win   - window  in question.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetWinWidth(W)		((W)->rect.width)

/*---------------------------------------------------------------------------
	Function:	winWFill

	Descript:	Fill window with specified color.  Previous contents are lost.
					If Nocolor is the background color, nothing is done.

	Prototype:	VOID	winWFill( WINDOW win, SYSCOLOR color );

	Returns:		none

	Args:			win   - window to fill
					color - color to fill with.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWFill(W,COL)		winRectOp(W, 0, 0, winWGetWinWidth(W), (W)->rect.dy,Fill,COL)

/*---------------------------------------------------------------------------
	Function:	winRFill

	Descript:	Fill rectangle with specified color.  Previous contents are 
					lost. If Nocolor is the background color, nothing is done.

	Prototype:	VOID	winRFill( RECT rect, SYSCOLOR color );

	Returns:		none

	Args:			rect  - rectangle to fill
					color - color to fill with.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/

#if 0
#define	winRFill(R,COL)		winRectOp(R, 0, 0, (R)->dx,(R)->dy,Fill,COL)
#endif

/*---------------------------------------------------------------------------
	Function:	winWPFill

	Descript:	Fill portion of window with specified color.  Previous contents 
					are lost. If Nocolor is the background color, nothing is done.

	Prototype:	VOID	winWPFill( WINDOW win, SHORT x, SHORT y, SHORT dx, SHORT dy, SYSCOLOR color );

	Returns:		none

	Args:			win   - window containing portion to fill
					x     - x-origin of portion to fill (relative to window)
					y     - y-origin
					dx    - width of portion to fill
					dy    - height
					color - color to fill with.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWPFill(W,X,Y,DX,DY,COL)		winRectOp(W, X, Y,DX,DY,Fill,COL)

/*---------------------------------------------------------------------------
	Function:	winRPFill

	Descript:	Fill portion of rectangle with specified color.  Previous 
					contents are lost. If Nocolor is the background color, nothing 
					is done.

	Prototype:	VOID winRPFill( RECT rect, SHORT x, SHORT y, SHORT dx, SHORT dy, SYSCOLOR color );

	Returns:		none

	Args:			rect  - rectangle containing portion to fill
					x     - x-origin of portion to fill (relative to rectangle)
					y     - y-origin
					dx    - width of portion to fill
					dy    - height
					color - color to fill with.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRPFill(R,X,Y,DX,DY,COL)		winRectOp(R, X, Y,DX,DY,Fill,COL)
#endif
/*--------------------------------------------------------------------------*/
/* COMPLEMENTING ROUTINES																	 */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWCompl

	Descript:	Complement the contents of a window

	Prototype:	VOID winWCompl (WINDOW win)

	Returns:		none

	Args:			win - window to complement

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWCompl(W)	winRectOp(W, 0, 0, winWGetWinWidth(W),(W)->rect.dy,Compl,Nocolor)

/*---------------------------------------------------------------------------
	Function:	winRCompl

	Descript:	Complement the contents of a rectangle

	Prototype:	VOID winRCompl (RECT rect)

	Returns:		none

	Args:			rect - rectangle to complement

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRCompl(R)	winRectOp(R, 0, 0, (R)->dx,(R)->dy,Compl,Nocolor)
#endif
/*---------------------------------------------------------------------------
	Function:	winWPCompl

	Descript:	Complement the contents of some portion of a window

	Prototype:	VOID winWPCompl (WINDOW win, SHORT x, SHORT y, SHORT dx, SHORT dy)

	Returns:		none

	Args:			win - window containing portion to complement
					x   - x-origin of portion to complement (relative to window)
					y   - y-origin
					dx  - width of portion to complement
					dy  - height

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWPCompl(W,X,Y,DX,DY)		winRectOp(W, X, Y,DX,DY,Compl,Nocolor)

/*---------------------------------------------------------------------------
	Function:	winRPCompl

	Descript:	Complement the contents of some portion of a rectangle

	Prototype:	VOID winRPCompl (RECT rect, SHORT x, SHORT y, SHORT dx, SHORT dy)

	Returns:		none

	Args:			rect - rectangle containing portion to complement
					x    - x-origin of portion to complement (relative to rectangle)
					y    - y-origin
					dx   - width of portion to complement
					dy   - height

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRPCompl(R,X,Y,DX,DY)		winRectOp(R, X, Y,DX,DY,Compl,Nocolor)
#endif

/*---------------------------------------------------------------------------
	Function:	winCCompl

	Descript:	Returns the complement of the color indicated.

	Prototype:	SYSCOLOR winCCompl (SYSCOLOR color)

	Returns:		Complement of color.

	Args:			color - color to complement

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 04/17/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winCCompl(COL)					winGetCompl (COL)

/*--------------------------------------------------------------------------*/
/* SCROLLING ROUTINES																	    */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* routines to scroll entire window 												    */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWScrollRight winWScrollLeft winWScrollUp winWScrollDown

	Descript:	Scroll a window by a specified amount in the direction 
					indicated.

	Prototype:	VOID winWScrollRight (WINDOW win, SHORT number);
               VOID winWScrollLeft  (WINDOW win, SHORT number);
               VOID winWScrollUp    (WINDOW win, SHORT number);
               VOID winWScrollDown  (WINDOW win, SHORT number);

	Returns:		none

	Args:			win    - window to scroll
					number - Number of pixels/scanlines to scroll.			

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWScrollRight(W,AMT)				winScroll(W,0,0,winWGetWinWidth(W),(W)->rect.dy,SCRL_UP,AMT,(W)->rect.bgc) 
#define	winWScrollLeft(W,AMT)				winScroll(W,0,0,winWGetWinWidth(W),(W)->rect.dy,SCRL_LEFT,AMT,(W)->rect.bgc)
#define	winWScrollUp(W,AMT)					winScroll(W,0,0,winWGetWinWidth(W),(W)->rect.dy,SCRL_UP,AMT,(W)->rect.bgc)
#define	winWScrollDown(W,AMT)				winScroll(W,0,0,winWGetWinWidth(W),(W)->rect.dy,SCRL_DOWN,AMT,(W)->rect.bgc)

/*---------------------------------------------------------------------------
	Function:	winWScrollColorRight winWScrollColorLeft winWScrollColorUp 
					winWScrollColorDown

	Descript:	Scroll a window by a specified amount in the direction 
					indicated and fill with the specified color.

	Prototype:	VOID winWScrollColorRight (WINDOW win, SHORT number, SYSCOLOR color);
               VOID winWScrollColorLeft  (WINDOW win, SHORT number, SYSCOLOR color);
               VOID winWScrollColorUp    (WINDOW win, SHORT number, SYSCOLOR color);
               VOID winWScrollColorDown  (WINDOW win, SHORT number, SYSCOLOR color);

	Returns:		none

	Args:			win    - window to scroll
					number - Number of pixels/scanlines to scroll.			
					color  - Color to fill scrolled region with.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 04/11/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWScrollColorRight(W,AMT,COL)				winScroll(W,0,0,winWGetWinWidth(W),(W)->rect.dy,SCRL_UP,AMT,COL) 
#define	winWScrollColorLeft(W,AMT,COL)				winScroll(W,0,0,winWGetWinWidth(W),(W)->rect.dy,SCRL_LEFT,AMT,COL)
#define	winWScrollColorUp(W,AMT,COL)					winScroll(W,0,0,winWGetWinWidth(W),(W)->rect.dy,SCRL_UP,AMT,COL)
#define	winWScrollColorDown(W,AMT,COL)				winScroll(W,0,0,winWGetWinWidth(W),(W)->rect.dy,SCRL_DOWN,AMT,COL)

/*--------------------------------------------------------------------------*/
/* routines to scroll entire rectangle												    */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWRScrollRight winWRScrollLeft winWRScrollUp winWRScrollDown

	Descript:	Scroll a rectangle by a specified amount in the direction 
					indicated.

	Prototype:	VOID winWRScrollRight (WINDOW win, RECT rect, SHORT number);
               VOID winWRScrollLeft  (WINDOW win, RECT rect, SHORT number);
               VOID winWRScrollUp    (WINDOW win, RECT rect, SHORT number);
               VOID winWRScrollDown  (WINDOW win, RECT rect, SHORT number);

	Returns:		none

	Args:			win	 - window containing rectangle
					rect   - rectangle to scroll
					number - Number of pixels/scanlines to scroll.			

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winWRScrollRight(W,R,AMT)				winScroll(W,winXOffset(W,R),winYOffset(W,R),(R)->dx,(R)->dy,SCRL_RIGHT,AMT,(R)->bgc)
#define	winWRScrollLeft(W,R,AMT)				winScroll(W,winXOffset(W,R),winYOffset(W,R),(R)->dx,(R)->dy,SCRL_LEFT,AMT,(R)->bgc)
#define	winWRScrollUp(W,R,AMT)					winScroll(W,winXOffset(W,R),winYOffset(W,R),(R)->dx,(R)->dy,SCRL_UP,AMT,(R)->bgc)
#define	winWRScrollDown(W,R,AMT)				winScroll(W,winXOffset(W,R),winYOffset(W,R),(R)->dx,(R)->dy,SCRL_DOWN,AMT,(R)->bgc)
#endif
/*--------------------------------------------------------------------------*/
/* routines to scroll some portion of window                                */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWPScrollRight winWPScrollLeft winWPScrollUp winWPScrollDown

	Descript:	Scroll a portion of a window by a specified amount in the 
					direction indicated.

	Prototype:	VOID winWPScrollRight (WINDOW win, SHORT x, SHORT y, SHORT dx,
										 SHORT dy, SHORT number);
               VOID winWPScrollLeft  (WINDOW win, SHORT x, SHORT y, SHORT dx,
										 SHORT dy, SHORT number);
               VOID winWPScrollUp    (WINDOW win, SHORT x, SHORT y, SHORT dx,
										 SHORT dy, SHORT number);
               VOID winWPScrollDown  (WINDOW win, SHORT x, SHORT y, SHORT dx,
										 SHORT dy, SHORT number);

	Returns:		none

	Args:			win    - window containing portion to scroll
					x      - x-origin of portion to scroll (relative to window)
					y      - y-origin
					dx     - width of portion to scroll
					dy     - height
					number - Number of pixels/scanlines to scroll.			

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWPScrollRight(W,X,Y,DX,DY,N)	winScroll(W,X,Y,DX,DY,SCRL_RIGHT,N,(W)->rect.bgc)
#define	winWPScrollLeft(W,X,Y,DX,DY,N)	winScroll(W,X,Y,DX,DY,SCRL_LEFT,N,(W)->rect.bgc)
#define	winWPScrollUp(W,X,Y,DX,DY,N)		winScroll(W,X,Y,DX,DY,SCRL_UP,N,(W)->rect.bgc)
#define	winWPScrollDown(W,X,Y,DX,DY,N)	winScroll(W,X,Y,DX,DY,SCRL_DOWN,N,(W)->rect.bgc)

/*---------------------------------------------------------------------------
	Function:	winWPScrollColorRight winWPScrollColorLeft winWPScrollColorUp winWPScrollColorDown

	Descript:	Scroll a portion of a window by a specified amount in the 
					direction indicated and fill with the specified color.

	Prototype:	VOID winWPScrollColorRight (WINDOW win, SHORT x, SHORT y, SHORT dx,
										 SHORT dy, SHORT number, SYSCOLOR color);
               VOID winWPScrollColorLeft  (WINDOW win, SHORT x, SHORT y, SHORT dx,
										 SHORT dy, SHORT number, SYSCOLOR color);
               VOID winWPScrollColorUp    (WINDOW win, SHORT x, SHORT y, SHORT dx,
										 SHORT dy, SHORT number, SYSCOLOR color);
               VOID winWPScrollColorDown  (WINDOW win, SHORT x, SHORT y, SHORT dx,
										 SHORT dy, SHORT number, SYSCOLOR color);

	Returns:		none

	Args:			win    - window containing portion to scroll
					x      - x-origin of portion to scroll (relative to window)
					y      - y-origin
					dx     - width of portion to scroll
					dy     - height
					number - Number of pixels/scanlines to scroll
					color	 - Color to fill scrolled region with

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 04/15/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWPScrollColorRight(W,X,Y,DX,DY,N,COL)	winScroll(W,X,Y,DX,DY,SCRL_RIGHT,N,COL)
#define	winWPScrollColorLeft(W,X,Y,DX,DY,N,COL)	winScroll(W,X,Y,DX,DY,SCRL_LEFT,N,COL)
#define	winWPScrollColorUp(W,X,Y,DX,DY,N,COL)		winScroll(W,X,Y,DX,DY,SCRL_UP,N,COL)
#define	winWPScrollColorDown(W,X,Y,DX,DY,N,COL)	winScroll(W,X,Y,DX,DY,SCRL_DOWN,N,COL)

/*--------------------------------------------------------------------------*/
/* routines to scroll some portion of rectangle                             */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWRPScrollRight winWRPScrollLeft winWRPScrollUp winWRPScrollDown

	Descript:	Scroll a portion of a rectangle by a specified amount in the 
					direction indicated.

	Prototype:	VOID winWRPScrollRight (WINDOW win, RECT rect, SHORT x, SHORT y, 
							  		SHORT dx, SHORT dy, SHORT direction, SHORT number);
               VOID winWRPScrollLeft  (WINDOW win, RECT rect, SHORT x, SHORT y, SHORT dx,
							  		SHORT dx, SHORT dy, SHORT direction, SHORT number);
               VOID winWRPScrollUp    (WINDOW win, RECT rect, SHORT x, SHORT y, SHORT dx,
							  		SHORT dx, SHORT dy, SHORT direction, SHORT number);
               VOID winWRPScrollDown  (WINDOW win, RECT rect, SHORT x, SHORT y, SHORT dx,
							  		SHORT dx, SHORT dy, SHORT direction, SHORT number);

	Returns:		none

	Args:			win    - window containing rectangle
					rect   - rectangle containing portion to scroll
      			x      - x-origin of portion to scroll (relative to rectangle)
					y      - y-origin
					dx     - width of portion to scroll
					dy     - height
					number - Number of pixels/scanlines to scroll.			

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWRPScrollRight(W,R,X,Y,DX,DY,N)	winScroll(W,winXOffset(W,R)+(X),winYOffset(W,R)+(Y),DX,DY,SCRL_RIGHT,N,(R)->bgc)
#define	winWRPScrollLeft(W,R,X,Y,DX,DY,N)	winScroll(W,winXOffset(W,R)+(X),winYOffset(W,R)+(Y),DX,DY,SCRL_LEFT,N,(R)->bgc)
#define	winWRPScrollUp(W,R,X,Y,DX,DY,N)		winScroll(W,winXOffset(W,R)+(X),winYOffset(W,R)+(Y),DX,DY,SCRL_UP,N,(R)->bgc)
#define	winWRPScrollDown(W,R,X,Y,DX,DY,N)	winScroll(W,winXOffset(W,R)+(X),winYOffset(W,R)+(Y),DX,DY,SCRL_DOWN,N,(R)->bgc)

/*--------------------------------------------------------------------------*/
/* BITMAP-DRAWING ROUTINES                                                  */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWDrawBitmap

	Descript:	Draw a bitmap at the specified location within a window.

	Prototype:	void winWDrawBitmap( WINDOW win, SHORT x, SHORT y, BITMAP bitmap );

	Returns:		none

	Args:			win    - window to contain bitmap
					x      - x-origin of bitmap (relative to window)
					y      - y-origin
					bitmap - pointer to structure containing pixel-map to draw.  
								
	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		A NULL bitmap pointer is a no-op.
---------------------------------------------------------------------------*/
#define	winWDrawBitmap(W,X,Y,BITMAP)		winDrawBitmap (W, X, Y, BITMAP, (W)->rect.bgc, (W)->rect.fgc) 

/*---------------------------------------------------------------------------
	Function:	winRDrawBitmap

	Descript:	Draw a bitmap at the specified location within a rectangle.

	Prototype:	void winRDrawBitmap(RECT rect, SHORT x, SHORT y, BITMAP bitmap);

	Returns:		none

	Args:			rect   - rectangle to contain bitmap
					x      - x-origin of bitmap (relative to rectangle)
					y      - y-origin
					bitmap - pointer to structure containing pixel-map to draw.  
								
	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		A NULL bitmap pointer is a no-op.
---------------------------------------------------------------------------*/
#if 0
#define	winRDrawBitmap(R,X,Y,BITMAP)		winDrawBitmap (R, X, Y, BITMAP, (R)->bgc, (R)->fgc) 
#endif

/*---------------------------------------------------------------------------
	Function:	winWDrawBitmapColor

	Descript:	Draw a bitmap at the specified location within a window using
					the colors indicated.

	Prototype:	void winWDrawBitmapColor (WINDOW win, SHORT x, SHORT y,
						  BITMAP bitmap, SYSCOLOR bgc, SYSCOLOR fgc);

	Returns:		none

	Args:			win    - window to contain bitmap
					x      - x-origin of bitmap (relative to window)
					y      - y-origin
					bitmap - pointer to structure containing pixel-map to draw.  
					bgc    - background color of bitmap
					fgc    - foreground color of bitmap
								
	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Results will only be meaningful if Black or White are the
					colors used.  Same color => fill	region with that color.
					A NULL bitmap pointer is a no-op.
---------------------------------------------------------------------------*/
#define	winWDrawBitmapColor(W,X,Y,MAP,BGC,FGC)	winDrawBitmap (W, X, Y, MAP, BGC, FGC)

/*---------------------------------------------------------------------------
	Function:	winRDrawBitmapColor

	Descript:	Draw a bitmap at the specified location within a rectangle 
					using	the colors indicated.

	Prototype:	void winRDrawBitmapColor (RECT rect, SHORT x, SHORT y,
						  BITMAP bitmap, SYSCOLOR bgc, SYSCOLOR fgc);

	Returns:		none

	Args:			rect   - rectangle to contain bitmap
					x      - x-origin of bitmap (relative to rectangle)
					y      - y-origin
					bitmap - pointer to structure containing pixel-map to draw.  
					bgc    - background color of bitmap
					fgc    - foreground color of bitmap
								
	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Results will only be meaningful if Black or White are the
					colors used.  Same color => fill	region with that color.
					A NULL bitmap pointer is a no-op.
---------------------------------------------------------------------------*/
#if 0
#define	winRDrawBitmapColor(R,X,Y,MAP,BGC,FGC)	winDrawBitmap (R, X, Y, MAP, BGC, FGC)
#endif

/*---------------------------------------------------------------------------
	Function:	winWBulletOn

	Descript:	Turn on bullet at specified position within a window.

	Prototype:	VOID winWBulletOn (WINDOW win, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			win  - window to contain bullet 
					x    - x-origin of bullet (relative to window)
					y    - y-origin
					size - height and width of bullet [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWBulletOn(W,X,Y,SIZE)			winBulletOn(W,X,Y,SIZE)

/*---------------------------------------------------------------------------
	Function:	winRBulletOn

	Descript:	Turn on bullet at specified position within a rectangle.

	Prototype:	VOID winRBulletOn (RECT rect, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			rect - rectangle to contain bullet 
					x    - x-origin of bullet (relative to rectangle)
					y    - y-origin
					size - height and width of bullet [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRBulletOn(R,X,Y,SIZE)			winBulletOn(R,X,Y,SIZE)
#endif

/*---------------------------------------------------------------------------
	Function:	winWBulletOff

	Descript:	Turn off bullet at specified position within a window.

	Prototype:	VOID winWBulletOff (WINDOW win, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			win  - window containing bullet 
					x    - x-origin of bullet (relative to window)
					y    - y-origin
					size - height and width of bullet [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWBulletOff(W,X,Y,SIZE)			winBulletOff(W,X,Y,SIZE)

/*---------------------------------------------------------------------------
	Function:	winRBulletOff

	Descript:	Turn off bullet at specified position within a rectangle.

	Prototype:	VOID winRBulletOff (RECT rect, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			rect - rectangle containing bullet 
					x    - x-origin of bullet (relative to rectangle)
					y    - y-origin
					size - height and width of bullet [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRBulletOff(R,X,Y,SIZE)			winBulletOff(R,X,Y,SIZE)
#endif

/*---------------------------------------------------------------------------
	Function:	winWChkboxOn

	Descript:	Turn on checkbox at specified position within a window.

	Prototype:	VOID winWChkboxOn (WINDOW win, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			win  - window to contain checkbox 
					x    - x-origin of checkbox (relative to window)
					y    - y-origin
					size - height and width of checkbox [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWChkboxOn(W,X,Y,SIZE)			winChkboxOn(W,X,Y,SIZE)

/*---------------------------------------------------------------------------
	Function:	winRChkboxOn

	Descript:	Turn on checkbox at specified position within a rectangle.

	Prototype:	VOID winRChkboxOn (RECT rect, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			rect - rectangle to contain checkbox 
					x    - x-origin of checkbox (relative to rectangle)
					y    - y-origin
					size - height and width of checkbox [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRChkboxOn(R,X,Y,SIZE)			winChkboxOn(R,X,Y,SIZE)
#endif

/*---------------------------------------------------------------------------
	Function:	winWChkboxOff

	Descript:	Turn off checkbox at specified position within a window.

	Prototype:	VOID winWChkboxOff (WINDOW win, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			win  - window containing checkbox 
					x    - x-origin of checkbox (relative to window)
					y    - y-origin
					size - height and width of checkbox [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWChkboxOff(W,X,Y,SIZE)			winChkboxOff(W,X,Y,SIZE)

/*---------------------------------------------------------------------------
	Function:	winRChkboxOff

	Descript:	Turn off checkbox at specified position within a rectangle.

	Prototype:	VOID winRChkboxOff (RECT rect, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			rect - rectangle containing checkbox 
					x    - x-origin of checkbox (relative to rectangle)
					y    - y-origin
					size - height and width of checkbox [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRChkboxOff(R,X,Y,SIZE)			winChkboxOff(R,X,Y,SIZE)
#endif

/*---------------------------------------------------------------------------
	Function:	winWIconOn

	Descript:	Turn on icon at specified position within a window.

	Prototype:	VOID winWIconOn (WINDOW win, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			win  - window to contain icon 
					x    - x-origin of icon (relative to window)
					y    - y-origin
					size - height and width of icon [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWIconOn(W,X,Y,SIZE)				winIconOn(W,X,Y,SIZE)

/*---------------------------------------------------------------------------
	Function:	winRIconOn

	Descript:	Turn on icon at specified position within a rectangle.

	Prototype:	VOID winRIconOn (RECT rect, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			rect - rectangle to contain icon 
					x    - x-origin of icon (relative to rectangle)
					y    - y-origin
					size - height and width of icon [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRIconOn(R,X,Y,SIZE)				winIconOn(R,X,Y,SIZE)
#endif

/*---------------------------------------------------------------------------
	Function:	winWIconOff

	Descript:	Turn off icon at specified position within a window.

	Prototype:	VOID winWIconOff (WINDOW win, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			win  - window containing icon 
					x    - x-origin of icon (relative to window)
					y    - y-origin
					size - height and width of icon [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWIconOff(W,X,Y,SIZE)				winIconOff(W,X,Y,SIZE)

/*---------------------------------------------------------------------------
	Function:	winRIconOff

	Descript:	Turn off icon at specified position within a rectangle.

	Prototype:	VOID winRIconOff (RECT rect, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			rect - rectangle containing icon 
					x    - x-origin of icon (relative to rectangle)
					y    - y-origin
					size - height and width of icon [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRIconOff(R,X,Y,SIZE)				winIconOff(R,X,Y,SIZE)
#endif

/*---------------------------------------------------------------------------
	Function:	winWFnctboxOn

	Descript:	Turn on function-box at specified position within a window.

	Prototype:	VOID winWFnctboxOn (WINDOW win, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			win  - window to contain function-box 
					x    - x-origin of function-box (relative to window)
					y    - y-origin
					size - height and half-width of function-box [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWFnctboxOn(W,X,Y,SIZE)			winFnctboxOn(W,X,Y,SIZE)

/*---------------------------------------------------------------------------
	Function:	winRFnctboxOn

	Descript:	Turn on function-box at specified position within a rectangle.

	Prototype:	VOID winRFnctboxOn (RECT rect, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			rect - rectangle to contain function-box 
					x    - x-origin of function-box (relative to rectangle)
					y    - y-origin
					size - height and half-width of function-box [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRFnctboxOn(R,X,Y,SIZE)			winFnctboxOn(R,X,Y,SIZE)
#endif

/*---------------------------------------------------------------------------
	Function:	winWFnctboxOff

	Descript:	Turn off function-box at specified position within a window.

	Prototype:	VOID winWFnctboxOff (WINDOW win, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			win  - window containing function-box 
					x    - x-origin of function-box (relative to window)
					y    - y-origin
					size - height and half-width of function-box [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWFnctboxOff(W,X,Y,SIZE)			winFnctboxOff(W,X,Y,SIZE)

/*---------------------------------------------------------------------------
	Function:	winRFnctboxOff

	Descript:	Turn off function-box at specified position within a rectangle.

	Prototype:	VOID winRFnctboxOff (RECT rect, SHORT x, SHORT y, SHORT size)

	Returns:		none

	Args:			rect - rectangle containing function-box 
					x    - x-origin of function-box (relative to rectangle)
					y    - y-origin
					size - height and half-width of function-box [temporary]

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#if 0
#define	winRFnctboxOff(R,X,Y,SIZE)			winFnctboxOff(R,X,Y,SIZE)
#endif

/*---------------------------------------------------------------------------
	Function:	winWPDrawBox

	Descript:	Draws a border around the region indicated.  The border will
					lie within the dimensions indicated.

	Prototype:	VOID winWPDrawBox(WINDOW win, SHORT x, SHORT y, SHORT dx, 
							SHORT dy, SHORT width)

	Returns:		none

	Args:			win	-	window to draw box within
					x		-	x-offset from window to draw box
					y		-	y-offset from window
					dx		-	width of box
					dy		-	height of box
					width	-	width of border of box

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 04/09/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWPDrawBox(W,X,Y,DX,DY,WID)		(winWPFill(W,X,Y,DX,WID,(W)->rect.fgc),\
														 winWPFill(W,X,Y,WID,DY,(W)->rect.fgc),\
														 winWPFill(W,(X)+(DX)-(WID),Y,WID,DY,(W)->rect.fgc),\
														 winWPFill(W,X,(Y)+(DY)-(WID),DX,WID,(W)->rect.fgc))

/*---------------------------------------------------------------------------
	Function:	winXOffset

	Descript:	Returns the x-offset of the rectangle specified from the window.

	Prototype:	SHORT winXOffset (WINDOW win, RECT rect)

	Returns:		x-offset

	Args:			none

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winXOffset(W,R)						((R)->x - winWGetWinWidth(W))

/*---------------------------------------------------------------------------
	Function:	winYOffset

	Descript:	Returns the y-offset of the rectangle specified from the window.

	Prototype:	SHORT winYOffset (WINDOW win, RECT rect)

	Returns:		y-offset

	Args:			none

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winYOffset(W,R)						((R)->y - (W)->rect.y)

/*--------------------------------------------------------------------------*/
/* FONT/CHARACTER/STRING ROUTINES                                           */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWFocusToStart

	Descript:	Set text focus to start of window baseline.

	Prototype:	VOID winWFocusToStart (WINDOW win)

	Returns:		none

	Args:			win - window to contain text focus

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWFocusToStart(W)					((W)->tx = (W)->base.tx0)

/*---------------------------------------------------------------------------
	Function:	winSetOnBaseline

	Descript:	Checks whether the horizontal position indicated lies on the
					baseline and returns the value if so, otherwise returns the
					appropriate baseline endpoint.

	Prototype:	SHORT winSetOnBaseline (SHORT x, BASELINE baseline)

	Returns:		value nearest to x that fits on baseline 

	Args:			x        - horizontal position to place within baseline limits.
					baseline - baseline 

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winSetOnBaseline(X,P_BASE)			SetInRange(X, (P_BASE)->tx0, (P_BASE)->twid - (P_BASE)->tx0)

/*---------------------------------------------------------------------------
	Function:	winWClearBase

	Descript:	Sets all vertical spacings of the baseline to zero.  The 
					position of the baseline does not alter.

	Prototype:	VOID winWClearBase (WINDOW win)

	Returns:		none

	Args:			win - window to clear baseline of

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWClearBase(W)	((W)->base.tabove = 0, (W)->base.tbelow = 0, (W)->base.tlead = 0)

/*---------------------------------------------------------------------------
	Function:	winWSetBaseToFont

	Descript:	Checks whether the current window font can fit in the vertical
					space required by the current baseline, and if not, adjust 
					baseline appropriately.

	Prototype:	VOID winWSetBaseToFont (WINDOW win)

	Returns:		none

	Args:			win - window to match baseline to font

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		If the baseline lines-above-base is smaller than the value
					required by the current font, the baseline lines-above-base
					is increased by that amount but the baseline is not moved
					down.
---------------------------------------------------------------------------*/
#define	winWSetBaseToFont(W)	((W)->base.tabove = ((W)->font.above > (W)->base.tabove) ? (W)->font.above : (W)->base.tabove,\
										 (W)->base.tbelow = ((W)->font.below > (W)->base.tbelow) ? (W)->font.below : (W)->base.tbelow,\
										 (W)->base.tlead = ((W)->font.lead > (W)->base.tlead) ? (W)->font.lead : (W)->base.tlead)
										 /* what about a check to ensure that these guys still fit w/in window? */

/*---------------------------------------------------------------------------
	Function:	winWGetDefaultFont

	Descript:	Returns the ID of the default font for the type of window
					specified.

	Prototype:	SYSFONTID winWGetDefaultFont (WINDOW w)

	Returns:		font-id.

	Args:			w - window to check type of. 

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetDefaultFont(W)					fontGetDefault(Chartext)

/*---------------------------------------------------------------------------
	Function:	winWSetFont

	Descript:	Changes the current font in the window indicated.

	Prototype:	VOID winWSetFont (WINDOW win, SYSFONTID fontid);

	Returns:		none

	Args: 		win    - window in which to change font.
					fontid - font identifier.
								
	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWSetFont(W,FONTID)				winSetFont(W, FONTID)

/*---------------------------------------------------------------------------
	Function:	winWStringSize

	Descript:	Determine the width of a string of ASCII characters in pixels
	            for the current font in the window indicated.

	Prototype:	SHORT winWStringSize (WINDOW win, UCHAR *string)

	Returns:		Returns the length of the string in pixels. 

	Args:			win    - window to check string length for
					string - pointer to a string of ASCII characters.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Some fonts do not contain all displayable characters.  Any
					non-displayable characters in the string are skipped.
---------------------------------------------------------------------------*/
#define	winWStringSize(W,P_STR)				fontWidthScrnStr((W)->font.id, P_STR)

/*---------------------------------------------------------------------------
	Function:	winWStringNSize

	Descript:	Determine the width of the first n characters of a string of 
					ASCII characters.  The value returned will be in pixels for the 
					current font in the window indicated.

	Prototype:	SHORT winWStringNSize (WINDOW win, UCHAR *string, SHORT n)

	Returns:		Returns the length of the string in pixels. 

	Args:			win    - window to check string length for
					string - pointer to a string of ASCII characters.
					n      - only measure the first n chars in string 

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 04/10/91

	Notes:		Some fonts do not contain all displayable characters.  Any
					non-displayable characters in the string are skipped.
---------------------------------------------------------------------------*/
#define	winWStringNSize(W,P_STR,N)				winStringNSize(W,P_STR,N)

/*---------------------------------------------------------------------------
	Function:	winWSetBase

	Descript:	Set the baseline of a window to a specified starting location 
					(relative to window) and length.

	Prototype:	VOID winWSetBase (WINDOW win, SHORT x0, SHORT y, SHORT wid)

	Returns:		none

	Args:			win - window to contain baseline
					x0  - x-coord of start of baseline (relative to window)
					y   - y-coord of baseline
					wid - width of baseline

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		No test is performed to ensure that the baseline lies within
					the window.  Current text focus is set to start of baseline.
---------------------------------------------------------------------------*/
#define	winWSetBase(W,X0,Y,WID)				((W)->base.tx0	=	SetInRange(X0, 0, winWGetWinWidth(W)),\
														 (W)->base.ty		=	SetInRange(Y, (W)->font.above, (W)->rect.dy - (W)->font.below),\
														 (W)->base.twid	=	SetInRange((X0)+(WID), 0, winWGetWinWidth(W)) - (W)->base.tx0,\
														 winWFocusToStart(W))

/*---------------------------------------------------------------------------
	Function:	winWRSetBase

	Descript:	Set the baseline of a window to a specified starting location 
					(relative to rectangle) and length.

	Prototype:	VOID winWRSetBase (WINDOW win, RECT rect, SHORT x0, SHORT y, SHORT wid)

	Returns:		none

	Args:			win  - window to contain baseline
					rect - rectangle to be used as offset to baseline
					x0   - x-coord of start of baseline (relative to rectangle)
					y    - y-coord of baseline
					wid  - width of baseline

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		No test is performed to ensure that the baseline lies within
					the rectangle.  Current text focus is set to start of baseline.
---------------------------------------------------------------------------*/
#define	winWRSetBase(W,R,X0,Y,WID)			((W)->base.tx0	=	SetInRange(X0, 0, (R)->dx) + winXOffset(W,R),\
														 (W)->base.ty		=	SetInRange(Y, (W)->font.above, (R)->dy - (W)->font.below) + winYOffset(W,R),\
														 (W)->base.twid	=	SetInRange((X0)+(WID), 0, (R)->dx) - (W)->base.tx0 + winXOffset(W,R),\
														 winWFocusToStart(W))

/*---------------------------------------------------------------------------
	Function:	winWSetBaseDefault

	Descript:	Set the baseline of a window to the default.

	Prototype:	VOID winWSetBaseDefault (WINDOW win)

	Returns:		none

	Args:			win - window to contain baseline

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Default starting point is the window origin offset vertically
					by an amount dependent upon the current window font, and the
					default width is window width.  Current text focus is set to 
					start of	baseline.
---------------------------------------------------------------------------*/
/** ?? Need checks in here for small windows where font.above might be larger
than the window height ?? **/
#define	winWSetBaseDefault(W)				((W)->base.tx0	=	0,\
														 (W)->base.ty		=	(W)->font.above,\
														 (W)->base.twid	=	winWGetWinWidth(W),\
														 winWFocusToStart(W))

/*---------------------------------------------------------------------------
	Function:	winWRSetBaseDefault

	Descript:	Set the baseline of a window to the default for the rectangle 
					indicated.

	Prototype:	VOID winWRSetBaseDefault (WINDOW win, RECT rect)

	Returns:		none

	Args:			win  - window to contain baseline
					rect - rectangle to be used for baseline positioning

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Default starting point is the rectangle origin offset 
					vertically by an amount dependent upon the current window font, 
					and the default width is rectangle width.  Current text focus 
					is set to start of baseline.
---------------------------------------------------------------------------*/
#define	winWRSetBaseDefault(W,R)			((W)->base.tx0	=	winXOffset(W,R),\
														 (W)->base.ty		=	(W)->font.above + winYOffset(W,R),\
														 (W)->base.twid	=	(R)->dx,\
														 winWFocusToStart(W))

/*---------------------------------------------------------------------------
	Function:	winWSetBaseYSpecial

	Descript:	Set the baseline of a window to the default x-origin and
					length, but specify the vertical position (relative to window).

	Prototype:	VOID winWSetBaseYSpecial (WINDOW win, SHORT y)

	Returns:		none

	Args:			win - window to contain baseline
					y   - y-coord of baseline

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		No test is performed to ensure that the baseline lies within
					the window.  Current text focus is set to start of baseline.
---------------------------------------------------------------------------*/
#define	winWSetBaseYSpecial(W,Y)			((W)->base.tx0	=	0,\
														 (W)->base.ty		=	SetInRange(Y, (W)->font.above, (W)->rect.dx - (W)->font.below),\
														 (W)->base.twid	=	winWGetWinWidth(W),\
														 winWFocusToStart(W))

/*---------------------------------------------------------------------------
	Function:	winWRSetBaseYSpecial

	Descript:	Set the baseline of a window to the default x-origin and
					length for the rectangle indicated, but specify the vertical 
					position (relative to rectangle).

	Prototype:	VOID winWRSetBaseYSpecial (WINDOW win, RECT rect, SHORT y)

	Returns:		none

	Args:			win  - window to contain baseline
					rect - rectangle to be used for baseline positioning
					y    - y-coord of baseline (relative to rectangle)

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		No test is performed to ensure that the baseline lies within
					the rectangle.  Current text focus is set to start of baseline.
---------------------------------------------------------------------------*/
#define	winWRSetBaseYSpecial(W,R,Y)		((W)->base.tx0	=	winXOffset(W,R),\
														 (W)->base.ty		=	SetInRange(Y, (W)->font.above, (R)->dy - (W)->font.below) + winYOffset(W,R),\
														 (W)->base.twid	=	(R)->dx,\
														 winWFocusToStart(W))

/*---------------------------------------------------------------------------
	Function:	winWIncBase

	Descript:	Increment the vertical position of the baseline of a window by
					the amount specified. This will occur provided the new baseline
					will still lie within the window.

	Prototype:	BOOL winWIncBase (WINDOW win, SHORT inc)

	Returns:		TRUE if increment was successful, FALSE otherwise.

	Args:			win - window in which to increment baseline
					inc - amount by which to increment baseline

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWIncBase(W,INC)				(((W)->base.ty + (INC) > (W)->rect.dy) ? FALSE : ((W)->base.ty += (INC), winWFocusToStart(W), TRUE))

/*---------------------------------------------------------------------------
	Function:	winWNextLine

	Descript:	Set the baseline of the window specified to the next text
					line for the current font. This will occur provided the new 
					baseline	will still lie within the window.

	Prototype:	BOOL winWNextLine (WINDOW win)

	Returns:		TRUE if baseline was changed, FALSE otherwise.

	Args:			win - window in which to move to the next line

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWNextLine(W)					winNextLine (W)

/*---------------------------------------------------------------------------
	Function:	winWSetTxtFocus

	Descript:	Set the text focus of a window to the location specified.

	Prototype:	VOID winWSetTxtFocus (WINDOW win, SHORT x)

	Returns:		none

	Args:			win - window containing text focus to reposition
					x   - x-coord of new focus (relative to window)

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Focus lies along baseline.  Any change in vertical positioning 
					must be done by resetting the baseline.  
---------------------------------------------------------------------------*/
#define	winWSetTxtFocus(W,X)					((W)->tx = winSetOnBaseline(X, &((W)->base)),\
														 ChkOnBaseline(X, &((W)->base))) 
														 /* leave this so that last thing done is returned with macro */

/*---------------------------------------------------------------------------
	Function:	winWRSetTxtFocus

	Descript:	Set the text focus of a window to the location specified 
					(relative to the rectangle).

	Prototype:	VOID winWRSetTxtFocus (WINDOW win, RECT rect, SHORT x)

	Returns:		none

	Args:			win  - window to contain baseline
					rect - rectangle to be used for baseline positioning
					x    - x-coord of new text focus (relative to rectangle)

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		No checking is done to see if (x,y) lies within rectangle.
---------------------------------------------------------------------------*/
#define	winWRSetTxtFocus(W,R,X)				((W)->tx = winSetOnBaseline((X) + winXOffset(W,R), &((W)->base)),\
														 ChkOnBaseline((X) + winXOffset(W,R), &((W)->base)) /* TRUE/FALSE */
														 /* leave this so that last thing done is returned with macro */

/*---------------------------------------------------------------------------
	Function:	winWIncTxtFocus

	Descript:	Increment the current text focus of the window indicated by
					the specified amount.  This will occur provided the new text
					focus still lies on the window baseline.

	Prototype:	BOOL winWIncTxtFocus (WINDOW win, SHORT inc)

	Returns:		TRUE if increment was successful, FALSE otherwise.

	Args:			win - window in which to increment text focus 
					inc - amount by which to increment focus

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWIncTxtFocus(W,INC)				winIncTxtFocus(W, INC)

/*---------------------------------------------------------------------------
	Function:	winWDecTxtFocus

	Descript:	Decrement the current text focus of the window indicated by
					the specified amount.  This will occur provided the new text
					focus still lies on the window baseline.

	Prototype:	BOOL winWDecTxtFocus (WINDOW win, SHORT dec)

	Returns:		TRUE if decrement was successful, FALSE otherwise.

	Args:			win - window in which to decrement text focus 
					dec - amount by which to decrement focus

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/27/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWDecTxtFocus(W,DEC)				winDecTxtFocus(W, DEC)

/*---------------------------------------------------------------------------
	Function:	winWPutChar

	Descript:	Writes a character in the current font and style at the text 
	            focus of the window indicated.  The text focus is advanced by 
					the setwidth of the character.  

	Prototype:	BOOL winWPutChar (WINDOW win, CHAR c);

	Returns:		TRUE if character was written, FALSE otherwise (ie character
 					could not fit on baseline).

	Args:			win - window to contain character.
					c   - ASCII code for the character to display.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWPutChar(W,C)						winPutChar(W, C, &(W)->base)

/*---------------------------------------------------------------------------
	Function:	winWPutStr

	Descript:	Writes a string in the current font and style at the text 
	            focus of the window indicated.  The text focus is advanced by 
					the setwidth of each character.

	Prototype:	SHORT winWPutStr (WINDOW win, CHAR *str);

	Returns:		Number of characters successfully written.  If less than total
               in string, then the remaining characters could not fit on 
					baseline.

	Args:			win - window to contain string.
					str - string to be displayed.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Some fonts do not contain all displayable characters.  Any
					non-displayable characters in the string are skipped but return
               value is still decremented.
---------------------------------------------------------------------------*/
#define	winWPutStr(W,P_STR)					winPutStr(W, P_STR)

/*---------------------------------------------------------------------------
	Function:	winWPutNStr

	Descript:	Writes a string of length indicated in the current font and 
					style at the text focus of the window.  The text focus is 
					advanced by the setwidth of each character.

	Prototype:	SHORT winWPutNStr (WINDOW win, CHAR *str, SHORT n);

	Returns:		Number of characters successfully written.  If less than number
               indicated, then the remaining characters could not fit on 
					baseline.

	Args:			win - window to contain character.
					str - string to be displayed.
					n   - number of characters to be displayed.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		String write will terminate after n chars or end-of-string,
					whichever comes first.
					
					since some fonts do not contain 
					all displayable characters, any non-displayable characters in 
					the string are skipped but return value is still incremented.

					Non-displayable characters for a given font are considered to
					have zero width and thus if present in string are skipped but
					considered to have been successfully displayed.
---------------------------------------------------------------------------*/
#define	winWPutNStr(W,P_STR,N)				winPutNStr(W, P_STR, N)

/*---------------------------------------------------------------------------
	Function:	winWPosPutChar

	Descript:	Write a character at the specified position along the baseline
					within the specified window.

	Prototype:	SHORT winWPosPutChar (WINDOW win, SHORT x, CHAR c)

	Returns:		0 - character could not fit on remaining portion of baseline.
					-ve - character does not exist in font.

	Args:			win - window to contain character
					x   - x-coord to reposition text focus to (relative to window)
					c   - ASCII value of character to write

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		If the write was successful, the text focus is updated by the 
					setwidth of the character.
---------------------------------------------------------------------------*/
#define	winWPosPutChar(W,X,C)				((W)->tx = winSetOnBaseline(X, &((W)->base)),\
														 winWPutChar(W, C))

/*---------------------------------------------------------------------------
	Function:	winWRPosPutChar

	Descript:	Write a character at the specified text focus (relative to 
					rectangle) along the window baseline.

	Prototype:	VOID winWRPosPutChar (WINDOW win, RECT rect, SHORT x, CHAR c)

	Returns:		none

	Args:			win  - window to contain character
					rect - rectangle to be used for text focus positioning
					x    - x-coord of new text focus (relative to rectangle)
					c    - ASCII value of character to write

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		1. char not in font set
					2. focus not within baseline
					3. char would not fit on baseline.
					If the specified text focus is outside the baseline boundaries,
					the text focus is set to the appropriate end of the baseline
					and the write is unsuccessful.  If the write was successful, 
					the text focus is updated by the setwidth of the character.
---------------------------------------------------------------------------*/
#define	winWRPosPutChar(W,R,X,C)			((W)->tx = winSetOnBaseline((X) + winXOffset(W,R), &((W)->base))\
														 winWPutChar(W, C))

/*---------------------------------------------------------------------------
	Function:	winWPosPutStr

	Descript:	Write a string at the specified position within a window.

	Prototype:	SHORT winWPosPutStr (WINDOW win, SHORT x, CHAR *str)

	Returns:		number of characters written

	Args:			win - window to contain string
					x   - x-coord of new text focus (relative to window)
					str - string to write

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		The text focus of the window is set to the position specified.
					For each successful character write, the text focus is updated 
					by the setwidth of that character.
---------------------------------------------------------------------------*/
#define	winWPosPutStr(W,X,P_STR)				((W)->tx = winSetOnBaseline(X, &((W)->base)),\
														 winPutStr(W, P_STR)) 

/*---------------------------------------------------------------------------
	Function:	winWRPosPutStr

	Descript:	Write a string at the specified position within a rectangle.

	Prototype:	SHORT winWRPosPutStr (WINDOW win, RECT rect, SHORT x, CHAR *str)

	Returns:		number of characters written

	Args:			win  - window containing baseline
					rect - rectangle to contain string
					x    - x-coord of start of string (relative to rectangle)
					str  - string to write

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		The text focus of the parent window is set to the position 
					specified.  For each successful character write, the text focus 
					is updated by the setwidth of that character.
---------------------------------------------------------------------------*/
#define	winWRPosPutStr(W,R,X,P_STR) 		((W)->tx = winSetOnBaseline((X) + winXOffset(W,R), &((W)->base))\
														 winPutStr(W, P_STR)) 

/*---------------------------------------------------------------------------
	Function:	winWPosPutNStr

	Descript:	Write n characters from a string at the specified position 
					within a window.

	Prototype:	SHORT winWPosPutNStr (WINDOW win, SHORT x, CHAR *str, SHORT n)

	Returns:		number of characters written

	Args:			win - window to contain string
					x   - x-coord of new text focus (relative to window)
					str - string to write
					n   - number of characters to write

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		The text focus of the window is set to the position specified.
					For each successful character write, the text focus is updated by the 
					setwidth of that character.
---------------------------------------------------------------------------*/
#define	winWPosPutNStr(W,X,P_STR,N)			((W)->tx = winSetOnBaseline(X, &((W)->base)),\
														 winPutNStr(W, P_STR, N)) 

/*---------------------------------------------------------------------------
	Function:	winWRPosPutNStr

	Descript:	Write n characters from a string at the specified position 
					within a rectangle.

	Prototype:	SHORT winWRPosPutNStr (WINDOW win, RECT rect, SHORT x, CHAR *str, SHORT n)

	Returns:		number of characters written

	Args:			win  - window containing baseline
					rect - rectangle to contain string
					x   - x-coord of start of string (relative to rectangle)
					str - string to write
					n   - number of characters to write

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		The text focus of the parent window is set to the position 
					specified.  For each successful character write, the text focus
					is updated by the setwidth of that character.
---------------------------------------------------------------------------*/
#define	winWRPosPutNStr(W,R,X,P_STR,N) 	((W)->tx = winSetOnBaseline((X) + winXOffset(W,R), &((W)->base))\
														 winPutNStr(W, P_STR, N)) 

/*---------------------------------------------------------------------------
	Function:	winWPutStrCenter 

	Descript:	Write a string in the window indicated, horizontally centered
					about the baseline.

	Prototype:	SHORT winWPutStrCenter (WINDOW win, CHAR *str)

	Returns:		number of characters successfully written

	Args:			win  - window containing baseline
					str - string to write

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Strings longer than the baseline will be written from the 
					left edge of the baseline until right edge is reached.
---------------------------------------------------------------------------*/
#define	winWPutStrCenter(W,P_STR)			(winPutStrCenter(W, P_STR))

/*--------------------------------------------------------------------------*/
/* CURSOR ROUTINES                                                          */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWSetBlinkRate

	Descript:	Set the text-cursor blink-rate for the window specified.

	Prototype:	WINDOW winWSetBlinkRate (WINDOW win, SHORT interval);

	Returns:		none.

	Args:			win - window to set cursor blink-rate for.
					interval - number of clock-ticks per blink.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWSetBlinkRate(W, INTVL)		((W)->blink=INTVL)

/*---------------------------------------------------------------------------
	Function:	winWOnTextCursor

	Descript:	Make the window indicated be the active window.  A text cursor
					is placed at the current text focus of this window.

	Prototype:	WINDOW winWOnTextCursor (WINDOW win);

	Returns:		none

	Args:			win - window to be activated.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWOnTextCursor(W)					winOnTextCursor(W, &(W)->base)

/*---------------------------------------------------------------------------
	Function:	winWOffTextCursor

	Descript:	Turn off the currently active text cursor.

	Prototype:	WINDOW winWOffTextCursor (void);

	Returns:		The window owning the text cursor.

	Args:			none

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/11/91

	Notes:		This routine turns off the current text cursor regardless of
	            who owns it. It is really only reasonable to call this if
					the caller is certain that it owns the text cursor.
---------------------------------------------------------------------------*/
#define	winWOffTextCursor()					winOffTextCursor()

/*---------------------------------------------------------------------------
	Function:	winWGetTextCursor

	Descript:	Return the window containing the currently active text cursor.
					NULL is returned if there is no active text cursor. The state
					of the cursor is unchanged.

	Prototype:	WINDOW winWGetTextCursor (VOID);

	Returns:		The window currently containing the text cursor.

	Args:			none

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		This can be used to test before calling winOffTextCursor()
					The value returned is the window structure of the window
					containing the text cursor.
---------------------------------------------------------------------------*/
#define	winWGetTextCursor()					winGetTextCursor()

/*---------------------------------------------------------------------------
	Function:	winWPushTextCursor

	Descript:	Pushes the current window on a cursor stack and establishes a
					new cursor in the window indicated.

	Prototype:	VOID winWPushTextCursor (WINDOW win);

	Returns:		none

	Args:			win - Window to contain the text cursor.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Popup windows should use this routine and the corresponding
					winPopTextCursor routine.
---------------------------------------------------------------------------*/
#define	winWPushTextCursor()				winPushTextCursor()

/*---------------------------------------------------------------------------
	Function:	winWPopTextCursor

	Descript:	Replaces the current text cursor with the previous one, ie
					inactivates the currently active window and activates the 
					previous one.

	Prototype:	VOID winWPopTextCursor (VOID);

	Returns:		none

	Args:			none

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		Popup windows should use this routine and the corresponding
					winPushTextCursor routine.
---------------------------------------------------------------------------*/
#define	winWPopTextCursor()					winPopTextCursor()

/*---------------------------------------------------------------------------
	Function:	winWTextCursorMouse

	Descript:	Asks the cursor task to suspend while the mouse is active

	Prototype:	void	winWTextCursorMouse

	Returns:		none

	Args:			none

	Includes:	none

	In Catalog:	Yes										Date:	06/27/91

	Notes:		none
---------------------------------------------------------------------------*/

#define winWTextCursorMouse() winTextCursorMouse()

/*---------------------------------------------------------------------------
	Function:	winWTextCursorBlink

	Descript:	Asks the cursor task to start the current cursor blinking.

	Prototype:	void	winWTextCursorBlink( void );

	Returns:		none

	Args:			none

	Includes:	none

	In Catalog:	Yes										Date:	06/27/91

	Notes:		none
---------------------------------------------------------------------------*/

#define winWTextCursorBlink()  winTextCursorBlink()

/*---------------------------------------------------------------------------
	Function:	winWTextCursorStop

	Descript:	Asks the cursor task to start the current cursor blinking.

	Prototype:	void	winWTextCursorStop( void );

	Returns:		none

	Args:			none

	Includes:	none

	In Catalog:	Yes										Date:	06/27/91

	Notes:		none
---------------------------------------------------------------------------*/

#define winWTextCursorStop()  winTextCursorStop()

/*--------------------------------------------------------------------------*/
/* WINDOW STRUCTURE ACCESS ROUTINES                                         */
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
	Function:	winWGetX

	Descript:	Get the x-origin of the window specified

	Prototype:	SHORT winWGetX (WINDOW win)

	Returns:		the window x-origin

	Args:			win - the window to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetX(W)								((W)->rect.x)

/*---------------------------------------------------------------------------
	Function:	winRGetX

	Descript:	Get the x-origin of the rectangle specified

	Prototype:	SHORT winRGetX (RECT rect)

	Returns:		the width

	Args:			rect - the rectangle to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRGetX(R)								((R)->x)

/*---------------------------------------------------------------------------
	Function:	winWGetY

	Descript:	Get the y-origin of the window specified

	Prototype:	SHORT winWGetY (WINDOW win)

	Returns:		the window y-origin

	Args:			win - the window to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetY(W)								((W)->rect.y)

/*---------------------------------------------------------------------------
	Function:	winRGetY

	Descript:	Get the y-origin of the rectangle specified

	Prototype:	SHORT winRGetY (RECT rect)

	Returns:		the width

	Args:			rect - the rectangle to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRGetY(R)								((R)->y)

/*---------------------------------------------------------------------------
	Function:	winWGetDx

	Descript:	Get the width of the window specified

	Prototype:	SHORT winWGetDx (WINDOW win)

	Returns:		the window width

	Args:			win - the window to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetDx(W)							(winWGetWinWidth(W))

/*---------------------------------------------------------------------------
	Function:	winRGetDx

	Descript:	Get the width of the rectangle specified

	Prototype:	SHORT winRGetDx (RECT rect)

	Returns:		the width

	Args:			rect - the rectangle to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRGetDx(R)							((R)->dx)

/*---------------------------------------------------------------------------
	Function:	winWGetDy

	Descript:	Get the height of the window specified

	Prototype:	SHORT winWGetDy (WINDOW win)

	Returns:		the window height

	Args:			win - the window to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetDy(W)							((W)->rect.dy)

/*---------------------------------------------------------------------------
	Function:	winRGetDy

	Descript:	Get the height of the rectangle specified

	Prototype:	SHORT winRGetDy (RECT rect)

	Returns:		the height

	Args:			rect - the rectangle to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRGetDy(R)							((R)->dy)

/*---------------------------------------------------------------------------
	Function:	winWGetBgc

	Descript:	Get the background color for the window specified

	Prototype:	SYSCOLOR winWGetBgc (WINDOW win)

	Returns:		the background color

	Args:			win - the window to get the background color from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetBgc(W)							((W)->rect.bgc)

/*---------------------------------------------------------------------------
	Function:	winRGetBgc

	Descript:	Get the background color for the rectangle specified

	Prototype:	SYSCOLOR winRGetBgc (RECT rect)

	Returns:		the background color

	Args:			rect - the rectangle to get the background color from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRGetBgc(R)							((R)->bgc)

/*---------------------------------------------------------------------------
	Function:	winWGetFgc

	Descript:	Get the foreground color for the window specified

	Prototype:	SYSCOLOR winWGetFgc (WINDOW win)

	Returns:		the foreground color

	Args:			win - the window to get the foreground color from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetFgc(W)							((W)->rect.fgc)

/*---------------------------------------------------------------------------
	Function:	winRGetFgc

	Descript:	Get the foreground color for the rectangle specified

	Prototype:	SYSCOLOR winRGetFgc (RECT rect)

	Returns:		the foreground color

	Args:			rect - the rectangle to get the foreground color from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRGetFgc(R)							((R)->fgc)

/*---------------------------------------------------------------------------
	Function:	winWGetBdc

	Descript:	Get the border color for the window specified

	Prototype:	SYSCOLOR winWGetBdc (WINDOW win)

	Returns:		the border color

	Args:			win - the window to get the border color from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetBdc(W)							((W)->rect.bdc)

/*---------------------------------------------------------------------------
	Function:	winRGetBdc

	Descript:	Get the border color for the rectangle specified

	Prototype:	SYSCOLOR winRGetBdc (RECT rect)

	Returns:		the border color

	Args:			rect - the rectangle to get the border color from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRGetBdc(R)							((R)->bdc)

/*---------------------------------------------------------------------------
	Function:	winWGetBdw

	Descript:	Get the border width for the window specified

	Prototype:	SHORT winWGetBdw (WINDOW win)

	Returns:		the border width

	Args:			win - the window to get the border width from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetBdw(W)							((W)->rect.bdw)

/*---------------------------------------------------------------------------
	Function:	winRGetBdw

	Descript:	Get the border width for the rectangle specified

	Prototype:	SHORT winRGetBdw (RECT rect)

	Returns:		the border width

	Args:			rect - the rectangle to get the border width from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winRGetBdw(R)							((R)->bdw)

/*---------------------------------------------------------------------------
	Function:	winWGetFontId

	Descript:	Get the current font-id for the window specified

	Prototype:	SYSFONTID winWGetFontId (WINDOW win)

	Returns:		the current font-id

	Args:			win - the window to get the font-id from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetFontId(W)							((W)->font.id)

/*---------------------------------------------------------------------------
	Function:	winWGetFontAbove

	Descript:	Get the number of lines above baseline for the current font for 
					the window specified

	Prototype:	SHORT winWGetFontAbove (WINDOW win)

	Returns:		the number of lines above the baseline

	Args:			win - the window to get the font info from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetFontAbove(W)							((W)->font.above)

/*---------------------------------------------------------------------------
	Function:	winWGetFontBelow

	Descript:	Get the number of lines below baseline for the current font for 
					the window specified

	Prototype:	SHORT winWGetFontBelow (WINDOW win)

	Returns:		the number of lines below the baseline

	Args:			win - the window to get the font info from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetFontBelow(W)							((W)->font.below)

/*---------------------------------------------------------------------------
	Function:	winWGetFontHt

	Descript:	Get the height of the current font for the window specified

	Prototype:	SHORT winWGetFontHt(WINDOW win)

	Returns:		the height of the current font

	Args:			win - the window to get the font-height from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetFontHt(W)							((W)->font.above + (W)->font.below)

/*---------------------------------------------------------------------------
	Function:	winWGetFontLead

	Descript:	Get the leading for the current font for the window specified

	Prototype:	SHORT winWGetFontLead (WINDOW win)

	Returns:		the leading for the current font

	Args:			win - the window to get the font info from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetFontLead(W)							((W)->font.lead)

/*---------------------------------------------------------------------------
	Function:	winWGetType

	Descript:	Get the current window type for the window specified

	Prototype:	SHORT winWGetType (WINDOW win)

	Returns:		the current window type

	Args:			win - the window whose type to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetType(W)							((W)->type)

/*---------------------------------------------------------------------------
	Function:	winWGetTxtFocus

	Descript:	Get the current text focus for the window specified

	Prototype:	SHORT winWGetTxtFocus (WINDOW win)

	Returns:		the current text focus

	Args:			win - the window to get the text focus from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetTxtFocus(W)					((W)->tx)

/*---------------------------------------------------------------------------
	Function:	winWGetBaseY

	Descript:	Get the vertical position of the specified window's baseline 
					relative to the window.

	Prototype:	SHORT winWGetBaseY (WINDOW win)

	Returns:		the baseline height

	Args:			win - the window to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetBaseY(W)						((W)->base.ty)

/*---------------------------------------------------------------------------
	Function:	winWGetBase

	Descript:	Get the current baseline from the specified window.

	Prototype:	BASE winWGetBase (WINDOW win)

	Returns:		the baseline structure for the window

	Args:			win - the window to get the baseline from

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetBase(W)							(&((W)->base))

/*---------------------------------------------------------------------------
	Function:	winWGetBlinkrate

	Descript:	Get the cursor blink-rate associated with the specified window.

	Prototype:	SHORT winWGetBlinkrate (WINDOW win)

	Returns:		the blink-rate

	Args:			win - the window to check

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWGetBlinkrate(W)					((W)->blink)

/*---------------------------------------------------------------------------
	Function:	winRGetWin

	Descript:	Find the parent window associated with the specified rectangle.

	Prototype:	WINDOW winRGetWin (RECT rect)

	Returns:		the parent window

	Args:			rect - the rectangle to get the parent window of

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 03/13/91

	Notes:		none
---------------------------------------------------------------------------*/
/* #define	winRGetWin(R)							winGetWin(R) */


/*---------------------------------------------------------------------------
	Function:	winWGetExtendedWidth

	Descript:	Returns the total length of the window including
			extensions in pixels.

	Prototype:	SHORT	winWGetExtendedWidth( WINDOW w );

	Returns:		Total length in pixels.

	Args:	w	- Pointer to the window to get the width for.

	Includes:	windows.h

	In Catalog:	Yes										Date:	06/17/91

	Notes:		none
---------------------------------------------------------------------------*/
#define winWGetExtendedWidth(W) ((SHORT) extGetWidth(W))

/*---------------------------------------------------------------------------
	Function:	winWShiftLeft

	Descript:	Shifts an extended window left by an amount specified

	Prototype:	SHORT	winWShiftLeft( WINDOW w, SHORT shift );

	Returns:		The actual amount of the shift.

	Args:	w	- Extended window to shift.
			shift	- Number of pixels to shift left. Assumed a multiple of 8.

	Includes:	windows.h

	In Catalog:	 No										Date:	06/17/91

	Notes:		none
---------------------------------------------------------------------------*/

#define winWShiftLeft( W, SHIFT ) (CHAR_BIT * winShiftLeft( W, ((SHIFT) / (CHAR_BIT))))

/*---------------------------------------------------------------------------
	Function:	winWShiftRight

	Descript:	Shifts an extended window right by an amount specified

	Prototype:	SHORT	winWShiftRight( WINDOW w, SHORT shift );

	Returns:		The actual amount of the shift.

	Args:	w	- Extended window to shift.
			shift	- Number of pixels to shift right. Assumed a multiple of 8.

	Includes:	windows.h

	In Catalog:	 No										Date:	06/17/91

	Notes:		none
---------------------------------------------------------------------------*/

#define winWShiftRight( W, SHIFT ) ((CHAR_BIT) * winShiftRight( W, ((SHIFT) / (CHAR_BIT))))


/*---------------------------------------------------------------------------
	Function:	winWGetLeftShift

	Descript:	Returns the number of pixels shifted left.

	Prototype:	SHORT	winWGetLeftShift( WINDOW w );

	Returns:		See above

	Args:	w	- Extended window of which to get the width.

	Includes:	windows.h

	In Catalog:	 No										Date:	06/17/91

	Notes:		none
---------------------------------------------------------------------------*/

#define winWGetLeftShift( W ) ((SHORT) extGetLeftShift( W ))

/*---------------------------------------------------------------------------
	Function:	winWMakeExtended

	Descript:	Extends an existing window by some number of pixels.
			The shiftable interior is assumed the same height as the
			window interior and is assumed have the width of the largest number of
			display pixels completely contained in the window.

	Prototype:	BOOL	winWMakeExtended( WINDOW w, SHORT n );

	Returns:		Returns TRUE if some sort of error occurred. This
			shouldn't occur, blah blah.

	Args:	w	- Window handle of the window to extend.
			n	- Number of characters by which to extend the window.

	Includes:	win1.p

	In Catalog:	Yes										Date:	05/18/91

	Notes:		none
---------------------------------------------------------------------------*/
#define winWMakeExtended( W, N )  winMakeExtended( W, ((N) / (CHAR_BIT)) )
/*---------------------------------------------------------------------------
	Function:	winWGetShiftRegion

	Descript:	Returns the length of a shiftable region of a window
			in pixels. This is less than or equal to the width of the window.

	Prototype:	SHORT	winWGetShiftRegion( WINDOW w );

	Returns:		Length in pixels of the shiftable region of the win-
			dow.

	Args:	w	- Window identifier.

	Includes:	windows.h

	In Catalog:	Yes										Date:	06/19/91

	Notes:		none
---------------------------------------------------------------------------*/
#define winWGetShiftRegion( W )  extGetShiftRegion( W )

#define winWIsWinExtended(W)	((W)->dispsurf.dlink.dl_n != (DLINK) 0)
#define winWIsWinEditwin(W)	winWIsWinExtended(W)
/*---------------------------------------------------------------------------
	Function:	winWPutCharUnderln

	Descript:	Writes an underlined character in the current font at the text 
	            focus of the window indicated.  The text focus is advanced by 
					the setwidth of the character.  

	Prototype:	BOOL winWPutCharUnderln (WINDOW win, UCHAR c);

	Returns:		TRUE if character was written, FALSE otherwise (ie character
 					could not fit on baseline).

	Args:			win - window to contain character.
					c   - ASCII code for the character to display.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 05/16/91

	Notes:		none
---------------------------------------------------------------------------*/
#define	winWPutCharUnderln(W,C)	((W)->attr = UNDER_ALL_ACTIVE, winWPutChar(W, C), (W)->attr = 0)

/*---------------------------------------------------------------------------
	Function:	winWPutStrUnderln

	Descript:	Writes an underlined string in the current font at the text 
	            focus of the window indicated.  The text focus is advanced by 
					the setwidth of the characters written.

	Prototype:	SHORT winWPutStrUnderln (WINDOW win, UCHAR *str);

	Returns:		Number of characters successfully written.  If less than total
               in string, then the remaining characters could not fit on 
					baseline.

	Args:			win - window to contain string.
					str - string to be displayed.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 05/16/91

	Notes:		Some fonts do not contain all displayable characters.  Any
					non-displayable characters in the string are skipped but return
               value is still decremented.
---------------------------------------------------------------------------*/
#define	winWPutStrUnderln(W,P_STR)					((W)->attr = UNDER_ALL_ACTIVE, winWPutStr(W,P_STR), (W)->attr = 0)

/*---------------------------------------------------------------------------
	Function:	winWPutStrUnderlnN

	Descript:	Writes a string in the current font with the n-th character
					underlined at the text focus of the window indicated.  The 
					text focus is advanced by the setwidth of the characters
					written.

	Prototype:	SHORT winWPutStrUnderlnN (WINDOW win, UCHAR *str, SHORT n);

	Returns:		Number of characters successfully written.  If less than total
               in string, then the remaining characters could not fit on 
					baseline.

	Args:			win	- window to contain string.
					str	- string to be displayed.
					n		- position in string of character to be underlined.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 05/16/91

	Notes:		Some fonts do not contain all displayable characters.  Any
					non-displayable characters in the string are skipped but return
               value is still decremented.
---------------------------------------------------------------------------*/
#define	winWPutStrUnderlnN(W,P_STR,N)					winPutStrUnderlnN(W, P_STR, N)
/*---------------------------------------------------------------------------
	Function:	winWPutNStrUnderln

	Descript:	Writes an underlined string of length n in the current 
					font at the text focus of the window.  The text focus is 
					advanced by the setwidth of the characters written.

	Prototype:	SHORT winWPutNStrUnderln (WINDOW win, UCHAR *str, SHORT n);

	Returns:		Number of characters successfully written.  If less than number
               indicated, then the remaining characters could not fit on 
					baseline.

	Args:			win - window to contain character.
					str - string to be displayed.
					n   - number of characters to be displayed.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 05/16/91

	Notes:		String write will terminate after n chars or end-of-string,
					whichever comes first.
					
					Any non-displayable characters in the string are skipped but 
					are considered to have been successfully displayed and thus
					the return count is still incremented for those characters.
---------------------------------------------------------------------------*/
#define	winWPutNStrUnderln(W,P_STR,N)				((W)->attr = UNDER_ALL_ACTIVE, winWPutNStr(W, P_STR), (W)->attr = 0)

/*---------------------------------------------------------------------------
	Function:	winWPutNStrUnderlnN

	Descript:	At the window text focus, writes a string of length n1 in the
					current font with the character at position n2 in the string
					underlined.  The text focus is advanced by the setwidth of 
					the characters written.

	Prototype:	SHORT winWPutNStrUnderlnN (WINDOW win, UCHAR *str, SHORT n1, SHORT n2);

	Returns:		Number of characters successfully written.  If less than number
               indicated, then the remaining characters could not fit on 
					baseline.

	Args:			win	- window to contain character.
					str	- string to be displayed.
					n1		- number of characters to be displayed.
					n2		- position in string of character to be underlined.

	Includes:	windows.h winmacro.h

	In Catalog:	Yes										Date: 05/10/91

	Notes:		String write will terminate after n chars or end-of-string,
					whichever comes first.
					
					Any non-displayable characters in the string are skipped but 
					are considered to have been successfully displayed and thus
					the return count is still incremented for those characters.
---------------------------------------------------------------------------*/
#define winWPutNStrUnderlnN(W,P_STR,N1,N2)				winPutNStrUnderlnN(W, P_STR, N1, N2)
#endif

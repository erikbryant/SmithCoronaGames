/****************************************************************************
*
*		$Workfile:   win1.p  $	- Header file.
*
*		$Revision:   1.18  $			$Modtime:   10 Dec 1991 14:44:34  $
*
*		$Author:   AJW  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/win1.p_v  $
* 
*    Rev 1.18   11 Dec 1991 09:57:18   AJW
* changed prototypes ChngTextCursor(), TextCursorPattern;
* new prototypes TmpCursorPattern() and TextCursorMouse().
* 
*    Rev 1.17   23 Jul 1991 14:03:08   rwh
* Changes made for changed cursor task.
* 
*    Rev 1.16   24 Jun 1991 10:20:24   ajw
* Added window extension software.
* 
*    Rev 1.15   14 May 1991 14:16:02   ajw
* New prototypes charBoldWidth() and charFAWidth().
* 
*    Rev 1.14   03 May 1991 06:02:46   rwh
* changes to add attributes
* 
* 
*    Rev 1.13   25 Apr 1991 13:13:52   rwh
* changed prototypes for win5 functs.
* 
*    Rev 1.12   25 Apr 1991 10:09:56   rwh
* changes for new functional prototypes.
* 
*    Rev 1.11   19 Apr 1991 15:07:46   ajw
* Added winGetCompl() prototype.
* 
*    Rev 1.10   18 Apr 1991 09:23:04   rwh
* Changed prototypes for cursor functions.
* 
*    Rev 1.9   11 Apr 1991 19:23:14   ajw
* All-new, revamped edition.  Complete rerun of the current source files.
* 
*    Rev 1.8   09 Apr 1991 15:06:26   ajw
* Same as 1.6
* 
*    Rev 1.7   09 Apr 1991 14:56:24   ajw
* No change.
* 
*    Rev 1.6   26 Mar 1991 08:51:08   rwh
* Reintroduced functional prototypes removed earlier.
* 
*    Rev 1.5   26 Mar 1991 07:43:08   rwh
* Removed prototypes for moveRow{Up,Down,Left,Right}
* 
*    Rev 1.4   25 Mar 1991 10:29:56   ajw
* Incorporated macros => deleted some functions and changed function 
* calls/param lists.
* 
*    Rev 1.3   19 Mar 1991 12:58:54   rwh
* changes for hardware windows.
* 
*    Rev 1.2   07 Mar 1991 13:33:50   rwh
* Removed functional prototype.
* 
*    Rev 1.1   07 Mar 1991 13:19:34   rwh
* Changed functional prototypes for scroll functions.
* 
*    Rev 1.0   07 Mar 1991 11:42:32   ajw
* Initial revision.
* 
*    Rev 1.9   02 Mar 1991 18:27:42   rwh
* added bitmap functional prototype
* 
*    Rev 1.8   26 Feb 1991 11:03:50   rwh
* Changed functional prototype for winMapBits.
* 
*    Rev 1.7   25 Feb 1991 15:48:44   ajw
* See previous change.
* 
*    Rev 1.6   25 Feb 1991 15:42:04   ajw
* See previous change.
*
*    Rev 1.5   25 Feb 1991 15:39:12   ajw
* winRectClear: new routine.
* 
*    Rev 1.4   25 Feb 1991 15:02:28   ajw
* Installed header.
*
*    Rev 1.3   25 Feb 1991            ajw
* winRectDrawBitmap: new routine.
*
*    Rev 1.2   25 Feb 1991            ajw
* winDrawBitmap: new prototype.
*
*
*
****************************************************************************/


#ifndef WIN1P
#define WIN1P

/* win0.c: */
void bitmap( PARAMBLK pbk, SHORT absx, SHORT absy, BITMAP map);

/* win1.c: */
WINDOW winCreate( SHORT x, SHORT y, SHORT dx, SHORT dy, SYSCOLOR bgc, 
                 SYSCOLOR fgc, SYSCOLOR bdc, SHORT bdw, SYSWINTYPE type );
WINDOW winChildCreate (WINDOW parent, SHORT x, SHORT y, SHORT dx, SHORT dy,
					  SYSCOLOR bgc, SYSCOLOR fgc, SYSCOLOR bdc, SHORT bdw, SYSWINTYPE type,  
					  BOOL saveback);
void winDestroy(WINDOW *w);
void winInit(void);
WINDOW winAlloc(void);
void winDealloc( WINDOW *w );
SYSFONTID fontGetDefault(SYSWINTYPE type);
void winEraseChar( WINDOW w, CHAR c );
void winDrawBitmap( WINDOW w, SHORT x, SHORT y, BITMAP bitvec, SYSCOLOR bgc, SYSCOLOR fgc);
void winSetDrawMode( WINDOW w, USHORT mode );
BITUNIT mkmask(SHORT *bit0, SHORT *dx);
void winRoot(SYSCOLOR color);

/* win2.c: */
VOID winScroll( WINDOW win, SHORT ulx, SHORT uly, SHORT dx, SHORT dy,
	                            SHORT direction, SHORT number, SYSCOLOR color );
VOID winRectOp(WINDOW w, SHORT x0, SHORT y0, SHORT dx0, SHORT dy0, RECTOP op, SYSCOLOR color);
VOID winRectBdLims (RECT rect, SHORT *x, SHORT *y, SHORT *dx, SHORT *dy, SHORT *w);
VOID winBorderCreate (WINDOW parent, RECT rect, SYSCOLOR bdc, SHORT bdw);

/* win3.c: */
void  winOnTextCursor( WINDOW w, BASE base);
void  winCursorOp( ULONG  op);
void  winCursorMouse(void);
void  winTextCursorTask(void);
void  curmsg( ULONG  *msg, ULONG xtra);
BOOL winChngTextCursor( WINDOW w );
void  winTextCursorPattern( USHORT fid, USHORT attr, BITMAP cursor, SHORT widthtype);
WINDOW winGetTextCursor(void);
void  winOffTextCursor(void);
void  winPushTextCursor(void);
void  winPopTextCursor(void);
void  winTextCursorBlink(void);
void  winTextCursorStop(void);
void winTmpCursorPattern( BASE base, BITMAP cursor, SHORT widthtype );
void	winTextCursorMouse(void);

/* win4.c: */
VOID winSetFont (WINDOW w, SYSFONTID fontid);
BOOL winIncTxtFocus (WINDOW win, SHORT inc);
BOOL winDecTxtFocus (WINDOW win, SHORT dec);
BOOL winNextLine( WINDOW w );
BOOL winPutChar( WINDOW w, UCHAR c, BASE base );
SHORT winPutStr (WINDOW w, CHAR *s);
SHORT winPutNStr (WINDOW w, CHAR *s, SHORT n);
SHORT winPutStrCenter (WINDOW w, CHAR *str);
SHORT winStringNSize(WINDOW win, UCHAR *str, SHORT n);
VOID charBoldWidth(CHARPARMS chp, SHORT *offset, SHORT *setwidth);
SHORT charFAWidth(UCHAR c, SYSFONTID font_id, USHORT attr);
VOID winRectAssign (WINDOW win, RECT rect, SHORT x, SHORT y, SHORT dx, SHORT dy,
					SYSCOLOR bgc, SYSCOLOR fgc, SYSCOLOR bdc, SHORT bdw);
SYSCOLOR winGetCompl (SYSCOLOR color);

/* win5.c: */
SHORT winBulletOn (WINDOW w, SHORT x, SHORT y, SHORT size);
SHORT winBulletOff (WINDOW w, SHORT x, SHORT y, SHORT size);
SHORT winChkboxOn (WINDOW w, SHORT x, SHORT y, SHORT size);
SHORT winChkboxOff (WINDOW w, SHORT x, SHORT y, SHORT size);
SHORT winIconOn (WINDOW w, SHORT x, SHORT y, SHORT size);
SHORT winIconOff (WINDOW w, SHORT x, SHORT y, SHORT size);
SHORT winFnctboxOn (WINDOW w, SHORT x, SHORT y, SHORT size);
SHORT winFnctboxOff (WINDOW w, SHORT x, SHORT y, SHORT size);

/* win6.c: */
hexout( WINDOW w, ULONG x );

/* win7.c: */

SHORT	winShiftLeft( WINDOW w, SHORT n );
SHORT	winShiftRight( WINDOW w, SHORT n );
BOOL	winMakeExtended( WINDOW w, SHORT n );
BITUNIT	*extNextCh( register PARAMBLK pbk );
BITUNIT	*extPrevCh( register PARAMBLK pbk );
void  extFindLoc( register PARAMBLK pbk );
SHORT h_adjust( register WINDOW w, register PARAMBLK pbk, SHORT x );
SHORT v_adjust( WINDOW w, SHORT y );
SHORT extGetWidth( register WINDOW w );
SHORT extGetShiftRegion( register WINDOW w );

/* Miscellaneous: contained in bitrev.a68 or bitfncs.a68 */
BITUNIT bitrev( BITUNIT );
BITUNIT *c_elem( struct paramblk * );
BITUNIT bitfn_xor( BITUNIT, BITUNIT );
BITUNIT bitfn_ior( BITUNIT, BITUNIT );
BITUNIT bitfn_nand( BITUNIT, BITUNIT );
ULONG jsys( ULONG fid,...);
#endif

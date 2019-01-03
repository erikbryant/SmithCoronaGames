/* downport.h */

/* Intended for CoronaCalc Portability */

/* Macros for access to character screen */

/******************/
/* screen output  */
/******************/
#define Tinit() rawTinit ()
#define Tquote(C) rawPutChr(C)
#define Tmaxcol() rawMaxCol()
#define Tmaxrow() rawMaxRow()
#define Tcleol()	rawClrEol()
#define Tcleow()	rawClrEow()
#define Tsetrow(R)  rawSetRow(R)
#define Tsetcol(C) rawSetCol(C)
#define Tundon() rawUndOn()
#define Tundoff() rawUndOff()
#define Tstdon() rawRevVideoOn()
#define Tstdoff() rawRevVideoOff()
#define Tclose() rawWinClose()

#define Tbell()		keybdBeep()


/******************/
/* keyboard input */
/******************/
#define Tgetc()		commKeysGet()
#define Tkbhit()		commKeysHit()


/******************/
/* printer output */
/******************/
#define pputchar(C)		prntPutChar(C)


/******************/
/* disk functions */
/******************/
#define lseek(F,L,W)		fileXSeek( F, L, W )
#define unlink(S)			diskRemoveFile( S, 0x7FFF )
#define read(F, B, I)	fileXRead( F, B, I, 0x7FFF )
#define write(F, B, I)	fileXWrite( F, B, I, 0x7FFF )
#define open(S, M)		fileXOpen( S, M )
#define close(F)			fileClose( F, 0x7FFF)


/*********************/
/* memory allocation */
/*********************************************************************/
/* direct fn calls, no macros needed 											*/
/* a = malloc(i) 				Allocate "i" bytes of memory 					*/
/* free(a)						Free memory at address "a" 				 	*/
/* i = msize() 				Return size of largest contiguous block 	*/
/*********************************************************************/

#define exit(rc) execTDwnExit()

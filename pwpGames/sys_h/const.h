#ifndef CONSTH
#define CONSTH

#define BF_SIZE 244
#define MASKON 1
#define ONEOF  2
#define ANYBUT 3
#define EXACTSTRING 5
#define NOCASE 6
#define BUFHEADRINFO 4
#define BUFNOMATCH	-1

#define FONTNAMESIZE 32
#define DOCNAMESIZE 32

#define MAXPAGES 65536

#if 0
/* Definitions for commands and attributes used in data buffers. */
#define	ATT_LNL		BF_ATTR + 0x7A		/* Logical newline character */
#define	ATT_PNL		BF_ATTR + 0x7B		/* Physical newline char.('\n' xlated) */
#define  ATT_EXTASC	BF_ATTR + 0x70		/* Extended Ascii sequence introducer */

/* All of these 8-bit codes have the most significant bit set.  */

#define CHARMASK ((UCHAR) 0xFF) /* Canonical character mask */

/* Horizontal Positioning characters */

#define BFHZFIELD		0xc0
#define BFHZCODE		0x80
#define BFHZDATA		( BFHZFIELD ^ CHARMASK )

/* Vertical Positioning characters */

#define BFVTFIELD		0xe0
#define BFVTCODE		0xc0
#define BFVTDATA		( BFVTFIELD ^ CHARMASK )

/* Font field characters */

#define BFFNTFIELD  	0xf0
#define BFFNTCODE	  	0xe0
#define BFFNTDATA	  	( BFFNTFIELD ^ CHARMASK )
#define FONTESC		( BFFNTFIELD ^ CHARMASK )

/* Everything else       */

#define BFFIELD		0xf0
#define BFCODE			0xf0
#define BFDATA			( BFFIELD ^ CHARMASK )

/* Variable length fields: completely variable, or 1,2 or 3 characters */

#define BFVARLEN		0xfc
#define BFVARLENDATA	(BFVARLEN ^ CHARMASK)	
#endif

#define BUFCODE 0xff
#define BUFESC	0x80				/* this is set in the type parameter in match. */
#define BUFATTLEN 2
#define BUFCOUNTMARK 0xff
#define BUFNOPARAM 0x80
#define BUF2CHAR	 0x80
#define BUF3CHAR   0xc0
#define BUF4CHAR   0xe0
#define BUF5CHAR   0xf0

/* #define NULL			0 */
#define MAXMSG   		256
#define MAXKEYS		2
#define ECHO_MAXECHO	256
#define ECHO_MAXESC 	256
#define CURS_MAXMICE	256
#define MINSIZE		8
#define ERRX_SIZE		80
#define KEYQUES		8
#define BUFTXTSIZE	80
#define CURS_ROWS		25
#define CURS_COLS		80
#define YES				(0 == 0)
#define NO           (0 != 0)
#define INT32			ULONG
#define INT16			unsigned int
#define INT8			UCHAR
#define PSOSPID      0
#define PSOSEXID     1
#define PSOSDATA_P	3
#define FOREVER		0
#define DONTCARE		0
#define ZERO			0
#define ONE				1
#define FIFO			((UCHAR) (0<<7))
#define PRIORITY		((UCHAR) (1<<7))
#define MQLLIM			((UCHAR) (1<<1))
#define MQLINF			((UCHAR) (0<<1))

#if 0
#define TRUE (0 == 0)
#define FALSE (0 == 1)
#endif

#define	KC_F1		0x03b	/* code 0 */
#define	KC_F2		0x03c	/* code 0 */
#define	KC_F3		0x03d	/* code 0 */
#define	KC_F4		0x03e	/* code 0 */
#define	KC_F5		0x03f	/* code 0 */
#define	KC_F6		0x040	/* code 0 */
#define	KC_F7		0x041	/* code 0 */
#define	KC_F8		0x042	/* code 0 */
#define	KC_F9		0x043	/* code 0 */
#define	KC_F10		0x044	/* code 0 */
#define	KC_UP		0x048	/* code 0 */
#define	KC_DN		0x050	/* code 0 */
#define	KC_RT		0x04d	/* code 0 */
#define	KC_LT		0x04b	/* code 0 */
#define	KC_ESC		0x01b
#define	KC_TAB		0x009
#define	KC_SHTAB		0x00f	/* code 0 */
#define	KC_ENTER		0x00d
#define	KC_DEL		0x053	/* code 0 */
#define	KC_BCK		0x008
#define	KC_END		0x04f	/* code 0 */
#define	KC_INS		0x052	/* code 0 */
#define	KC_HOME		0x047	/* code 0 */
#define	KC_F11	0x1d
#define	KC_F12	0x2a
#define	KC_PGUP	0x49
#define	KC_PGDN	0x51

#define NUMTABS	20
#define LONGESTWORD	22

/* types for keycode passing */

typedef enum
{
	KeyAll,
	KeyOnly,
	KeyAllBut,
	KeyNone
} KEYPASS;

/* Conversion factors: rows/inch, columns/inch for IBM PC VGA */
/* fixed width characters.                                    */
#define UNITSPERINCH	300

#define	COLSPERINCH	10.0
#define	ROWSPERINCH	4.3479

/* System message. */

/*****************************************************************************
*   Major functions defined for the message ulMsgFunct field. Each task will 
*   define commands that it will execute and status that it will return. 
*   It should be possible to send commands and status in a single message, so
*   the 
******************************************************************************/


#define	usMsgError (USHORT) 0 /* This is a mistake. Designed to detect bad 
												messages. 		                        */
#define	usMsgCmd (USHORT) 1<<0 /* This message contains a command. Data has
                            further information. Buffer might be present.  */										
#define  usMsgStatus (USHORT) 1<<1  /* Status return. Value is in Data, 
                                   Buffer if required.                     */
#define  usMsgKeyData (USHORT) 1<<2	/* Keyboard data is contained in 
												usMsgWhat									 	*/
#define  usMsgMouseData (USHORT) 1<<3 /* Mouse data is contained in usMsgWhat
										 		and in MsgData.MsgDataXY					*/

#define ROOT_WIDTH	480
#define ROOT_HEIGHT	128
#define PIXL_W			1
#define PIXL_H			1
#define SYS_FONT		0
#define STACKSIZE 500

#endif

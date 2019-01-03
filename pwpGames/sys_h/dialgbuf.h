/****************************************************************************
*
*		$Workfile:   dialgbuf.h  $	- Ram structure defining Dialog Window.
*
*		$Revision:   1.35  $			$Modtime:   23 Aug 1991 15:22:52  $
*
*		$Author:   JoeM  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/dialgbuf.h_v  $
* 
*    Rev 1.35   23 Aug 1991 15:23:28   JoeM
* SIGNAL_DM 
* 
*    Rev 1.34   23 Aug 1991 08:52:16   JoeM
* prototypes 
* 
*    Rev 1.33   12 Aug 1991 18:42:12   JoeM
* prototypes 
* 
*    Rev 1.32   11 Aug 1991 15:00:18   JoeM
* prototype
* 
*    Rev 1.31   05 Aug 1991 20:32:20   JoeM
* dialogHPCall() prototype 
* 
*    Rev 1.30   01 Aug 1991 15:01:26   JoeM
* Moved Task Used Protos fm dman.h
* 
*    Rev 1.29   31 Jul 1991 19:24:18   FrankW
* John Munton's final changes
* 
*    Rev 1.28   24 Jun 1991 12:13:50   FrankW
* No change.
* 
*    Rev 1.27   25 Jun 1991 10:21:54   JohnM
* Included new flush #define for validation functions.
* 
*    Rev 1.22   14 May 1991 14:30:12   JohnM
* Incorperated Joes changes.
* 
*    Rev 1.21   14 May 1991 13:51:14   JohnM
* Incorperated hidden fields.
* 
*    Rev 1.14   01 May 1991 10:21:32   JohnM
* Now supports super and subscript.
* 
*    Rev 1.13   24 Apr 1991 08:47:20   JohnM
* Included screen_no in validation status.
* 
*    Rev 1.12   19 Apr 1991 12:17:18   JohnM
* Included two new functions - "task get literal", and "task get graphic"
* that are migration proof to allow data compression in a future release.
* 
*    Rev 1.8   08 Apr 1991 13:10:12   JohnM
* Changed window_control to screen_ni for task to determine which session
* the screen is from.
*
*
*
****************************************************************************/

#ifndef DIALOGBUFH
#define DIALOGBUFH

#ifdef SYSPC
#include <malloc.h>	/* DEFINTIONS OF  MALLOC(), AND FREE() */
#endif

#include <string.h>	/* DEFINITIONS OF STRNCMP(), STRNCPY() */

/***************************************************************************
* Validation function condition codes.
*/
enum valid_cond_code
{
	FIRST_CHECK,						/* BEFORE ITEM IS SELECTED */
	CHARACTER_CHECK,					/* CHARACTER VALUE ENTERED */
	CANCEL_CHECK,						/* CANCEL WAS SELECTED */
	SELECTED_CHECK,					/* ITEM HAS BEEN SELECTED */
	ITEM_CHECK,							/* NEXT OR PREVIOUS ITEM */
	GROUP_CHECK,						/* NEXT OR PREVIOUS GROUP */
	SCREEN_CHECK,						/* EXIT DIALOG SESSION */
	PREV_CHECK,							/* TASK SCROLL PREVIOUS DATA */
	NEXT_CHECK,							/* TASK SCROLL NEXT DATA */
	TOP_CHECK,							/* TASK SCROLL TOP DATA */
	BOTTOM_CHECK,						/* TASK SCROLL BOTTOM DATA */
	LANGUAGE_CHECK,					/* LANGUAGE CHANGED */
};
typedef enum valid_cond_code	VALID_COND_CODE;

#define	VALID_OK					0	 				
#define	VALID_ITEM				0x010000			/* GO TO ITEM WITHIN GROUP */
#define	VALID_GROUP				0x020000			/* GOTO GROUP */
#define	VALID_NEXT_ITEM		0x040000			/* GOTO NEXT ITEM */
#define	VALID_PREV_ITEM		0x080000			/* GOTO PREVIOUS ITEM */
#define	VALID_NEXT_GROUP		0x0100000		/* GOTO NEXT GROUP */
#define	VALID_PREV_GROUP		0x0200000		/* GOTO PREVIOUS GROUP */
#define	VALID_CURRENT			0x0400000	  	/* STAY IN CURRENT ITEM */
#define	VALID_EXIT_SESSION	0x0800000	  	/* EXIT DIALOG SESSION */
#define	VALID_BAD_VALUE		0x010000000		/* BAD VALUE ENTERED */
#define	VALID_FLUSH				0x020000000		/* FLUSH SCREEN */

#define	VALID_MASK				VALID_ITEM | VALID_GROUP | VALID_NEXT_ITEM |\
										VALID_PREV_ITEM | VALID_NEXT_GROUP |\
										VALID_PREV_GROUP | VALID_CURRENT | VALID_EXIT_SESSION |\
										VALID_BAD_VALUE | VALID_FLUSH

/***************************************************************************
* WINDOW SCROLL MODE
*/
enum scroll_mode
{
	NO_SCROLL,
	UP_SCROLL,
	DOWN_SCROLL,
	BOTH_SCROLL
};
typedef enum scroll_mode	SCROLL_MODE;

/****************************************************************************
* Enumeration Definitions For PSOS Dialog Manager. Legal Commands.
*
*	EDIT_DM			   Edit Current DM Session.
*	START_DM			   To Begin A Dialog Window Session For The First Time.
*	START_DM_RESP	   Same As START_DM, Use Sender's Response Command.
*	REFRESH_DM		   Request For Dialog Manager To Update/Refresh the display.
*	SUSPEND_DM		   Suspend But Don't Close Dialog Window.
*	RESTART_DM		   Restart Suspended Dialog Window.
*	END_DM			   Close Dialog Window.
*	END_FREE_DM		   Close Dialog Window and Free Memory. (This will eliminate
*					      the need to issue the dialogTDelBuf() call).
*	INPUT_DM			   Response Sent From Keyboard To Dialog Manager.
*	START_HIGH_DM     Start dialog at High Priority.
*	START_HIGH_DM_RESP Start dialog at High Priority and expect responce.
*/

#define RESP_BIT    0x100
#define NO_SV_DISP  0x200
#define SIGNAL_DM   0x300

enum request_type
{
	EDIT_DM,
	START_DM,
	REFRESH_DM,
	SUSPEND_DM,
	RESTART_DM,
	END_DM,
	END_FREE_DM,
	INPUT_DM,
	START_HIGH_DM,
	START_DM_RESP      = START_DM | RESP_BIT,
	START_HIGH_DM_RESP = START_HIGH_DM | RESP_BIT
};

/****************************************************************************
* EXTERNAL BASED APPLICATION TABLES
*/

struct extern_table
{
	struct dialog_window		**windows;				/* ARRAY OF WINDOW TEMPLATES */
	CHAR							**literal_table;		/* ARRAY OF LITERAL POINTERS */
	struct bitmap				**graphic_table;		/* ARRAY OF GRAHIC POINTERS */
	UCHAR							*hot_key_table;		/* ARRAY OF HOT KEY OFFSETS */
	SHORT							tot_windows, 			/* TOTAL WINDOW */
									tot_literals, 			/* TOTAL LITERALS */
									tot_graphics,			/* TOTAL GRAPHICS */
									tot_hot_keys;			/* TOTAL HOT KEYS */
};
typedef struct extern_table	EXTERN_TABLE;

/****************************************************************************
* INPUT STRUCTURE
*/
struct item_input
{
	CHAR							*undo_buf,				/* UNDO BUFFER */
									*buffer_pos,			/* BUFFER LOCATION */
  									*picptr;					/* CURRENT PICTURE POINTER */
	UCHAR							cursor_pos,				/* CURRENT CURSOR POSITION */
									escape_cnt,				/* COUNT OF ESCAPE SEQ. */
									length;					/* LENGTH OF INPUT FIELD */
	SYSFONTID					font;						/* FONT TYPE */
	UINT							moved : 1,				/* CURSOR HAS MOVED */
									in_super : 1,			/* IN SUPERSCRIPT */
									in_sub : 1;				/* IN SUBSCRIPT */
};
typedef struct item_input		ITEM_INPUT;

/****************************************************************************
* RAM LITERAL VALUES
*/

struct ram_lit
{
	SHORT							labeloffset,
									keyoffset;
};
typedef struct ram_lit	RAM_LIT;
/****************************************************************************
* TEXT TYPE
*/
struct text_buf
{
	union
			{
			CHAR	 				*buffer;					/* TEXT POINTER */
			UCHAR					value; 					/* NUMERIC TYPE */
			RAM_LIT				literal;					/* RAM BASED LITERAL */
			SHORT					graphic;					/* RAM BASED GRAPHIC */
			} data;

	UINT 							user_modify : 1,		/* MODIFIED BY USER */
									task_modify : 1,		/* MODIFIED BY TASK */
									skipped : 1,			/* ITEM IS SKIPPED */
									hidden : 1,				/* ITEM IS HIDDEN */
									selected : 1;			/* SELECTED OR ACTIVE */
};
typedef struct text_buf	TEXT_BUF;

/****************************************************************************
* BOOLEAN TYPE
*/
struct boolean_buf
{
	UINT 							user_modify : 1, 	  	/* MODIFIED BY USER */
									task_modify : 1, 	  	/* MODIFIED BY TASK */
									is_set : 1,		  	  	/* BOOLEAN IS SET */
									skipped : 1,	  		/* ITEM IS SKIPPED */
									hidden : 1;				/* ITEM IS HIDDEN */
};
typedef struct boolean_buf	BOOLEAN_BUF;

/****************************************************************************
* SELECT TYPE
*/
struct select_buf
{
	UINT							task_modify : 1,		/* TASK MODIFIED */
									active : 1,				/* ITEM IS SELECTED */
									skipped : 1,			/* ITEM IS SKIPPED */
									hidden : 1;				/* ITEM IS HIDDEN */
};
typedef struct select_buf	SELECT_BUF;

/****************************************************************************
* GROW BUFFER DECLARATIONS
*/
struct items_grow
{
	UCHAR					  		*grow_pool,				/* GROW BUFFERS */
									last_pool,				/* CURRENT ALLOCATED ITEM */
									last_size,				/* LAST ALLOCATED SIZE */
									base_pool;				/* BASE OF ALLOCATION POOL */
};
typedef struct items_grow	ITEMS_GROW;

/****************************************************************************
* GROUP BUFFER
*/
struct group_buf
{
	LONG			 		 		(*ProcName)(struct item_control *);
	ITEMS_GROW					*grow;
	struct item_buf			*item_buf;				/* SCREEN ITEM BUFFER */
	union
	{
 		BOOLEAN_BUF				*boolean;  	  			/* ARRAY OF BOOLEAN */
		TEXT_BUF					*text;	  	  			/* ARRAY OF TEXT */
		SELECT_BUF				*select;	  				/* ARRAY OF SELECT */
	} types;

	SHORT							curr_item,				/* CURRENT ITEM */
									currsel;					/* CURRENT SELECTION */
	UCHAR							curr_offset,			/* OFFSET FOR MULTI PAGE LISTS */
									tot_items,				/* NUMBER OF VARIABLE LIST ITEMS */
									max_items,				/* SIZE OF VARIABLE LIST BUFFER */
#ifdef GROW_ITEMS
									grow_tot,				/* FILLED GROW # OF ITEMS */
#endif
									flush_to,				/* WHERE TO STOP FLUSHING */
									select_tot;				/* TOTAL TRAVELABLE ITEMS */
	SCROLL_MODE					scroll;					/* SCROLL MODE */
	UINT	 						user_modify : 1,		/* MODIFIED BY USER */
									task_modify : 1,		/* MODIFIED BY TASK */
									skipped : 1,			/* GROUP IS SKIPPED */
									hidden : 1,				/* ITEM IS HIDDEN */
									prev_scroll : 1,		/* MORE PREVIOUS TASK DATA */
									next_scroll : 1,		/* MORE FUTURE TASK DATA */
									no_start_wrap : 1,	/* NO WRAP FROM START */
									no_end_wrap : 1,		/* NO WRAP FROM END */
									need_flush : 1,		/* END MUST BE FLUSHED */
									flush_scroll : 1;		/* SCROLL FLAGS FLUSHED */
};
typedef struct group_buf	GROUP_BUF;

/****************************************************************************
* DIALOG SESSION BUFFER
*/
struct dialog_buf
{
	ITEM_INPUT					item_input;				/* INPUT STRUCTURE */
	struct window				*win_Handle;			/* WINDOW STRUCTURE */
	LONG							(*ProcName)(struct item_control *);
	GROUP_BUF	  				*group_ptr;				/* ARRAY OF GROUPS */
	VOID							*task_ptr;				/* POINTER TO ANYTYPE FOR TASK */
	EXTERN_TABLE				*tables;					/* APPLICATION TABLES */
	struct dm_EventTable		*mouse_table;			/* MOUSE TABLE */
	SYSKEY						value;					/* KEY VALUE */
	SHORT							curr_group;		  	  	/* CURRENT GROUP */
	UCHAR							select_tot,				/* TOTAL TRAVELABLE GROUPS */
									pool_tot,				/* TOTAL BUFFER POOLS */
									dm_window_id,			/* WINDOW IDENTIFIER */
									screen_no;				/* DIALOG SCREEN NUMBER */
	CHAR							**buf_pools;			/* ARRAY OF BUFFER POOLS */
	UINT	 						user_modify : 1,	  	/* MODIFIED BY USER */
									task_modify : 1,	  	/* MODIFIED BY TASK */
									is_active : 1,	 	  	/* SCREEN IS ACTIVE */
									is_a_validate : 1,	/* CALLED BY A VALIDATE FUNCTION */
									is_suspended : 1,		/* SESSION IS SUSPENDED */
									skipped : 1,			/* SESSION IS SKIPPED */
									flush_scroll : 1;		/* SCROLL FLAGS FLUSHED */
};
typedef struct dialog_buf	DIALOG_BUF;

/****************************************************************************
* GROUP CONTROL STRUCTURE
*/
struct group_control
{
	DIALOG_BUF					*window_buf;
	struct dialog_window		*window_control;
	GROUP_BUF					*group_buf;
	struct group_info			*group_control;
};
typedef struct group_control	GROUP_CONTROL;

/***************************************************************************
* ITEM CONTROL STRUCTURE
*/
struct item_control
{
	DIALOG_BUF					*window_buf;			/* WINDOW BUFFER */
	struct dialog_window		*window_control;		/* WINDOW CONTROL */
	GROUP_BUF					*group_buf;				/* GROUP BUFFER */
	struct group_info			*group_control;		/* GROUP CONTROL */
	struct item_info			*item_control;			/* ITEM CONTROL */
	struct item_buf			*item_buf;				/* SCREEN ITEM BUFFER */
	union
	{
		BOOLEAN_BUF				*boolean_buf;	 		/* BOOLEAN BUFFER */
		TEXT_BUF					*text_buf; 				/* TEXT BUFFER */
		SELECT_BUF				*select_buf;			/* SELECTION BUFFER */
	} types;

	VALID_COND_CODE			condition;				/* EDIT CONDITION */
	UINT							current : 1;			/* ITEM IS HIGHLIGHTED */
	UCHAR							fill_size,				/* SIZE NEEDED FOR FILL */
									shift_size,				/* SCROLL SHIFT SIZE */
									move_size;				/* TASK SCROLL ITEMS ADDED */
};
typedef struct item_control	ITEM_CONTROL;

/***************************************************************************
* The structure used from a validation routine to determine the fields'
* status.
*/
struct valid_stat
{
	SHORT							next_group;
	VALID_COND_CODE			condition;
	SYSKEY						value;
	UINT							changed : 1;
	UCHAR							screen_no,
									group,
									tot_items,
									item,
									fill_size;
};
typedef struct valid_stat	VALID_STAT;

/***************************************************************************
* The structure used to determine a screens' status.
*/
struct dialog_stat
{
	VALID_COND_CODE			condition;
	UCHAR							tot_groups,
									screen_no;
	UINT							active : 1,
									changed : 1;
};
typedef struct dialog_stat	DIALOG_STAT;

/***************************************************************************
* The structure used to determine a groups' status.
*/
struct group_stat
{
	SHORT							next_group;
	UCHAR							tot_items,
									num_items,
									max_items;
	UINT 							skipped : 1,
									changed : 1,
									hidden : 1;
};
typedef struct group_stat GROUP_STAT;

/***************************************************************************
* The structure used to determine a items' status.
*/
struct item_stat
{
	UINT 							skipped : 1,
									changed : 1,
									hidden : 1;
};
typedef struct item_stat	ITEM_STAT;

/****************************************************************************
* TYPE MACROS
*/

#define IS_TEXT(x)			((x) & (RAM_PTR | RAM_BUF | BAR_GRAPH |\
										RAM_TEXT | RAM_ICON | GROW_BUF))	/* TEXT ITEM */
#define IS_BOOLEAN(x)		((x) & BOOLEAN)					/* BOOLEAN ITEM */
#define IS_SELECTION(x)		((x) & SELECT)						/* SELECT ITEM */
#define IS_LITERAL(x) 		( ! ((x) & (RAM_PTR | RAM_BUF |\
											BAR_GRAPH | GROW_BUF |\
											RAM_TEXT | RAM_ICON | BOOLEAN | SELECT)))
											/* NON BUFFERED */

#define IS_BUFFERED(x)		((x) & (RAM_BUF | GROW_BUF)) /* ALLOCATE BUFFER */
#define IS_A_GROW(x)			((x) & GROW_BUF)				/* GROW BUFFER */
#define IS_POINTER(x)		((x) & RAM_PTR)				/* NO BUFFER */
#define IS_NUMERIC(x)		((x) & BAR_GRAPH)				/* RULER TYPE */
#define IS_TABLE_TEXT(x)	((x) & RAM_TEXT)				/* RAM LITERAL */
#define IS_TABLE_GRAF(x)	((x) & RAM_ICON)				/* RAM GRAPHIC */
#define IS_ITEM_BUF(x)		(((x) & RAM_BUF) && ((x) & CURS_YES))

#define IS_LANGUAGE(x)		((x) & (RAM_TEXT | RAM_ICON |\
										ROM_TEXT | ROM_ICON))	/* LANGUAGE SPECIFIC */

#define IS_VLIST(x)			((x) & VARIABLE)				/* VARIABLE LIST SIZE */
#define IS_INPUT(x)			((x) & CURS_YES)				/* TRAVELABLE */

#define HAS_A_TRIGGER(x)	((x) & TRIGGER)					/* TRIGGER */
#define HAS_A_MOUSE(x)		((x) & (CURS_YES | TRIGGER))	/* MOUSE */
#define HAS_A_KEY(x)			((x) & (CURS_YES | TRIGGER))	/* ASSOCIATED KEY */
#define HAS_A_KEYTIP(x)		((x) & KEYTIP)						/* OUTPUT KEYTIP */

/*************************************************************************/
/********************   \DIALOG\JOHN TASK ROUTINES   *********************/

ITEM_CONTROL	 * dialogTMakeBuf (EXTERN_TABLE *, INT, INT *);
INT				dialogTGetSelect (ITEM_CONTROL *, INT),
	 				dialogTGetLiteral (EXTERN_TABLE *, INT, CHAR *, INT),
					dialogTGetGraphic (EXTERN_TABLE *, INT, struct bitmap *, INT);
CHAR				* dialogTGetBuf (ITEM_CONTROL *),
					* dialogTGetText (ITEM_CONTROL *, INT, INT, INT *),
					dialogTPutText (ITEM_CONTROL *, INT, INT, CHAR *),	
					dialogTSetSelect (ITEM_CONTROL *, INT, INT),
					dialogTSetCurrent (ITEM_CONTROL *, INT, INT),
					dialogTSetPicture (ITEM_CONTROL *, INT, INT, CHAR *),
					dialogTSetBoolean (ITEM_CONTROL *, INT, INT),
					dialogTClearBoolean (ITEM_CONTROL *, INT, INT),
					dialogTGetBoolean (ITEM_CONTROL *, INT, INT),
					dialogTDelBuf (ITEM_CONTROL *),
					dialogTStatValid (ITEM_CONTROL *, VALID_STAT *),
					dialogTPutBuf (ITEM_CONTROL *, VOID *),
					dialogTSkipGroup (ITEM_CONTROL *, INT),
					dialogTSkipItem (ITEM_CONTROL *, INT, INT),
					dialogTIncludeGroup (ITEM_CONTROL *, INT),
					dialogTIncludeItem (ITEM_CONTROL *, INT, INT),
					dialogTStatScreen (ITEM_CONTROL *, DIALOG_STAT *),
					dialogTStatGroup (ITEM_CONTROL *, INT, GROUP_STAT *),
					dialogTStatItem (ITEM_CONTROL *, INT, INT, ITEM_STAT *),
					dialogTInitGroup (ITEM_CONTROL *, INT),
					dialogTInitScreen (ITEM_CONTROL *),
					dialogTLastItem (ITEM_CONTROL *, INT, INT),
					dialogTScrollItems (ITEM_CONTROL *, INT *, INT),
					dialogTSetScroll (ITEM_CONTROL *, INT, INT, INT),
					dialogTSetWrap (ITEM_CONTROL *, INT, INT, INT),
	 				dialogTSetTables (ITEM_CONTROL *, EXTERN_TABLE *),
					dialogTSetScreenFunc (ITEM_CONTROL *, LONG (*)(ITEM_CONTROL *)),
					dialogTSetGroupFunc (ITEM_CONTROL *, INT, LONG (*)(ITEM_CONTROL *)),
					dialogTSetBar (ITEM_CONTROL *, INT, INT, INT),
					dialogTClearBar (ITEM_CONTROL *, INT, INT),
					dialogTFillBar (ITEM_CONTROL *, INT, INT),
					dialogTCreateMouse (ITEM_CONTROL *),
					dialogTDeleteMouse (ITEM_CONTROL *),
					dialogTSetLiteral (ITEM_CONTROL *, INT, INT, SHORT, SHORT),
					dialogTSetGraphic (ITEM_CONTROL *, INT, INT, SHORT),
					dialogTDelGrow (ITEM_CONTROL *, INT),
					dialogTSkipScreen (ITEM_CONTROL *),
					dialogTIncludeScreen (ITEM_CONTROL *),
					dialogTHideItem (ITEM_CONTROL *, INT, INT),
					dialogTShowItem (ITEM_CONTROL *, INT, INT),
					dialogTHideGroup (ITEM_CONTROL *, INT),
					dialogTShowGroup (ITEM_CONTROL *, INT);


/*************************************************************************/
/********************   \DIALOG\JOE TASK ROUTINES   *********************/

/*****   Module - dialgman.c   *****/
UCHAR          dialogHPCall (USHORT, ITEM_CONTROL *, USHORT);
UCHAR				dialogTCall  (USHORT, ITEM_CONTROL *, USHORT);
INT  	  			dialogHideItem (ITEM_CONTROL *, INT, INT);
INT    			dialogShowItem (ITEM_CONTROL *, INT, INT);
void				dialogSPutText (ITEM_CONTROL *, INT, INT, CHAR *);

/*****   Module - dmanutil.c   *****/
USHORT			dialogQueues (ULONG);
struct window *dialogDGetWindow (EXTERN_TABLE *, INT);
void           dialogStartKeys (void);
UCHAR           dialogChangeInputFont (ITEM_CONTROL *, INT, INT, SYSFONTID);
UCHAR           dialogRestreInputFont (ITEM_CONTROL *, INT, INT);

/***************************************************************************
* The dialog declarations
*/
CHAR 				dialogDGetAllMod (ITEM_CONTROL *, ITEM_CONTROL *),
	 				dialogDGetScrollMod (ITEM_CONTROL *, GROUP_CONTROL *, CHAR),
	 				dialogDGetAllScroll (ITEM_CONTROL *, GROUP_CONTROL *, CHAR),
					dialogDGetAllItem (ITEM_CONTROL *, ITEM_CONTROL *),
					dialogDFirstItem (ITEM_CONTROL *),
					dialogDNextItem (ITEM_CONTROL *),
					dialogDPrevItem (ITEM_CONTROL *),
					dialogDFirstPage (ITEM_CONTROL *, GROUP_CONTROL *),
					dialogDLastPage (ITEM_CONTROL *, GROUP_CONTROL *),
					dialogDNextGroup (ITEM_CONTROL *),
					dialogDPrevGroup (ITEM_CONTROL *),
					dialogDGotoGroup (ITEM_CONTROL *, INT),
					dialogDGotoItem (ITEM_CONTROL *, INT, INT),
					dialogDNextScroll (ITEM_CONTROL *, GROUP_CONTROL *, INT),					
					dialogDPrevScroll (ITEM_CONTROL *, GROUP_CONTROL *, INT),
					dialogDShiftItems (ITEM_CONTROL *),
					dialogDGetItemId (ITEM_CONTROL *, INT *, INT *),
					dialogDCreateLanguage (ITEM_CONTROL *),
					dialogDGetNewLanguage (ITEM_CONTROL *, ITEM_CONTROL *),
					dialogDSwapLanguage (ITEM_CONTROL *);
UCHAR				dialogDGetHotKey (EXTERN_TABLE *, SHORT);
struct bitmap	*dialogDGetGraphic (EXTERN_TABLE *, SHORT);
CHAR 				*dialogDGetLiteral (EXTERN_TABLE *, SHORT);
struct dialog_window	*dialogDGetScreen (EXTERN_TABLE *, INT);
/****************************************************************************
* The Internal declarations
*/
CHAR 				dialogIGetGroup (ITEM_CONTROL *,	INT *, GROUP_CONTROL *),
					dialogIGetDialItem (ITEM_CONTROL *, INT *, INT *, ITEM_CONTROL *),
					dialogIGetTaskItem (ITEM_CONTROL *, INT *, INT *, ITEM_CONTROL *),
					dialogIGetInternalItem (ITEM_CONTROL *, INT, INT, ITEM_CONTROL *),
					dialogIInitGroup (ITEM_CONTROL *, struct group_info *, GROUP_BUF *),
					dialogIGroupGrow (struct group_info *, GROUP_BUF *, INT, USHORT),
					dialogIItemIsHidden (ITEM_CONTROL *),
					dialogIItemIsSkipped (ITEM_CONTROL *);
VOID				dialogIDelGrow (GROUP_BUF *),
					dialogIScrollFlag (ITEM_CONTROL *, struct group_info *, GROUP_BUF *, CHAR),
					dialogIFlushGroup (GROUP_CONTROL	*);

/****************************************************************************
* The string function defintions
*/

#ifndef MEMSET
#define MEMSET(x,y,z)		memset ((x), (y), (z))
#endif

#ifndef MEMCCPY
#define MEMCCPY(w,x,y,z)	memccpy ((w), (x), (y), (z))
#endif

#ifndef MEMCPY
#define MEMCPY(x,y,z)		memcpy ((x), (y), (z))
#endif

#ifndef STRNCMP
#define STRNCMP(x,y,z)		strncmp ((x), (y), (z))
#endif

#ifndef STRNCPY
#define STRNCPY(x,y,z)		strncpy ((x), (y), (z))
#endif

#ifndef STRCMP
#define STRCMP(x,y)			strcmp ((x), (y))
#endif

#ifndef STRCPY
#define STRCPY(x,y)			strcpy ((x), (y))
#endif

#ifndef STRLEN
#define STRLEN(x)				strlen ((x))
#endif

#ifndef STRCHR
#define STRCHR(x,y)			strchr ((x), (y))
#endif

#ifndef MALLOC
#define MALLOC(x)				malloc ((x))
#endif

#ifndef FREE
#define FREE(x)				free ((x))
#endif

/****************************************************************************
* The buffer STRATEGEY
*/

#define ALIGN(x)	((x) % sizeof (char *) ? \
						(x) + sizeof (char *) - ((x) % sizeof (char *)) : \
						(x))
#define IS_ALIGNED
#ifndef POOL_SIZE
#define POOL_SIZE				256
#endif

#define ESCAPE_SEQ_SIZE		2
#define MAX_ESCAPE_LIMIT	4

#endif

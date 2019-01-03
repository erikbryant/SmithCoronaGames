#ifndef WINDH
#define WINDH
/*************************************************************************
*  Definitions of rectangular regions. This is to impose a selection 
*  structure on a window. Each of the regions is to correspond to a certain
*  associated action by the user. Regions may be entered by the mouse cursor
*  or by certain designated keystrokes. Once the mouse is in a region, certain
*  other actions, such as mouse button or enter key depressions cause events
*  to be sent to the owner of the window, notifying them of the selection. 
*
*  Each of the rectangular regions is specified using window-relative coordinates
*  The underlying window is assumed to save any background necessary. 
*  The character of the bit image, if any, contained in the region is the 
*  responsibility of the window owner. 
*  Only a single set of regions is assumed active at any time. This is to 
*  avoid problems with menus overlapping other menus. 
*  When an application displays a menu, it registers the fact with the mouse
*  task, which then assumes responsibility for tracking the entries, exits 
*  and event generation associated with the region. When an application
*  is finished with the menu, it deregisters the menu from the mouse task and
*  any previous menu is restored.
***************************************************************************/


#if 0

/* 4 mar 91 -- rwh -- */
/* these enumerated types are to be redefined to be USHORT */

typedef enum 
{
	Nocolor = 0, Black, White = 7, Gray
} SYSCOLOR;

typedef enum
{
	Graphtext, Chartext, Graphics, Root
} SYSWINTYPE;

typedef enum
{
	Nothing=0, Window, Bitmap
} OBJECT;

#else

typedef USHORT OBJECT;

/* 4 mar 91 -- rwh -- */
/* Defined values replacing the enumerated types */

/* Tags for colors */
#define Nocolor	0
#define Black		1
#define White		7
#define Gray		2
#define CoronaArt	3

/* Tags for window types. */

#define Graphtext	0
#define Chartext	1
#define Graphics	2
#define Root		3
#define Hardware	(1<<((sizeof(SYSWINTYPE)*8) - 1))

/* Tags for object types. */

#define Nothing	0
#define Window		1
#define Bitmap		2

#endif

/* Bitmap structure */

typedef struct bitmap
{
	SHORT iwidth,iheight;			/* Dimensions are in pixels */
	SHORT ioriginx;
	SHORT ioriginy; 
	DATAUNIT *bits;
} *BITMAP;

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* Changing this => changing c_elem (contained in bitrev.a68)*/
/* Also change the root window in win1.c and possibly winChildCreate */
/* in the same place.  */

typedef struct dispsurf
{
	struct dlink dlink;			/* pointers to left and right offsets */
	BITUNIT *dispsurf;			/* pointer to the array in question   */
	SHORT	pitch;					/* pitch of the display surface.      */
	BITUNIT *shftrgn;				/* region of the display surface subject to shift */
	SHORT	width;					/* Width of the displayable region    */
	SHORT index;					/* first byte of shiftregion from left of dispsurf */
	SHORT lused;					/* amount of the left region now used */
	SHORT rused;					/* amount of the right region now used */
} *DISPSURF;
/************************************************************************/
/************************************************************************/
/************************************************************************/

typedef struct region
{
	SHORT x,y,dx,dy;				/* location of a rectangular region. Movement
											of the mouse into this area causes an event 
											to be sent to the owner of the region.   */
	OBJECT	objtype;				/*	What is enclosed by the region			*/
	union
	{

		BITMAP bitmap;				/* Image in the region, if any. This may be 
											null, which implies no image.            */
		struct rect *window;				/* The region may also contain a window		*/
	} object;

} *REGION;

typedef struct base	/* baseline structure */
{
	SHORT tx0;				/* x-origin of baseline, relative to enclosing window */
	SHORT ty;				/* y-position */
	SHORT twid;				/* width of baseline */
	SHORT tabove;			/* largest lines-above-baseline for all fonts on this baseline */
	SHORT tbelow;			/* largest lines-below-baseline */
	SHORT tlead;			/* largest leading */
} *BASE;

typedef struct rect
{
	/* Ownership and bookkeeping                                           */
	struct dlink dlink; 		/* Chain field for rectangles. 0 => Closed     */

	/* Window location and dimensions                                      */
	SHORT			x,y;			/* Upper left corner                           */
	SHORT			dx,dy;		/* number of rows/columns on display           */
	SHORT			width;		/* Width of the displayable surface            */

	/* Appearence parameters: background & foreground colors               */
	/* Three colors are defined: white, black and grey                     */
	SYSCOLOR		bgc, fgc, bdc;/* Background/foreground/border colors       */
	SHORT			bdw;			/* Border width                                */
} *RECT;

typedef struct window
{
	/* Ownership and bookkeeping                                           */
	struct rect rect;			/* Chain field for rectangles. 0 => Closed     */
	struct dlink	children;	/* Chain of child windows                      */
	ULONG			owner;		/* Owning task input queue                     */

	/* Display surface parameters.                                         */
	struct	dispsurf  dispsurf;
	BITUNIT		*saved;		/* Saved background stored here                */

	struct 						/* font structure for windows */
	{
		SYSFONTID	id;		/* font-id */
		SHORT			above;	/* lines-above-baseline = font_height-lns_below_base */
		SHORT			below;	/* lines-below-baseline = lns_below_base */
		SHORT			lead;		/* leading */
		SHORT			sp_wid;	/* width of a space-character */
	} font;

	USHORT attr;				/* attributes, in editor-format, eg BOLD_ACTIVE */

	SYSWINTYPE	type;			/* Window type: eg Chartext, Hardware          */

	/* Cursor locations                                                    */
	struct base base;			/* current baseline                            */
	SHORT			tx;			/* Text cursor x-position on baseline          */
	struct bitmap cursorpat;/* Bitmap of the cursor pattern                */
	SHORT			blink;		/* Blink interval for text cursor              */
} *WINDOW;

typedef enum /* permissible operations on some rectangle w/in a window */
{
   Compl, Fill
} RECTOP;

/* Function definition for bitmap masking functions. */

typedef  BITUNIT (*BITOPFN)( BITUNIT, BITUNIT );

typedef struct paramblk
{
	/* The first four elements, word, pitch and row, col must be preserved in 
	   this structure in the order given. The function c_elem, written in 
		assembler, depends upon this ordering. If the internal representation
  		of this structure changes, c_elem must also change.                   */
	/* 9 may 91. This is added to allow extended display surfaces.           */

	struct dispsurf dispsurf;
	SHORT   row,col;
	/* This entry is added to save a parameter push in do_row in win0.c */
	BITOPFN fn;
	BITUNIT *nextch;
	SHORT index;
} *PARAMBLK;

/* 4 july 91. Added a reference to rootwindow. */

#ifdef VARSTAB
    #define rootwindow (*(struct window *) vtab[VT_rootwindow])
#else
    struct window rootwindow;
#endif

#define DisplaySurface (&rootwindow)

#endif


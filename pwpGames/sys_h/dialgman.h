/****************************************************************************
*
*		$Workfile:   dialgman.h  $	- Rom structure defining Dialog Window.
*
*		$Revision:   1.15  $			$Modtime:   16 Jun 1991 08:41:00  $
*
*		$Author:   JohnM  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/dialgman.h_v  $
* 
*    Rev 1.15   20 Jun 1991 09:07:06   JohnM
* Inbedded picture capabilities.
* 
*    Rev 1.9   01 May 1991 10:20:34   JohnM
* Now defines the input attribute of escape.
* 
*    Rev 1.8   25 Apr 1991 10:32:08   JohnM
* Reordered attributes.
* 
*    Rev 1.7   22 Apr 1991 17:23:22   JoeM
* Added IJUST define == 0x1000
* 
* 
*    Rev 1.6   18 Apr 1991 14:08:34   JohnM
* Included new mouse regions.
* 
*    Rev 1.5   08 Apr 1991 10:38:48   JohnM
* Changed "cursor_pos" to SHORT type instead of a USHORT type.
*
*
*
****************************************************************************/

#ifndef DIALOGROMH
#define DIALOGROMH

/***************************************************************************
* FONT SELECTIONS 
*/

#define  COURIER10		0
#define  COURIER12		1
#define  COURIER15		2
#define  HELVETICA8		3
#define  HELVETICA10		4
#define  HELVETICA12		5

/****************************************************************************
* DATA TYPE MASKS 
*/

#define RAM_PTR				0x00001
#define ROM_TEXT				0x00002
#define RAM_BUF				0x00004
#define ROM_ICON				0x00008
#define RAM_ICON				0x00010
#define RAM_TEXT				0x00020
#define GROW_BUF				0x00040

#define SELECT					0x00100
#define BOOLEAN				0x00200
#define INPUT					0x00400

#define BAR_GRAPH				0x01000
#define CURS_NO				0x00000
#define CURS_YES				0x02000
#define TRIGGER				0x04000
#define FIXED					0x00000
#define VARIABLE				0x08000

typedef USHORT					FIELD_TYPES;

/****************************************************************************
* INPUT TYPE MASKS 
*/

#define NO_INPUT				0x0000
#define NUMERIC_INPUT		0x0001
#define ALPHA_INPUT			0x0002
#define ESCAPE_INPUT			0x0004

typedef UCHAR					INPUT_TYPES;

/****************************************************************************
* OUTPUT TYPE MASKS 
*/

#define NJUST					0x00000	/* NO OUTPUT FORMATING */
#define LJUST					0x00001	/* LEFT JUSTIFY OUTPUT */
#define RJUST					0x00002	/* RIGHT JUSTIFY OUTPUT */
#define CJUST					0x00004	/* CENTER JUSTIFY OUTPUT */
#define IJUST					0x00008	/* X,Y Coords Imply Input Region */

#define KEYTIP					0x00010
#define CHECKBOX				0x00020
#define BULLET					0x00040
#define ICON					0x00080
#define CHAR_STR				0x00100
#define U_ARROW				0x00200
#define D_ARROW				0x00400

#define PREFIX					0x00800
#define SUFFIX					0x01000
#define NO_SUFFIX				0x00000

typedef USHORT					OUTPUT_TYPES;

/***************************************************************************
* SCREEN POSITION DESCRIPTION
*/

struct fld_position
{
	SYSCOORD					x,					/* HOME COL IN PIXELS */
								y,					/* HOME ROW IN PIXELS */
								width,			/* WIDTH IN PIXELS */
								height;			/* HEIGHT IN PIXELS */
};
typedef struct fld_position	FLD_POSITION;

/***************************************************************************
* DIALOG BUFFER MOUSE REGION
*/

struct item_buf
	{
	FLD_POSITION	position;						/* COMPUTED FIELD POSITION */
	CHAR				*picture;						/* FIELDS PICTURE IF ANY */
	};
typedef struct item_buf	ITEM_BUF;

/***************************************************************************
* LITERAL VALUES
*/

struct list_info
{
	SHORT						list_table_val,		/* equate of ROM string */
								key_table_offset;		/* offset into literal for "HOT" key */ 
};
typedef struct list_info	LIST_INFO;


/****************************************************************************
* ITEM DESCRIPTION 
*/

struct item_info
{
	FLD_POSITION			position;
	LIST_INFO				list;
	SYSKEY					key_value;		/* HOT KEY VALUE */
	SYSFONTID				font;				/* FONT TYPE */
	SHORT						cursor_pos;		/* CURSOR POSITION IN CHARACTERS */
	UCHAR						length;			/* LENGTH OF INPUT REGION */
};
typedef struct item_info	ITEM_INFO;


/****************************************************************************
* GROUPS DESCRIPTION 
*/

struct group_info
{
	ITEM_INFO				*items;			/* ARRAY OF ITEMS */
	SHORT						next_link;		/* NEXT LINK (-1 IF NO NEXT) */
	FIELD_TYPES			 	field_type;		/* TYPE OF FIELD */
	OUTPUT_TYPES			output_attrib;	/* OUTPUT ATTRBUTE */
	INPUT_TYPES				input_attrib;	/* INPUT ATTRIBUTE */
	UCHAR						num_items,		/* NUMBER OF ITEMS */
								scroll_box;		/* BOX NUMBER TO DISPLAY SCROLL */
};
typedef struct group_info	GROUP_INFO;



/********************************************************************
- description: struct main_window -

Defines the dimensions and attributes of the main dialog window.
********************************************************************/
struct main_window
{
	FLD_POSITION			position;				/* COORDINATES OF WINDOW */
   SYSCOORD					border_width;			/* 0 = no boarder */
	SYSCOLOR					bd_color,   			/* boarder color */
								bg_color,  				/* background color */
								fg_color;	   		/* foreground color */
	SYSWINTYPE				type;						/* Window type, Graphic or Text */
};
typedef struct main_window	MAIN_WINDOW;


/********************************************************************
- description: struct dialog_window -

Will contain information corresponding to an entire dialog window to
be displayed and manipulated as a user INTerface.

	MAIN_WINDOW main_win 
							 - see above, main_window.
											
	GROUP_INFO   *groups
							 - see above, group_info.

	UCHAR    num_groups
							 - the total number of field groups within the window.
	UCHAR    num_boxes
							 - the total number of boxes within the window.

********************************************************************/
struct dialog_window  
{
	MAIN_WINDOW				main_win,
								*box;    
	GROUP_INFO				*groups;
	UCHAR  			 	 	num_boxes,
		   	 				num_groups;
};
typedef struct dialog_window	DIALOG_WINDOW;

#endif

#ifndef	KEYTASKH
#define 	KEYTASKH

/**********************************************************************
event_type will let DM know how to interpret the response from the
keyboard manager.

NOTE: There can be two cursor states.

		1. Where the mouse cursor is at any given time.

		2. Where the input cursor is. The input region is described as the
		   region where the mouse cursor last clicked upon. This cursor is
			active for keyboard input.

**********************************************************************/

#if 0

enum event_type
{
	EXECUTE,				/* Goto This region and perform the defined action */
	GOTO, 				/* Goto Group and wait for next instruction */
	KEYCODE,				/* Process Keycode */
	KEYBUFFER,			/* KEY VALUE */
	MOUSECODE			/* A MOUSE CODE */
};
typedef enum event_type EVENT_TYPE;

#endif


/**********************************************************************
acceptable_keys define all acceptable values to be returned to DM
from keyboard manager.
**********************************************************************/

enum actions
{
	HALT_KEYS,					/* Halt sending keys until further notice */
	SEND_KEYS					/* Begin sending keys until further notice */
};
typedef enum actions ACTIONS;

/**********************************************************************
acceptable_keys define all acceptable values to be returned to DM
from keyboard manager.
**********************************************************************/

enum priority_type
{
	LOW_PRIO,
	MED_PRIO,
	HIGH_PRIO
};
typedef enum priority_type PRIORITY_TYPE;

/******************************************************************************
Event Template is the description of one defined region in the entire window.
Also defined are the acceptable keys for that particular region. 
******************************************************************************/

struct dm_EventTemplate
{
	struct fld_position *event_region;	 	/* Values checked by Keyboard Manager */
	SYSKEY				  select_key;			/* Values checked by Keyboard Manager */
	UCHAR					  group,					/* Group Number  */
				   		  item;					/* Item Number   */
	ACTIONS				  action;				/* Keyboard Manager's action to be taken */	
};
typedef struct dm_EventTemplate DM_EVENT_TEMPLATE;


/******************************************************************************
Event Table is the structure passed to the Keyboard Manager. It defines the
list of all regions of interest in a windows coordinate system.
******************************************************************************/

struct dm_EventTable
{
	struct fld_position *trap_region;		/* Region in which mouse is confined */
	DM_EVENT_TEMPLATE   *template;			/* Pointer to template list */
	UCHAR               template_count,		/* Total # of Template Regions */
							  dm_window_id;		/* DIALOGS ID */
	UINT					  trap : 1,				/* TRAP OR NOT */
							  advance_keys : 1,	/* SEND ADVANCE KEYS */
							  alpha_keys : 1,		/* SEND ALPHA KEYS */
							  numeric_keys : 1,	/* SEND NUMERIC KEYS */
							  mouse_clicks : 1,	/* SEND MOUSE CLICKS */
							  select_key : 1,		/* SEND SELECT KEY VALUES */
							  code_key : 1,		/* SEND CODE KEY VALUES */
							  wp_keys : 1;			/* EDITOR KEYS */
	PRIORITY_TYPE       priority;
};
typedef struct dm_EventTable DM_EVENT_TABLE;

/****************************************************************************
* MOUSE RETURN CODE
*/

struct dm_mouse_coord
{
	SYSCOORD			y,				/* MOUSE COL COORDINATE */
						x;				/* MOUSE ROW COORDINATE */
};
typedef struct dm_mouse_coord DM_MOUSE_COORD;


#if 0

/*****   Event Response structure is filled and passed back to the DM
******   from Keyboard Manager with pointer to a region of interest.
********************************************************************/

struct dm_EventResponse
{
	union
			{
			SYSKEY				keycode;	  			/* Keycode Value passed from HSSB Manager */
			DM_EVENT_TEMPLATE *template_ptr; 	/* Event Template pointer To process, this value */
			DM_MOUSE_COORD		mouse_coord;		/* MOUSE COORDINATES */
			} value;										/*	corresponds to the coordinates entered by mouse */

	EVENT_TYPE					dm_event;
	UCHAR							dm_window_id;
};
typedef struct dm_EventResponse DM_EVENT_RESPONSE;

#endif


#endif

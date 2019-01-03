/*****************************************************************************
**
**  Header Name:
**      helplib.h
**
**  Description:
**      This module contains the constant, macro, and type definitions used
**      by the help library routines.
**
**  Notes:
**      None
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**	    05/20/92    Initial version
**
*****************************************************************************/

#ifndef helplib_h
#define helplib_h

#include "sc_def.h"                /* Miscellaneous type definitions */
#include "fontlib.h"               /* Font definitions */

/*
** Limits on the size of the data.
*/
#define MAX_TEXT_LEN              200
#define MAX_TOPIC_LEN              30
#define MAX_APP_LEN     MAX_TOPIC_LEN
#define MAX_ENTRIES                 7

/*
** Status values.
*/
#define HELP_EXIT           1
#define HELP_SUCCESS        0
#define HELP_INVALID_TOPIC -1
#define HELP_ERROR         -3

/*
** The help text consists of TOPICS that have...
*/
typedef struct help_topic {
   char  topic_name[MAX_TOPIC_LEN]; /* Name of topic that pages concern */
   char  topic_char;                /* Significant character in topic_name */
   UINT1 first_page;                /* First page for this topic */
   UINT1 page_count;                /* Number of pages for this topic */
} HELP_TOPIC;

/*
** ...some PAGES associated with them that each have...
*/
typedef struct help_page {
   UINT1 first_record;              /* First record on this page */
   UINT1 record_count;              /* Number of records on this page */
} HELP_PAGE;

/*
** ...some RECORDS associated with them.
*/
typedef struct help_record {
   UINT1 font_id;                   /* Font to print this line in */
   UINT1 space_width;               /* Size of each space */
   INT2  tab_width;                 /* Width of tab stops */
   INT1  kerning;                   /* Space to put between each character */
   INT1  leading;                   /* Space between lines */
   UINT1 underline_thickness;       /* Thickness of underline */
   INT1  underline_offset;          /* Distance of underline from baseline */
   char  *text;                     /* Text of the record */
} HELP_RECORD;

/*
** The help structure.
*/
typedef struct help_info {
   char  app_name[MAX_APP_LEN];     /* Name of calling application */
   char  font[MAX_FONT_NAME_LEN];   /* Font to display menus in */
   UINT1 topic_count;               /* Number of topics */
   UINT1 page_count;                /* Total number of pages of help */
   INT2  record_count;              /* Total number of records */
   HELP_TOPIC  *topics;             /* Pointer to the topics */
   HELP_PAGE   *pages;              /* Pointer to the pages */
   HELP_RECORD *records;            /* Pointer to the records */
} HELP_INFO;

/*
** Define the function prototype for each of the exported routines
*/
INT1 help_display (HELP_INFO *game_help);

#endif

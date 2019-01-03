/*****************************************************************************
**
**  Header Name:
**      fontlib.h
**
**  Description:
**      This module contains the constant, macro, and type definitions used
**      by the font library routines.
**
**  Notes:
**      none
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      05/20/92    Initial version
**
*****************************************************************************/

#ifndef fontlib_h
#define fontlib_h
#include "sc_def.h"    /* Miscellaneous definitions */
#include "gr.h"        /* Needed for color definitions */


/*
** Define status constants used by the font library
*/
#define FONT_SUCCESS             0      /* Code for successful completion */
#define FONT_INVALID_NAME       -1      /* Code for invalid font name */
#define FONT_INVALID_COORD      -2      /* Code for invalid coordinate */
#define FONT_INVALID_CHAR       -3      /* Code for invalid character */
#define FONT_ERROR              -4      /* Code for internal error */

/*
** Define the bit masks for the font styles
*/
#define FONT_STYLE_NORMAL        0          /* Bit mask for normal */
#define FONT_STYLE_BOLD          1 << 0     /* Bit mask for bold */
#define FONT_STYLE_UNDERLINE     1 << 1     /* Bit mask for underlining */
#define FONT_STYLE_ITALIC        1 << 2     /* Bit mask for italics */
#define FONT_STYLE_SUPERSCRIPT   1 << 3     /* Bit mask for superscripts */
#define FONT_STYLE_SUBSCRIPT     1 << 4     /* Bit mask for subscripts */
#define FONT_STYLE_RESERVED_3    1 << 5     /* Bit mask -reserved- */
#define FONT_STYLE_RESERVED_2    1 << 6     /* Bit mask -reserved- */
#define FONT_STYLE_RESERVED_1    1 << 7     /* Bit mask -reserved- */

/*
** Define bounds for font data files
*/
#define MAX_FONT_NAME_LEN (20 + 1)   /* Maximum length of a font name */
#define MAX_CHARS         200        /* Maximum symbols in a font */

/*
** Define macros for determining the size of the screen
*/
#define SCREEN_MAX_X (gr_scr_width () - 1)
#define SCREEN_MIN_X 0
#define SCREEN_MAX_Y (gr_scr_height () - 1)
#define SCREEN_MIN_Y 0

/*
** Define a macro to toggle selected bits given a mask.
** If the bit is on then turn it off, else turn it on.
**
**    b = byte
**    m = mask
**
** Usage:
**
**    TOGGLE_BIT (data_byte, mask);
*/
#define TOGGLE_BIT(b,m) (b ^= m)

/*
** Define a macro that will assign a pointer into the array of bytes
** that define the characters.
*/
#define RELATIVE_CHAR(a) ((a) - Font_table[Font_id]->first_char) 
#define INDEX(b) (BYTE_SWAP (Font_table[Font_id]->index[RELATIVE_CHAR(b)]))
#define CHAR_DATA(c) (CHAR_HEADER *) &(Font_data[Font_id][INDEX(c)])

/*
** Define types used by the font library
*/

/*
** Font attributes data structure.  All measurements are in pixels.
*/
typedef struct font_attr {
   UINT1 height;                /* height of font */
   INT1  space_width;           /* width of space character */
   INT1  kerning;               /* kerning space */
   INT1  leading;               /* leading space */
   UINT1 style;                 /* style of font */
   UINT1 color;                 /* greyscale color for text display */
   UINT1 underline_thickness;   /* thickness of underline */
   INT1  underline_offset;      /* offset from baseline */
   INT1  italic_offset;         /* X difference between top and bottom */
   UCHAR first_char;            /* first printable character */
   UCHAR last_char;             /* last printable character */
   UINT1 max_ascender;          /* How high is highest character */
   UINT1 max_descender;         /* How low is lowest character */
   INT2  tab_width;             /* width of tab stops */
} FONT_ATTR;

/*
** Coordinates (in pixels) structure
*/
typedef struct font_coord {
   INT2 x;    /* X value of coordinate */
   INT2 y;    /* Y value of coordinate */
} FONT_COORD;


/*
** Header for font file.  The data in the font file had better
** follow this format!
*/
typedef struct font_header {
   UCHAR name[MAX_FONT_NAME_LEN];    /* Textual name of the font */
   INT1  height;                     /* Height of the font */
   INT1  space_width;                /* Width of a space in the font */
   INT1  kerning;                    /* Kerning of the font */
   INT1  leading;                    /* Leading of the font */
   UCHAR first_char;                 /* First printable character */
   UCHAR last_char;                  /* Last printable character */
   UINT1 max_ascender;               /* How high is highest character */
   UINT1 max_descender;              /* How low is lowest character */
   UINT1 alignment;                  /* There must be an even number of bytes */
   INT2  index[MAX_CHARS];           /* Offsets of each character's data */
} FONT_HEADER;

/*
** Header for each character
*/
typedef struct char_header {
   INT1  offset;                    /* Offset from baseline */
   UINT1 width;                     /* Width of character */
   UINT1 height;                    /* Height of character */
   UINT1 data[1];                   /* Bitmap */
} CHAR_HEADER;

/*
** Define the function prototypes for each of the exported routines
*/
INT1  font_select (char *fontname);
INT1  font_get_attributes (FONT_ATTR *current_attr);
INT1  font_set_attributes (FONT_ATTR *new_attr);
INT1  font_reset_attributes (FONT_ATTR *new_attr, FONT_ATTR *old_attr);
INT1  font_string_size (char *text, INT2 *length, 
                        INT2 *ascender, INT2 *descender);
void  font_get_pos (FONT_COORD *current_pos);
INT1  font_set_pos (FONT_COORD *new_location);
INT1  font_printf (char *text);
INT1  font_printf_centered (char *text, INT2 x1, INT2 y1, INT2 x2, INT2 y2);
INT1  font_putc (UCHAR character);
UINT1 character_dimensions (char character, INT2 *width,
                            INT2 *ascender, INT2 *descender);

#endif

/*****************************************************************************
**
**  Module Name:
**      fontlib
**  
**  Description:
**      This module contains the functions which implement the font library.
**
**  Exported Units:
**      font_select
**      font_get_attributes
**      font_set_attributes
**      font_reset_attributes
**      font_string_size
**      font_get_pos
**      font_set_pos
**      font_printf
**      font_printf_centered
**      font_printf_centered_each
**      font_putc
**      character_dimensions
**
**  Local Units:
**      
**  Exported Data:
**      none
**
**  Notes:
**
**  Change Log:
**      Date            Description of Change
**      ----            ---------------------
**      04/01/92        Version 1.0 
**
*****************************************************************************/


/*
** Included files for module.
*/
#include "fontlib.h"                /* Constants, macros, types */

/*
** Global data within module.
*/

/*
** Load in all of the available font headers.  For each font 
** that needs to be loaded there should be the following:
**
**    static UINT1 font_X[] = {
**       #include "font-name.HDR"
**    };
**
** Where X is a unique number.
**
*/
#ifdef LCD
   static UINT1 font_0[] = {
      #include "HELV_12.HDR"
   };

   static UINT1 font_1[] = {
      #include "HELV_16.HDR"
   };

   static UINT1 font_2[] = {
      #include "HELV_20.HDR"
   };

   static UINT1 font_3[] = {
      #include "HELV_24.HDR"
   };
/*
** Array of font file headers.  For each font that needs to be loaded
** there should be a line reading:
**
**    (FONT_HEADER *) font_X,
**
** Where X is the unique number assigned to the particular font.
*/
static FONT_HEADER *Font_table[]= {
   (FONT_HEADER *) font_0,
   (FONT_HEADER *) font_1,
   (FONT_HEADER *) font_2,
   (FONT_HEADER *) font_3,
   (FONT_HEADER *) 0
};
#endif
#ifdef CRT
   static UINT1 font_0[] = {
      #include "HELV_20.HDR"
   };

   static UINT1 font_1[] = {
      #include "HELV_24.HDR"
   };

   static UINT1 font_2[] = {
      #include "HELV_30.HDR"
   };

/*
   static UINT1 font_3[] = {
      #include "HELV_37.HDR"
   };
*/
/*
** Array of font file headers.  For each font that needs to be loaded
** there should be a line reading:
**
**    (FONT_HEADER *) font_X,
**
** Where X is the unique number assigned to the particular font.
*/
static FONT_HEADER *Font_table[]= {
   (FONT_HEADER *) font_0,
   (FONT_HEADER *) font_1,
   (FONT_HEADER *) font_2,
/*
   (FONT_HEADER *) font_3,
*/
   (FONT_HEADER *) 0
};
#endif


/*
** Load in all of the available font data.  For each font 
** that needs to be loaded there should be the following:
**
**    static UINT1 font_data_X[] = {
**       #include "font-name.FNT"
**    };
**
** Where X is a unique number.
**
*/
#ifdef LCD
   static UINT1 font_data_0[] = {
      #include "HELV_12.FNT"
   };

   static UINT1 font_data_1[] = {
      #include "HELV_16.FNT"
   };

   static UINT1 font_data_2[] = {
      #include "HELV_20.FNT"
   };

   static UINT1 font_data_3[] = {
      #include "HELV_24.FNT"
   };
/*
** Array of font file data.  For each font that needs to be loaded
** there should be a line reading:
**
**    (CHAR_HEADER *) font_X,
**
** Where X is the unique number assigned to the particular font.
*/
static UINT1 *Font_data[]= {
   font_data_0,
   font_data_1,
   font_data_2,
   font_data_3,
   0
};
#endif
#ifdef CRT
   static UINT1 font_data_0[] = {
      #include "HELV_20.FNT"
   };

   static UINT1 font_data_1[] = {
      #include "HELV_24.FNT"
   };

   static UINT1 font_data_2[] = {
      #include "HELV_30.FNT"
   };

/*
   static UINT1 font_data_3[] = {
      #include "HELV_37.FNT"
   };
*/
/*
** Array of font file data.  For each font that needs to be loaded
** there should be a line reading:
**
**    (CHAR_HEADER *) font_X,
**
** Where X is the unique number assigned to the particular font.
*/
static UINT1 *Font_data[]= {
   font_data_0,
   font_data_1,
   font_data_2,
/*
   font_data_3,
*/
   0
};
#endif


/*
** Store the current font ID so that it will not have to be
** passed around.  Initialize it to FONT_INVALID_NAME so that
** if a routine is called and the font has not been chosen it
** can return an error.
*/
static INT1 Font_id = FONT_INVALID_NAME;

/*
** Store the current font attributes and the default font 
** attributes.  Some of these are not able to be computed 
** from the font data.  Initialize these now.
*/
static FONT_ATTR Font_attr = {           /* Current settings */
   0,
   0,
   0,
   0,
   FONT_STYLE_NORMAL,  /* By default the text has no special features */
   GR_BLACK,           /* By default the text is black */
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
};
static FONT_ATTR Font_attr_default = {   /* Default settings */
   0,
   0,
   0,
   0,
   FONT_STYLE_NORMAL,  /* By default the text has no special features */
   GR_BLACK,           /* By default the text is black */
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
};

/*
** Store the current focus point.
*/
static FONT_COORD Font_pos = { 0, 0 };   /* Default to (0,0) */


/****************************************************************************
**
**  Unit Name:
**      font_select
**
**  Description: 
**      This function selects the default font to be used by the string
**      display routines.
**
**  Calling Convention:
**      INT1 font_select (char *fontname);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      fontname        pointer to a null terminated string containing the
**                      name of the font to be selected
**
**  Return Value:
**      ID of font selected, FONT_INVALID_NAME if font is not supported
**
**  Notes:
**      The values that are preset for the font (kerning, leading, etc.)
**      are just rough approximations of what they should be.  Feel free
**      to set your own.
**
*****************************************************************************/

/* BEGIN FONT_SELECT */
/* SEARCH FONT TABLE FOR FONTNAME SPECIFIED */
/* IF (FONTNAME IS FOUND) THEN */
/* .  SET FONTID EQUAL TO TABLE INDEX */
/* .  LOAD FONT_ATTRIBUTES STRUCTURE WITH INFO ABOUT THAT FONT */
/* .  LOAD DEFAULT FONT_ATTRIBUTES STRUCTURE WITH INFO ABOUT THAT FONT */
/* ELSE */
/* .  SET FONTID EQUAL TO FONT_INVALID_NAME */
/* END IF */
/* RETURN (FONTID) */
/* END FONT_SELECT */

INT1 font_select (char *fontname)
{
   register UINT1 i      = 0;   /* Loop index */
   register INT1  old_id = Font_id;
   register UINT1 old_style;
   
   for (i = 0; Font_table[i] != 0; i++) {
      if (!strcmp (fontname, Font_table[i]->name)) {
         Font_id = i;
         /*
         ** Load the default structure with information so that the user
         ** can fall back on the defaults if he screws up his settings
         ** too much.  Once the default structure is set up  copy it into
         ** the current structure.
         */
         Font_attr_default.height        = Font_table[Font_id]->height;
         Font_attr_default.space_width   = Font_table[Font_id]->space_width;
         Font_attr_default.kerning       = Font_table[Font_id]->kerning;
         Font_attr_default.leading       = Font_table[Font_id]->leading;
         Font_attr_default.underline_thickness = 
            (Font_table[Font_id]->height >> 4) + 1;
         Font_attr_default.underline_offset =
            (Font_table[Font_id]->height >> 3) + 2;
         Font_attr_default.italic_offset = Font_table[Font_id]->height >> 2;
         Font_attr_default.first_char    = Font_table[Font_id]->first_char;
         Font_attr_default.last_char     = Font_table[Font_id]->last_char;
         Font_attr_default.tab_width     =
            (5 * Font_table[Font_id]->space_width);
         Font_attr_default.max_ascender  = Font_table[Font_id]->max_ascender;
         Font_attr_default.max_descender = Font_table[Font_id]->max_descender;

         old_style = Font_attr.style;
         Font_attr = Font_attr_default;

         if (old_id != FONT_INVALID_NAME) {
            Font_attr.style = old_style;
         }
         break;
      }
   }

   return (Font_id);
}


/****************************************************************************
**
**  Unit Name:
**      font_get_attributes
**
**  Description: 
**      This function returns all of the available information about
**      the default font.
**
**  Calling Convention:
**      INT1 font_get_attributes (FONT_ATTR *current_attr);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      current_attr    pointer to a FONT_ATTR structure
**
**  Return Value:
**      FONT_SUCCESS if successful, FONT_INVALID_NAME if no font 
**      has been selected
**
**  Notes:
**      none
**
*****************************************************************************/

/* BEGIN FONT_GET_ATTRIBUTES */
/* IF (THERE IS A FONT SELECTED) THEN */
/* .  COPY STATIC FONT STRUCTURE INTO USER'S STRUCTURE */
/* .  STATUS = SUCCESS */
/* ELSE */
/* .  STATUS = FAILURE */
/* END IF */
/* RETURN (STATUS) */
/* END FONT_GET_ATTRIBUTES */

INT1 font_get_attributes (FONT_ATTR *current_attr)
{
   register INT1 status = FONT_INVALID_NAME;  /* Status value to return */

   if (Font_id != FONT_INVALID_NAME) {
      *current_attr = Font_attr;
      status = FONT_SUCCESS;
   }

   return (status);
}


/****************************************************************************
**
**  Unit Name:
**      font_set_attributes
**
**  Description: 
**      This function sets allowable values of the information about
**      the default font.  Allowable values to set are:
**        space_width
**        kerning
**        leading
**        style
**        color
**        underline_thickness
**        underline_offset (NOTE:changing the underline_offset does not
**                          automatically change the height or the leading)
**        tab_width
**
**  Calling Convention:
**      INT1 font_set_attributes (FONT_ATTR *new_attr);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      new_attr        pointer to a FONT_ATTR structure which contains
**                      the new data
**
**  Return Value:
**      FONT_SUCCESS if successful, FONT_INVALID_NAME if no font 
**      has been selected
**
**  Notes:
*****************************************************************************/

/* BEGIN FONT_SET_ATTRIBUTES */
/* IF (THERE IS A FONT SELECTED) THEN */
/* .  COPY USER'S STRUCTURE INTO STATIC FONT STRUCTURE */
/* .  STATUS = SUCCESS */
/* ELSE */
/* .  STATUS = FAILURE */
/* END IF */
/* RETURN (STATUS) */
/* END FONT_SET_ATTRIBUTES */

INT1 font_set_attributes (FONT_ATTR *new_attr)
{
   register INT1 status = FONT_INVALID_NAME;  /* Status value to return */
   CHAR_HEADER *char_table;

   if (Font_id != FONT_INVALID_NAME) {
      Font_attr.space_width         = new_attr->space_width;
      char_table = CHAR_DATA (' ');
      char_table->width = new_attr->space_width;
      Font_attr.kerning             = new_attr->kerning;
      Font_attr.leading             = new_attr->leading;
      Font_attr.style               = new_attr->style;
      Font_attr.color               = new_attr->color;
      Font_attr.underline_thickness = new_attr->underline_thickness;
      Font_attr.underline_offset    = new_attr->underline_offset;
      Font_attr.italic_offset       = new_attr->italic_offset;
      Font_attr.tab_width           = new_attr->tab_width;
      status = FONT_SUCCESS;
   }

   return (status);
}


/****************************************************************************
**
**  Unit Name:
**      font_reset_attributes
**
**  Description: 
**      This function sets the font attributes back to their original values
**      and returns the old settings in case they are needed later
**
**  Calling Convention:
**      INT1 font_reset_attributes (FONT_ATTR *new_attr, FONT_ATTR *old_attr);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      new_attr        pointer to a FONT_ATTR structure to hold the new
**                      settings 
**      old_attr        pointer to a FONT_ATTR structure to hold the old
**                      settings 
**
**  Return Value:
**      FONT_SUCCESS if successful, FONT_INVALID_NAME if no font 
**      has been selected
**
**  Notes:
**      none
**
*****************************************************************************/

/* BEGIN FONT_RESET_ATTRIBUTES */
/* IF (THERE IS A FONT SELECTED) THEN */
/* .  COPY STATIC STRUCTURE INTO USER'S STRUCTURES */
/* .  COPY DEFAULT STRUCTURE INTO CURRENT STRUCTURE */
/* .  STATUS = SUCCESS */
/* ELSE */
/* .  STATUS = FAILURE */
/* END IF */
/* RETURN (STATUS) */
/* END FONT_RESET_ATTRIBUTES */

INT1 font_reset_attributes (FONT_ATTR *new_attr, FONT_ATTR *old_attr)
{
   register INT1 status = FONT_INVALID_NAME;  /* Status value to return */

   if (Font_id != FONT_INVALID_NAME) {
      *old_attr = Font_attr;
      *new_attr = Font_attr_default;
      Font_attr = Font_attr_default;
      status = FONT_SUCCESS;
   }
 
   return (status);
}


/****************************************************************************
**
**  Unit Name:
**      font_string_size
**
**  Description: 
**      This function determines the length and height of the bounding
**      rectangle for a given string when it is marked up in the current
**      font
**
**  Calling Convention:
**      INT1 font_string_length (char *text, &length, &ascender, &descender);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      text            pointer to the null terminated string to be sized
**      length          pointer to a buffer to hold the calculated length 
**                      of the string
**      ascender        pointer to a buffer to hold the calculated height
**                      of the string above the baseline
**      descender       pointer to a buffer to hold the calculated height
**                      of the string below the baseline
**
**  Return Value:
**      FONT_SUCCESS if successful, FONT_INVALID_NAME if no font 
**      has been selected
**
**  Notes:
**      This routine ignores underlining (and bold, but since bold is not 
**      implemented...).
**
**      does not count underline as part of text dimensions
**      does not calculate the height of a block of text properly
**
*****************************************************************************/

/* BEGIN FONT_STRING_SIZE */
/* IF (THERE IS A FONT SELECTED) THEN */
/* .  FOR EACH CHARACTER IN THE STRING */
/* .  .  IF (CHARCTER IS A '\') THEN */
/* .  .  .  IF (NEXT CHARACTER = '\') THEN */
/* .  .  .  .  PROCESS AS A SINGLE '\' */
/* .  .  .  .  DETERMINE HOW WIDE IT IS AND ADD THIS TO THE RUNNING TOTAL */
/* .  .  .  ELSE */
/* .  .  .  .  PROCESS AS EMBEDDED COMMAND */
/* .  .  .  END IF */
/* .  .  ELSE */
/* .  .  .  PROCESS CHARACTER */
/* .  .  .  DETERMINE HOW WIDE IT IS AND ADD THIS TO THE RUNNING TOTAL */
/* .  .  END IF */
/* .  END FOR */
/* .  DETERMINE THE HEIGHT OF THE STRING */
/* .  STATUS = SUCCESS */
/* ELSE */
/* .  STATUS = FAILURE */
/* END IF */
/* RETURN (STATUS) */
/* END FONT_STRING_SIZE */

INT1 font_string_size (char *text, INT2 *length, 
                       INT2 *ascender, INT2 *descender)
{
   INT1 status = FONT_INVALID_NAME;  /* Status value to return */
   register INT2 i=0;                /* Loop index for scanning input */
   char *ptr;                        /* Pointer into the input string */
   BOOL first_char = TRUE;           /* Is this the first character? */
   BOOL add_italic = FALSE;          /* Need to add italic_offset? */
   BOOL escape_flag = FALSE;         /* Was the character preceded with '\'? */
   INT2 max_ascender = 0;            /* Highest height above baseline */
   INT2 max_descender = 0;           /* Lowest low below baseline */
   INT2 max_underscore = 0;          /* Lowest underscore below baseline */
   INT2 max_length = 0;              /* Longest line found */
   INT2 next_length;                 /* Length of next line in input */
   INT2 next_ascender;               /* Height of next ascender in input */
   INT2 next_descender;              /* Depth of next descender in input */
   FONT_ATTR old_attr;               /* Store the current attributes */

   /*
   ** See if a font has been selected yet.  If not signal an error 
   ** and exit
   */
   if (Font_id == FONT_INVALID_NAME) {
      return (FONT_INVALID_NAME);
   }

   /*
   ** Get the current attributes so that they can be reset after
   ** this routine is over.
   */
   status = font_get_attributes (&old_attr);
   if (status) {
      return (status);
   }
   
   /*
   ** Scan through each symbol in the input.  Be sure to catch any
   ** embedded \ commands
   */
   for (i = 0; text[i]; i++) {
      /*
      ** If this is a '\' then look at the character next to it.
      */
      if ((text[i] == '\\') && !escape_flag) {
         switch (text[++i]) {
            case 'b': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_BOLD);
               break;
            case 'C':
            case 'c':
               switch (text[++i]) {
                  case 'W':
                  case 'w':
                     Font_attr.color = GR_WHITE;
                     break;
                  case 'G':
                  case 'g':
                     Font_attr.color = GR_GRAY;
                     break;
                  case 'B':
                  case 'b':
                     Font_attr.color = GR_BLACK;
                     break;
                  default:
                     break;
               }
               break;
            case 'i': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_ITALIC);
               break;
            case 'n': 
               max_ascender = MAX (max_ascender, Font_attr.max_ascender);
               /*
               ** If there is a new line then call this function
               ** again.  Have it calculate the new values of the
               ** next line.  Once it returns exit as there is
               ** nothing left to process.
               */
               ptr = (char *) &text[++i];
               status = font_string_size (ptr, &next_length,
                                          &next_ascender, &next_descender);
               if (status) {
                  return (status);
               }
               *length    = MAX (max_length, next_length);
               *ascender  = max_ascender;
               max_descender = MAX (max_descender, Font_attr.max_descender);
               *descender = MAX (max_descender, max_descender + 
                                                Font_attr.leading + 
                                                next_ascender + 
                                                next_descender);
               /*
               ** If an italic character was the last thing on the line then
               ** the italic offset needs to be added to the length of the line
               ** to account for it.
               */
               if (add_italic) {
                  max_length += Font_attr.italic_offset;
                  add_italic = FALSE;
               }
               /*
               ** Reset the attributes.
               */
               status = font_set_attributes (&old_attr);
               return (status);
               break;
            case 't': 
               max_length += Font_attr.tab_width - (max_length % Font_attr.tab_width);
               break;
            case 'u': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_UNDERLINE);
               break;
            case '+': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_SUPERSCRIPT);
               break;
            case '-': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_SUBSCRIPT);
               break;
            default:
               /*
               ** This is just a regular character.  Catch it the next
               ** time around.
               */
               i--;
               escape_flag = TRUE;
               break;
         }
      }
      else {
         /*
         ** If this is an escaped character then turn off the escape flag.
         */
         escape_flag = FALSE;

         /*
         ** If this character is not in the font then it has zero size.
         */
         if ((text[i] > Font_attr.last_char) ||
             (text[i] < Font_attr.first_char)) {
            next_length    = 0;
            next_ascender  = 0;
            next_descender = 0;
         }
         else {
            /*
            ** This is a regular character.
            */
            status = character_dimensions (text[i], &next_length, 
                                           &next_ascender, &next_descender);
            if (status) {
               return (status);
            }
         }

         if (!(Font_attr.style & FONT_STYLE_ITALIC)) {
            if (add_italic) {
               next_length += Font_attr.italic_offset;
               add_italic = FALSE;
            }
         }
         else {
            max_length -= Font_attr.italic_offset;
            add_italic = TRUE;
         }

         
         /*
         ** If this character is underlined then that may have to
         ** be taken into account in the height of the string later.
         */
         if (Font_attr.style & FONT_STYLE_UNDERLINE) {
            max_underscore = MAX (max_underscore, Font_attr.underline_offset + Font_attr.underline_thickness);
         }

         max_length   += next_length;
         max_ascender  = MAX (max_ascender, next_ascender);
         max_descender = MAX (max_descender, next_descender);
         if (!(first_char)) {
            max_length += Font_attr.kerning;
         }
         first_char = FALSE;
      }
   }
   
   /*
   ** If an italic character was the last thing on the line then
   ** the italic offset needs to be added to the length of the line
   ** to account for it.
   */
   if (add_italic) {
      max_length += Font_attr.italic_offset;
      add_italic = FALSE;
   }

   /*
   ** If there was an underlined character on this line then adjust
   ** the max_descender value to take this into account.
   */
   max_descender  = MAX (max_descender, max_underscore);
   
   /*
   ** Reset the attributes.
   */
   status = font_set_attributes (&old_attr);
   if (status) {
      return (status);
   }
   
   /*
   ** Copy the calculated values into the result variables.
   */
   *length      = max_length;
   *ascender    = MAX (max_ascender,  Font_attr.max_ascender);
   *descender   = MAX (max_descender, Font_attr.max_descender);

   /*
   ** If this line is reached then no errors were found.  Return
   ** a success value
   */
   return (FONT_SUCCESS);
}


/****************************************************************************
**
**  Unit Name:
**      font_get_pos
**
**  Description: 
**      This function gets the coordinates of the current focus point.
**      The focus corresponds to the baseline of the font.
**
**  Calling Convention:
**      void font_get_pos (FONT_COORD *current_pos);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      current_pos     pointer to a FONT_COORD structure 
**
**  Return Value:
**      none
**
**  Notes:
**      none
**
*****************************************************************************/

void font_get_pos (FONT_COORD *current_pos)
{
   *current_pos = Font_pos;
}


/****************************************************************************
**
**  Unit Name:
**      font_set_pos
**
**  Description: 
**      This function sets the coordinates of the current focus point.
**      The focus corresponds to the baseline of the font.
**
**  Calling Convention:
**      INT1 font_set_pos (FONT_COORD *new_pos);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      new_pos         pointer to a FONT_COORD structure 
**
**  Return Value:
**      If coordinate is off of the screen then FONT_INVALID_COORD,
**      else FONT_SUCCESS
**
**  Notes:
**      none
**
*****************************************************************************/

INT1 font_set_pos (FONT_COORD *new_pos)
{
   register INT1 status = FONT_INVALID_COORD;  /* Status value to return */

   if (new_pos->x <= SCREEN_MAX_X && new_pos->x >= SCREEN_MIN_X &&
       new_pos->y <= SCREEN_MAX_Y && new_pos->y >= SCREEN_MIN_Y) {
      Font_pos = *new_pos;
      status = FONT_SUCCESS;
   }
 
   return (status);
}


/****************************************************************************
**
**  Unit Name:
**      font_printf
**
**  Description: 
**      This function formats and outputs a string, starting at the
**      default focus point.  If the string goes off the edge of the
**      screen an error will be returned.  If a character is encountered
**      in the input that is a non-printing character, nothing will be
**      output for that particular character.
**
**  Calling Convention:
**      INT1 font_printf (char *text);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      text            Pointer to a null terminated string to be printed,
**                      which can have any of the following embedded 
**                      commands in it:
** 
**                         \b  = toggle bold
**                         \cX = set color to X where X is B, G, or W
**                         \fX = set font to X where X is a single digit
**                         \i  = toggle italics
**                         \n  = carriage return/line feed
**                         \t  = tab
**                         \u  = toggle underlining
**                         \+  = toggle superscript
**                         \-  = toggle subscript
**
**  Return Value:
**      FONT_SUCCESS if successful, FONT_INVALID_NAME if no font 
**      has been selected, FONT_INVALID_COORD if it is detected that the
**      output has gone off of the edge of the screen.  NOTE: it
**      may not always be detected that a character has gone off of
**      the screen.
**
**  Notes:
*****************************************************************************/

/* BEGIN FONT_PRINTF */
/* IF (THERE IS A FONT SELECTED) THEN */
/* .  FOR EACH CHARACTER IN THE STRING */
/* .  .  IF (CHARCTER IS A '\') THEN */
/* .  .  .  IF (NEXT CHARACTER = '\') THEN */
/* .  .  .  .  PRINT A SINGLE '\' */
/* .  .  .  ELSE */
/* .  .  .  .  PROCESS AS EMBEDDED COMMAND */
/* .  .  .  END IF */
/* .  .  ELSE */
/* .  .  .  PRINT CHARACTER */
/* .  .  END IF */
/* .  END FOR */
/* .  STATUS = SUCCESS */
/* ELSE */
/* .  STATUS = FAILURE */
/* END IF */
/* RETURN (STATUS) */
/* END FONT_PRINTF */

INT1 font_printf (char *text)
{
   register INT1 status = FONT_INVALID_NAME; /* Status value to return */
   register INT1 old_space_width;            /* To store spacE_width */
   register BOOL italic_print = FALSE;       /* Was an italic just printed? */
   register BOOL escape_flag = FALSE;        /* Was the last char a '\'? */
   register INT2 i=0;                        /* Index for scanning input */
   FONT_COORD start_pos;                     /* Starting position */
   FONT_COORD pos;                           /* Current focus point */
  
   /*
   ** See if a font has been selected yet.  If not signal an error 
   ** and exit
   */
   if (Font_id == FONT_INVALID_NAME) {
      return (FONT_INVALID_NAME);
   }

   /*
   ** Store the starting position to use as a reference for newlines, 
   ** etc.  Also get the current position.
   */
   start_pos = Font_pos;
   pos = Font_pos;

   /*
   ** Scan through each symbol in the input.  Be sure to catch any
   ** embedded \ commands
   */
   i = -1;
   while (text[++i]) {
      /*
      ** If this is a '\' then look at the character next to it.
      */
      if ((text[i] == '\\') && !escape_flag) {
         switch (text[++i]) {
            case 'B': 
            case 'b': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_BOLD);
               break;
            case 'C':
            case 'c':
               switch (text[++i]) {
                  case 'W':
                  case 'w':
                     Font_attr.color = GR_WHITE;
                     break;
                  case 'G':
                  case 'g':
                     Font_attr.color = GR_GRAY;
                     break;
                  case 'B':
                  case 'b':
                     Font_attr.color = GR_BLACK;
                     break;
                  default:
                     break;
               }
               break;
            case 'F': 
            case 'f':
               Font_id = text[++i] - '0';
               Font_attr.height = Font_table[Font_id]->height;
               break;
            case 'I': 
            case 'i': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_ITALIC);
               break;
            case 'N': 
            case 'n': 
               font_get_pos (&pos);
               pos.x  = start_pos.x;
               pos.y += Font_attr.height + Font_attr.leading;
               if (pos.y > SCREEN_MAX_Y || pos.y < SCREEN_MIN_Y){
                  return (FONT_INVALID_COORD);
               }
               Font_pos = pos;
               italic_print = FALSE;
               break;
            case 'T': 
            case 't': 
               /*
               ** Make a space be just as wide as the tab.  Print
               ** the space.  This way if underlining is on it
               ** will get done.  Be sure to subtract the kerning
               ** from the X position when done.
               */
               pos = Font_pos;
               pos.x += Font_attr.tab_width - ((pos.x - start_pos.x) % Font_attr.tab_width);
               if (pos.x > SCREEN_MAX_X) {
                  pos.x = SCREEN_MAX_X;
               }
               if (pos.x < SCREEN_MIN_X) {
                  pos.x = SCREEN_MIN_X;
               }
               old_space_width = Font_attr.space_width;
               Font_attr.space_width = pos.x - Font_pos.x;
               status = font_putc (' ');
               Font_pos.x -= Font_attr.kerning;
               if (status) {
                  return (status);
               }
               Font_attr.space_width = old_space_width;
               break;
            case 'U': 
            case 'u': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_UNDERLINE);
               break;
            case '+': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_SUPERSCRIPT);
               break;
            case '-': 
               TOGGLE_BIT (Font_attr.style, FONT_STYLE_SUBSCRIPT);
               break;
            case '\\': /* Fall through to default... */
            default:
               i--;
               escape_flag = TRUE;
               break;
         }
     
      }
      else {
         /*
         ** Reset the escape_flag flag.
         */
         escape_flag = FALSE;

         if (Font_attr.style & FONT_STYLE_ITALIC) {
            italic_print = TRUE;
         }
         else {
            if (italic_print) {
               Font_pos.x += Font_attr.italic_offset;
               italic_print = FALSE;
            }
         }
         /*
         ** This is a regular character.  Print it.  If it is 
         ** an invalid character then just ignore it.
         */
         status = font_putc (text[i]);  
         if ((status) && (status != FONT_INVALID_CHAR)) {
            return (status);
         }
      }
   }
   
   /*
   ** If this line is reached then no errors were found.  Return
   ** a success value
   */
   return (FONT_SUCCESS);
}

/****************************************************************************
**
**  Unit Name:
**      font_printf_centered
**
**  Description: 
**      This function prints a block of text just as font_printf would,
**      but it centers it in relation to the two coordinates passed
**      to it.
**
**  Calling convention:
**      INT1 font_printf_centered (char *text, INT2 x1, INT2 y1, 
**                                 INT2 x2, INT2 y2);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      text            pointer to the text to be printed
**      x1, y1          coordinate of upper left corner of window
**      x2, y2          coordinate of lower right corner of window
**
**  Return Value:
**      FONT_SUCCESS if successful, FONT_INVALID_NAME if no font 
**      has been selected, FONT_INVALID_COORD if it is noticed that
**      the printing has gone off the screen, FONT_INVALID_CHAR if
**      the character does not exist in this font
**
**  Notes:
**      This function does not check to see if your text goes out
**      of the specified window.  This is so that you can set the
**      two Y values equal and force the text to print at that Y
**      location on the screen, just centering on the X bounds
**      (same with setting the X values equal).
**
*****************************************************************************/

/* BEGIN FONT_PRINTF_CENTERED */
/* IF (THERE IS A FONT SELECTED) THEN */
/* .  GET THE SIZE OF THE BLOCK OF TEXT */
/* .  CALCULATE THE STARTING X COORD */
/* .  CALCULATE THE STARTING Y COORD */
/* .  GO THERE */
/* .  PRINT THE BLOCK OF TEXT */
/* .  STATUS = SUCCESS */
/* ELSE */
/* .  STATUS = FAILURE */
/* END IF */
/* RETURN (STATUS) */
/* END FONT_PRINTF_CENTERED */

INT1 font_printf_centered (char *text, INT2 x1, INT2 y1, INT2 x2, INT2 y2)
{
   INT1 status;
   INT2 width;
   INT2 ascending;
   INT2 descending;
   INT2 window_width;
   INT2 window_height;
   FONT_COORD pos;

   status = font_string_size (text, &width, &ascending, &descending);
   if (status) {
      return (status);
   }

   window_width  = x2 - x1;
   window_height = y2 - y1;

   pos.x = x1 + ((window_width - width) >> 1);
   pos.y = y1 + ((window_height - (ascending + descending)) >> 1) + ascending;
   status = font_set_pos (&pos);
   if (status) {
      return (status);
   }

   status = font_printf (text);
   if (status) {
      return (status);
   }

   return (FONT_SUCCESS);
}

/****************************************************************************
**
**  Unit Name:
**      get_next_line
**
**  Description: 
**      Parses a string into tokens delimited by newline markers ('\n').
**
**  Calling convention:
**      (BOOL) success = get_next_line (char *line, char **context)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      line            The line that was parsed from the input string
**      context         The current location in the input string
**
**  Return Value:
**      Status value as to whether there was another line left in the string.
**
**  Notes:
**
*****************************************************************************/
BOOL get_next_line (char *line, char **context)
{
   INT2 i = 0;
   BOOL end_of_line = FALSE;

   /*
   ** If there is nothing to parse then just return.
   */
   if (*context[0] == '\0') {
      return (FALSE);
   }
   else {
      /*
      ** There is something to parse.  Copy text from context into
      ** line until a '\n' is found.
      */
      do {
         /*
         ** If this is an escape character then see what it
         ** is escaping.  If it is an 'n' then this is the
         ** end of the line.  If not, then copy that into the
         ** line.
         */
         if (*context[0] == '\\') {
            (*context)++;
            if (toupper (*context[0]) == 'N') {
               line[i] = '\0';
               (*context)++;
               end_of_line = TRUE;

               /*
               ** If the only thing on this line was a '\n' then
               ** put something into the output string so that
               ** a blank line will actually be printed.
               */
               if (i == 0) {
                  line[i++] = ' ';
                  line[i++] = '\0';
               }
            }
            else {
               line[i++] = '\\';
               line[i++] = *context[0];
            }
         }
         else {
            /*
            ** This is just a regular character.
            */
            line[i++] = *context[0];
            if (*context[0] == '\0') {
               end_of_line = TRUE;
            }
         }

         if (!end_of_line) {
            (*context)++;
         }
      } while (!end_of_line);

   }

   return (TRUE);
}

/****************************************************************************
**
**  Unit Name:
**      font_printf_centered_each
**
**  Description: 
**      This function prints a block of text just as font_printf would,
**      but it centers it in relation to the two coordinates passed
**      to it.  It also centers (horizontally) each line in the string.
**
**  Calling convention:
**      INT1 font_printf_centered_each (char *text, INT2 x1, INT2 y1, 
**                                      INT2 x2, INT2 y2);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      text            pointer to the text to be printed
**      x1, y1          coordinate of upper left corner of window
**      x2, y2          coordinate of lower right corner of window
**
**  Return Value:
**      FONT_SUCCESS if successful, FONT_INVALID_NAME if no font 
**      has been selected, FONT_INVALID_COORD if it is noticed that
**      the printing has gone off the screen, FONT_INVALID_CHAR if
**      the character does not exist in this font
**
**  Notes:
**      This function does not check to see if your text goes out
**      of the specified window.  This is so that you can set the
**      two Y values equal and force the text to print at that Y
**      location on the screen, just centering on the X bounds
**      (same with setting the X values equal).
**
*****************************************************************************/

/* BEGIN FONT_PRINTF_CENTERED_EACH */
/* IF (THERE IS A FONT SELECTED) THEN */

/* ELSE */
/* .  STATUS = FAILURE */
/* END IF */
/* RETURN (STATUS) */
/* END FONT_PRINTF_CENTERED_EACH */

INT1 font_printf_centered_each (char *text, INT2 x1, INT2 y1, INT2 x2, INT2 y2)
{
   INT1 status;
   INT2 width;
   INT2 ascending;
   INT2 descending;
   INT2 window_width;
   INT2 window_height;
   FONT_COORD pos;
   FONT_ATTR  attr;
   char *context = text;
   char line[255];

   status = font_get_attributes (&attr);

   /*
   ** Figure out the height of the text.
   */
   status = font_string_size (text, &width, &ascending, &descending);
   if (status) {
      return (status);
   }

   window_width  = x2 - x1;
   window_height = y2 - y1;

   pos.y = y1 + ((window_height - (ascending + descending)) >> 1) + ascending;

   /*
   ** For each line of text, print it centered.
   */
   while (get_next_line (line, &context)) {
      /*
      ** Get the size of this line so that we know where to center it.
      */
      status = font_string_size (line, &width, &ascending, &descending);
      if (status) {
         return (status);
      }

      /*
      ** Figure out where this string goes.
      */
      pos.x = x1 + ((window_width - width) >> 1);

      status = font_set_pos (&pos);
      if (status) {
         return (status);
      }

      /*
      ** Figure out where the next string goes.
      */
      pos.y += attr.height + attr.leading;
 
      /*
      ** Print the line.
      */
      status = font_printf (line);
       if (status) {
          return (status);
      }
   }

   return (FONT_SUCCESS);
}

/****************************************************************************
**
**  Unit Name:
**      font_putc
**
**  Description: 
**      This function outputs a single character, starting at the
**      default focus position.  The position is incremented by the
**      width of the character.  If an input character does not exist 
**      in the default font an error will be signalled.
**
**  Calling convention:
**      INT1 font_putc (UCHAR character);
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      character       character to be printed
**
**  Return Value:
**      FONT_SUCCESS if successful, FONT_INVALID_NAME if no font 
**      has been selected, FONT_INVALID_COORD if it is noticed that
**      the printing has gone off the screen, FONT_INVALID_CHAR if
**      the character does not exist in this font
**
**  Notes:
**      If you use font_putc to put out a character in italics and then
**      call it again immediately after to put one out in normal the 
**      characters will overlap somewhat.  If you are going to mix
**      attributes you should consider calling font_printf which takes
**      all of these anomalies into account.
**      Font_putc also puts out a kerning after every character it prints.
**
*****************************************************************************/

/* BEGIN FONT_PUTC */
/* IF (THERE IS A FONT SELECTED) THEN */
/* .  DETERMINE HOW WIDE IT IS */
/* .  DETERMINE HOW HIGH IT IS */
/* .  DISPLAY CHARACTER */
/* .  UPDATE SCREEN POSITION */
/* .  STATUS = SUCCESS */
/* ELSE */
/* .  STATUS = FAILURE */
/* END IF */
/* RETURN (STATUS) */
/* END FONT_PUTC */

INT1 font_putc (UCHAR character)
{
   register INT1 byte;            /* Byte position in data */
   register INT1 bit;             /* Bit position in byte */
   register INT1 italic_offset=0; /* Adjustment to width of the character */
   register INT1 italic;          /* Adjustment to particular bit */
   register INT2 x;               /* X coord to be plotted */
   register INT2 y;               /* Y coord to be plotted */
   register INT2 width;           /* Width of character */
   register INT2 height;          /* Height of character */
   register INT2 lower;           /* Lower X coord for underline */
   register INT2 upper;           /* Upper X coord for underline */
   register INT2 offset;          /* Adjustment to Y coord */
   register UINT4 addr;           /* Offset into video buffer */
   register UINT1 byte_count;     /* Number of bytes of data */
   register UINT1 *byte_ptr;      /* Pointer to current byte */
   register INT2 bit_count;       /* Number of bits processed so far */
   FONT_COORD origin;             /* Origin of character */
   CHAR_HEADER *char_table;       /* Description of character & bitmap */
                                 
   /*
   ** If there is no font selected then signal an error and exit
   */
   if (Font_id == FONT_INVALID_NAME) {
      return (FONT_INVALID_NAME);
   }

   /*
   ** If this character is not in the font then signal an error
   ** and exit.
   */
   if ((character > Font_attr.last_char) ||
       (character < Font_attr.first_char)) {
      return (FONT_INVALID_CHAR);
   }

   /*
   ** If this is to be printed in italics then the character
   ** will actually be wider than the information says it is
   */
   if (Font_attr.style & FONT_STYLE_ITALIC) {
      italic_offset = Font_attr.italic_offset;
      if (italic_offset < 0) {
         if ((Font_pos.x + italic_offset) < SCREEN_MIN_X) {
            return (FONT_INVALID_COORD);
         }
      }
   }
   
   /*
   ** See if the character will fit on the screen.  If it does
   ** not then signal an error and exit.  Ignore things like 
   ** underlining.  This routine just wants to get the character
   ** out.  If the underline falls off of the screen it does not
   ** care.
   */
   font_get_pos (&origin);
   char_table = CHAR_DATA (character);
   width = char_table->width;
   if (character == ' ') {
      width = Font_attr.space_width;
   }
   height = char_table->height;
   if ((origin.x + width + italic_offset)  > SCREEN_MAX_X || 
       (origin.x + italic_offset)  < SCREEN_MIN_X ||
       (origin.y - char_table->offset) > SCREEN_MAX_Y || 
       (origin.y - (height + char_table->offset)) < SCREEN_MIN_Y) {
      return (FONT_INVALID_COORD);
   }

   /*
   ** Scan through the bitmap and plot a point for each cell
   ** that has a 1 in it.  Scan through each byte checking to
   ** see if it is non-zero.  If it is non-zero then there is
   ** data to be plotted.  Scan through each bit of the byte
   ** and plot it if it needs to be.
   */

   offset = origin.y - char_table->offset - height;
   byte_count = ((width * height) + 7) >> 3;
   byte_ptr = char_table->data;


   /*
   ** This code is duplicated twice for speed.  This IF 
   ** statement selects which loop to execute based on what
   ** the current color is.  If the color is black 
   ** then we do not need to look up what the dithering is,
   ** we can just print that color.  If the color is some sort
   ** of grey then we have to look up what grey dithers to.
   ** Note that changes made to either one of these two sections
   ** must be made to the other one.
   */
   if (Font_attr.color == GR_BLACK) {
      for (byte = 0; byte < byte_count; byte++) {
         if (*byte_ptr) {
            for (bit = 0; bit < 8; bit++) {
               if (*byte_ptr & (1 << (7 - bit))) { 
                  bit_count = (byte << 3) + bit;
                  italic = italic_offset - 
                           (italic_offset * (bit_count / width) / 
                           Font_attr.height);
                  x = origin.x + (bit_count % width) + italic;
                  y = (bit_count / width) + offset;
                  /*
                  ** Plot the point using the specified color
                  */
                  addr = VID_ADDR (x, y);
                  vidbuf[addr] |= mask_or[x & 7];
               }
            }
         }
         byte_ptr++;
      }
   }
   else{
      for (byte = 0; byte < byte_count; byte++) {
         if (*byte_ptr) {
            for (bit = 0; bit < 8; bit++) {
               if (*byte_ptr & (1 << (7 - bit))) { 
                  bit_count = (byte << 3) + bit;
                  italic = italic_offset - 
                           (italic_offset * (bit_count / width) / 
                           Font_attr.height);
                  x = origin.x + (bit_count % width) + italic;
                  y = (bit_count / width) + offset;
                  /*
                  ** Plot the point using the specified color
                  */
                  addr = VID_ADDR (x, y);
                  if (PIX_VAL(x, y, Font_attr.color) == 1)
                  {
                     vidbuf[addr] |= mask_or[x & 7];
                  }
                  else
                  {
                     vidbuf[addr] &= mask_and[x & 7];
                  }
               }
            }
         }
         byte_ptr++;
      }
   }

   
   

   /*
   ** If underlining is on then draw an underline for this character.
   ** Allow the underline to precede and to follow the character by
   ** half of the kerning.  This way underlined characters that are
   ** adjacent will have a continuous underline.
   */
   if (Font_attr.style & FONT_STYLE_UNDERLINE) {
      lower = 0;
      upper = 0;
      if (Font_attr.style & FONT_STYLE_ITALIC) {
         upper += Font_attr.italic_offset;
      }
      if (Font_attr.kerning > 0) {
         lower -= Font_attr.kerning >> 1;
         upper += (Font_attr.kerning + 1) >> 1;
      }
      else
      {
         upper -= Font_attr.kerning;
      }
      gr_line ((INT2) origin.x + lower, 
               (INT2) origin.y + Font_attr.underline_offset,
               (INT2) origin.x + width - 1 + upper,
               (INT2) origin.y + Font_attr.underline_offset,
               (INT2) Font_attr.underline_thickness,
               (INT2) Font_attr.color);
               
   }
   
   /*
   ** Set the focus to be after this character.  Be sure to include
   ** the kerning space.  Do not let the focus go off of the screen.
   ** Printing a character can only update the X coordinate of the
   ** focus.
   */
   origin.x += width + Font_attr.kerning;
   if (origin.x > SCREEN_MAX_X) {
      origin.x = SCREEN_MAX_X;
   }
   else {
     if (origin.x < SCREEN_MIN_X) {
        origin.x = SCREEN_MIN_X;
     }
   }

   /*
   ** To get here no errors must have been found.  Set the
   ** focus to the new location and return the status of that.
   */
   return (font_set_pos (&origin));
}


/****************************************************************************
**
**  Unit Name:
**      character_dimensions
**
**  Description: 
**      This function returns the width, height, and offset from the
**      baseline of a single character
**
**  Calling Convention:
**      UINT1 character_dimensions (char character, UINT2 *width,
**                                  UINT2 *ascender, UINT2 *descender)
**
**  Parameters:
**      Name            Description
**      ----            ----------- 
**      character       character to measure 
**      width           pointer to a buffer to receive the width of the
**                      character
**      ascender        pointer to a buffer to receive the height of the
**                      character above the baseline
**      descender       pointer to a buffer to receive the height of the
**                      character below the baseline
**
**  Return Value:
**      FONT_INVALID_NAME if no font has been selected, FONT_SUCCESS
**      otherwise
**
**  Notes:
**      This includes the style attribute effects, such as the italic
**      character being wider, etc.
**
*****************************************************************************/

UINT1 character_dimensions (char character, INT2 *width, 
                            INT2 *ascender, INT2 *descender) 
{
   register INT1 status = FONT_INVALID_NAME;
   register INT2 index;          /* Index into data */
   register UINT1 relative_char; /* Character offset from start of symbol set */
   CHAR_HEADER *char_data;       /* Mask to overlay onto stream data */
   
   if (Font_id != FONT_INVALID_NAME) {
      status = FONT_SUCCESS;
      char_data = CHAR_DATA (character);

      /*
      ** Note: Be sure to change this statement to take into account
      **       things such as italics and bold!
      */
      *width = char_data->width;
      if (Font_attr.style & FONT_STYLE_ITALIC) {
         *width += Font_attr.italic_offset;
      }

      *ascender  = char_data->height + char_data->offset;
      if (char_data->offset < 0) {
         *descender = -char_data->offset;
         if (*ascender < 0) {
            *ascender = 0;
         }
      }
      else {
         *descender = 0;
      }
   }

   return (status);
}

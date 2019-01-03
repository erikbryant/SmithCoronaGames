/****************************************************************************
*
*		$Workfile:   FONTS.H  $	- fonts.h
*
*		$Revision:   1.12  $			$Modtime:   17 Sep 1991 13:23:30  $
*
*		$Author:   SueB  $
*
*
*
*	(C) Copyright 1990 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/H/FONTS.H_V  $
* 
*    Rev 1.12   17 Sep 1991 15:03:46   SueB
* Change to fontWrtHdr to add an amount above the em-square height for extended
* characters for mono-spaced CRT fonts.
* 
*    Rev 1.11   11 Sep 1991 10:38:28   SueB
* Changes to incorporate fax font into the font rom. Also had to change fontGet
* Ids to not return an id for a fax font because the user can't choose it.
* 
*    Rev 1.10   09 Aug 1991 16:56:08   SueB
* Changes to treat required space like a space when accesing font tables.
* 
*    Rev 1.9   04 Jun 1991 09:55:20   SueB
* Changes to use fixed printer heights for Daisy models.  Use fixed heights for
* mono-spaced fonts on LCD.
* 
*    Rev 1.8   30 May 1991 11:09:56   SueB
* Changes to make height values: font_height, lns_below_base and leading fixed
* for a monospaced font.
* 
*    Rev 1.7   17 May 1991 07:57:54   SueB
* Changed FONTIDPTR, misc. changes to clean up some warnings,free a pointer.
* 
*    Rev 1.6   14 May 1991 13:28:10   SueB
* Change parameters to fontHeightPrt to pass back three height values in a 
* structure. Font rom structure was also changed.
* 
*    Rev 1.5   12 Apr 1991 16:17:02   SueB
* Added new function fontGetDmNames. New structure fontdm in fonts.h.
* 
*    Rev 1.4   05 Apr 1991 09:07:24   SueB
* Put #ifndef back in header and prototype files.
* 
*    Rev 1.3   04 Apr 1991 10:16:50   SueB
* Remove nested include files, update source accordingly.
* 
*    Rev 1.2   18 Mar 1991 11:34:06   SueB
* Added types for two new functions fontGetAll and fontGetIds.
* 
*    Rev 1.1   07 Mar 1991 15:13:20   SueB
* Changes made to incorporate Corona art into font rom.
* 
*    Rev 1.0   26 Feb 1991 14:03:42   SueB
* Initial revision.
*
*
*
****************************************************************************/
#ifndef FONTSH
#define FONTSH

/*************************************/
/* Header file for font information. */
/*************************************/

#ifdef MSDOS
#define FAR far
#else
#define FAR
#endif

#ifdef NOCONST	
#define CONST 
#else
#define CONST const
#endif


#define TERM '\0'
#define FONT_TYPE_SIZE 15 /* No. of characters in font typeface name */
#define FONT_STYLE_SIZE 9 /* No. of characters in font style name */
#define FONT_NAME_SIZE (FONT_TYPE_SIZE + FONT_STYLE_SIZE +1)
  /*size of typeface name concatenated with style name separated by a blank */
#define FONT_STR_LEN 50	
 /* No of chars in string of font information: typeface,style and size */
#define FONT_SOFT_NO_SIZE 6 /* No of characters in software number */
#define NOFONT ((SYSFONTID) 0xFFFF)	 /* value when SYSFONTID not found */
#define NOINDEX -1	 /* Value when character index not found */

#define UNITS_PER_PASS 50 /* No of printer units per vertical pass */
#define PRT_RES 300 /* Resolution of the printer in printer units */
#define PRT_HGT_MULT 3	/* Constant to multiply point_size by, to get prt hgt*/
#define PRT_HGT_ADD 2	/* Adjustment to point_size for prop. fonts */
#define VERT_DRAW_CHAR 124 /* character used to draw vertical line for a box */
#define COR_ART_UNDERLINE_CHAR 95 	/* special character used in Corona art in case */
												/* a user chooses underline mode */
#define PRT_COMMON_DENOM 72  
#define PRT_HALF_ROUND_UP (PRT_RES/2)
#define PRT_CART_OFF_FROM_BASE 12 /* Offset from baseline for a Corona art picture */
                                   /* in printer units */

/* define fixed  height information for mono-spaced fonts */
#define MONO_FONT_HEIGHT 9		 /* total line height on LCD display */
#define MONO_LNS_BELOW_BASE 2	 /* lines below the baseline on LCD display*/
#define MONO_LEADING 0  /* leading for extended characters */

#define DAISY_LNS_ABOVE_BASE 38  /* values for daisy type printers */
#define DAISY_LNS_BELOW_BASE 12
#define DAISY_LEADING 0

#define CRT_MONO_ABOVE_EM_SQ 1 /* amount of extended characters above em-square for CRT */
										 /* mono-spaced fonts, added to font height */

#define REQUIRED_SPACE 0x07 /* this is the same as the required space in editdefs.h */
#define FONT_SPACE_CHAR 32 

/* Bitmap type */
typedef unsigned char BITMAPUNIT;

/* Types for font id count and pointer to list of font ids */
typedef SHORT *FONTIDCOUNT;
typedef SYSFONTID *FONTIDPTR;

/* Type for style and number to turn on flag */
typedef struct
{
	unsigned char *name;
	short num;
} fstyle_type;

/* styles for fonts */
typedef enum
{
	normal = 0, italic = 2, reverse =4,corart = 8, ps = 16, dropcap = 32,fax = 64
} FONTSTYLES;

/**********************/
/* Font structures    */
/**********************/
/* Font information put into separate fields for printer, editor, etc */
 typedef struct fontall
 {
 	UCHAR typeface [FONT_TYPE_SIZE +1];
	UCHAR style [FONT_STYLE_SIZE + 1];
	union
	{
		UCHAR pitch;
		UCHAR ca_pos;
	} type;
	union 
	{
		UCHAR point_size;
		UCHAR ca_passes;
   } size;
	USHORT fontflags;
 } *FONTALL;


 /* Structure of information for Dialog Manager to use to display list */
 /* of font names. */
 typedef struct fontdm
	{
		UCHAR **list;
		SHORT count;
	} *FONTDM;

/* Information to identify the font by name and size. Size can be */
/* either pitch or point size */                                    
 typedef struct fonttype
 {
	 UCHAR fname[FONT_NAME_SIZE+1];
	 UCHAR fsize;
 } *FONTTYPE;
	
/* Information for entire font */
 typedef struct fontinfo
 { 
    UCHAR sp_char_width;
    UCHAR font_height;
    UCHAR lns_below_base;
    UCHAR leading;
 } *FONTINFO;

/* Structure of display character parameters */
 typedef struct charparms
 {
    UCHAR offset_from_em_edge;
	 CHAR offset_from_base;
	 UCHAR bitmap_width;
	 UCHAR bitmap_height;
	 UCHAR set_width;
 } *CHARPARMS;

/* Font character directory information: display character parameters */
/* and bitmap offset (as stored) */
 typedef CONST struct chardir
 {
	 struct charparms chparms;	
	 SHORT bitmap_offset;
  } *CHARDIR,cs_chardir;

/* Structure for font header information */
 typedef struct fheadtype
	{
		unsigned char sp_char_width;
		unsigned char font_height;
		unsigned char lns_below_base;
	} *FHEADTYPE;


	
/* Font character directory information: display character parameters and pointer */
/* to the bitmap for the character. (as passed to display routines.) */
 typedef struct fontdisp
 {
	 struct charparms chparms;	
	 CONST BITMAPUNIT FAR *bitmap_ptr;
   } *FONTDISP;

/* Structure for font display table */
	typedef CONST struct	dsptable
	{	
		struct fheadtype header;
		UCHAR rindex;
		CHARDIR chdir;
		CONST BITMAPUNIT FAR *bitmaps;
	} *DSPTABLE,cs_dsptable;


/* Structure for printer table in rom */
	typedef CONST struct prttable
	{ 
		UCHAR rindex;
		UCHAR leading;
		SHORT lns_above_base;
		SHORT lns_below_base;
		CONST UCHAR *widths;
	} *PRTTABLE,cs_prttable;

/* Structure for printer height information */
/* returned to user */
	typedef struct fontprt
	{
		UCHAR leading;
		SHORT lns_above_base;
		SHORT lns_below_base;
	}*FONTPRT;


 /* Structure for font index table */

	typedef CONST struct	fontindex
	{
		UCHAR font_name[FONT_NAME_SIZE+1];
		union
		{
			UCHAR pitch;
			UCHAR ca_pos;
		} type;
		union 
		{
			UCHAR point_size;
			UCHAR ca_passes;
	   } size;
		USHORT fontflags;
		UCHAR soft_no[FONT_SOFT_NO_SIZE+1];
		SHORT rev_level;
		PRTTABLE ptable_ptr;
		DSPTABLE dtable_ptr;
	} *FONTINDEX, cs_fontindex;


/************************************/
/* Character range structures       */
/************************************/

/* Action type to use with range table */
	typedef enum 
	{
		INDEX,SUBTRACT
	}ACTION_TYPE;


/* Structure of range table format */
	typedef CONST struct rangetype
	{
		UCHAR start;		 	 /* starting character of range */
		UCHAR end;			 	 /* ending character of range */
		UCHAR val;		 		 /* Value to be used to calculate char index */
		ACTION_TYPE action;	 /* Action to use with value */
	} *RANGETYPE,cs_rangetype;

/* Structure for range table index */ 
 typedef CONST struct rangeindex
  {
		SHORT no_of_ranges;
		RANGETYPE rtblptr;
  } *RANGEINDEX, cs_rangeindex;			

  /* Structure used for creating range table and range table index */ 
 typedef struct rangearr
  {
  		CHAR *table_name;
		SHORT no_of_ranges;
		RANGETYPE rtblptr;
  } *RANGEARR;			

#endif

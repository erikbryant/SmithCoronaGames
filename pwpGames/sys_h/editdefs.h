/* mode flags for embedded attributes and global modes that can be active */
/* on a character basis. */

#define	INSERT_ACTIVE			0X01
#define	TYPEMODE_ACTIVE		0X02
#define	BOLD_ACTIVE				0X04
#define	HEADER_ETC_ACTIVE		0X08

#define	UNDER_ALL_ACTIVE		0X20
#define	SUPERSCRIPT_ACTIVE	0X40
#define	SUBSCRIPT_ACTIVE		0X80
#define	PAR_INDENT_ACTIVE		0x100
#define	AUTO_CENT_ACTIVE		0X200
#define	FLUSH_TABR_ACTIVE		0X400
#define	CENTER_TAB_ACTIVE		0X800
#define	DECIMAL_TAB_ACTIVE	0X1000
#define	MARKER_ACTIVE			0X2000
#define	REFORMATTING			0X4000
#define	NEED_TO_REFORMAT		0X8000


/* mode flags that happen on a LINE basis - this byte of words resides */
/* in every BLOCK structure. */

#define	SINGLE_LNSPACE			0X01
#define	ONEHLF_LNSPACE			0X02
#define	DOUBLE_LNSPACE			0X04
#define	PROTECT_BLOCK			0X08
#define	LEFT_JUSTIFY			0X10
#define	RM_JUSTIFY				0X20
#define	CENTER_BLOCK			0X40
#define	RFLUSH_BLOCK			0X80
#define	PAR_INDENT_BLOCK		0X100 
#define	LINESPACING_MASK		(SINGLE_LNSPACE+ONEHLF_LNSPACE+DOUBLE_LNSPACE)


/* mode flags that happen on a PAGE basis - this byte of words resides */
/* in every PAGE structure. */

#define	VERT_JUSTIFY			0X01
#define	VERT_CENTER				0X02
#define	NUMBER_PAGE				0X04
#define	PAGE_CHANGED			0X08
#define	LANDSCAPE				0x10

#define	REFORMATTEDTHEPAGE	0x80


/* probe flags for letting the calling routine know what happened */
/* in certain cases. */

#define	CROSSEDSOFTNEWLINE	0X01
#define	CROSSEDFOOTNOTE		0X02
#define	CROSSEDSTOPCODE		0X04

#define	OVER_MARKED_BLOCK		0X80


/* reformat flags - tell what type of reformat, when its active, etc. */

#define	INSERT_REFORMAT	0x01
#define	REFORMAT_CHAR		0x02
#define	REFORMAT_BLOCK		0x04
#define	REFORMAT_PAGE		0x08
#define	REFORMAT_DOC		0x10
#define	MOVED_FROM_MARKER	0x20
#define	REFORMAT_LINE		0x40
#define	SOFT_BLOCK_AT_END_OF_PAGE	0x80
#define	ADDED_SOFT_NEWLINE			0x100
#define	FORMATTING_ACROSS_COLUMNS	0x200
#define	REDRAW_ENTIRE_LINE			0x400
#define	REDRAW_FROM_CURRENT_LINE	0x800
#define	DONE_UPDATING_SCREEN			0x1000
#define	NOT_PAST_FOCUS_MARKER		0x2000
#define	NEED_TO_REFORMAT_PAGES		0x4000
#define	DONT_NEED_TO_REFORMAT		0x8000

/* display flags - global */

#define ALREADY_UPDATED_SCREEN	0x01
#define RIGHT_FLUSH_SCREEN			0x02
#define CENTER_SCREEN				0x04
#define UPDATE_PREVIOUS_LINE		0x08
#define UPDATE_ENTIRE_SCREEN		0x10
#define ALREADY_CLEARED_LINE		0x20
#define TYPED_PAST_RIGHT_MARGIN	0x40

/* used for the deciphering of the SHORT within the tab table */
#define LEFT_TAB              0X1000
#define CENTER_TAB            0x2000
#define FRIGHT_TAB            0x4000
#define DECIMAL_TAB           0x8000
#define MASK_TAB_POSITION		0x0FFF
#define MASK_TAB_TYPE			0xF000

#define	STOPCODE          	0x01
#define	FOCUSMARKER				0x02
#define  DROPMARKER           0x03
#define  TEMP_MARKER          0x04
#define  COLUMNMARKER         0x05
#define	CURSORMARKER			0x06
#define	PRINTCOLUMNMARKER		0x07
/* the one below is reserved - do not use */
#define	SETATTMARKER			0x08


#define	REQUIREDSPACE			0x07
#define	AUTOCENTON				0x08
#define	TABCHARACTER	   	0x09
#define	HARDPAGEEND		   	0x0C
#define	SOFTNEWLINE		   	0x0D
#define	HARDNEWLINE		   	0x0D
#define	FORCEDSOFTNEWLINE		0x0E
#define	SOFTSPECIALFIELDOFF	0x0F
#define	SPACE						0x20
#define	PERIOD					0x2E
#define	EXCLAMATION_POINT		0x21
#define	QUESTION_MARK			0x3F
#define  ASCIIUNDERLINE			0x5F

#define	BOLDON			0x60


#define	UNDERALLON		0x63
#define	SUPERSCRIPTON	0x64
#define	SUBSCRIPTON		0x65

#define	BOLDOFF			0x68


#define	UNDERALLOFF		0x6B
#define	SUPERSCRIPTOFF	0x6C
#define	SUBSCRIPTOFF	0x6D



/* defines for MARKER-MARKER process */
#define 	MARKERON			0x70
#define  MARKEROFF		0x71
#define	MARKERDEL		0x72
#define	MARKERANC		0x73
#define	MARKERELOC		0x74
#define	MARKERWRITE		0x75
#define	MARKERLIMIT		0x1000

/* defines for the dictionary stuff */
#define	WORD_NOT_FOUND	0x01
#define	END_OF_LIST		0x02
#define	WORD_ALERT		0x04
#define	THES_WORD		0x08
#define	PWL_WORD			0x10
#define	END_OF_WORD		0x20
#define	UPPER_CASE_WRD	0x40
#define	CAPITAL_WORD	0x80

#define  MAX_WORD_LEN	30				/* largest possible word in our dictionary */

#define	MAXBUFREAD		0x35

#define	HYPHEN			0x2D

#define	FONTCODE			0x80
#define	FOOTNOTE_MARK	0x81
#define	ENDNOTE_MARK	0x82


#define	NUMBEROFCOLUMNSMASK	0x3F
#define	TYPEOFCOLUMNSMASK		0xC0
#define	NEWSPAPERCOLUMNSMASK	0x80
#define	BALANCECOLUMNSMASK	0x40
#define	THIRDCOLUMN				0x04
#define	FOURTHCOLUMN			0x08


#define  INVALID			-1
#define	WORD_TOO_LONG	-2
#define	TOGGLEDIFF		8
#define	BOLDWIDTHADDER 0		/* zero for now, 2 for Thermal */
#define	ATTRIBUTEVALUE	0xFF
#define	SPECIAL_FIELD	((USHORT) (AUTO_CENT_ACTIVE+FLUSH_TABR_ACTIVE+CENTER_TAB_ACTIVE+DECIMAL_TAB_ACTIVE))

#ifdef MSDOS

#define	PAGE_TEST_KEY			0x5D00	/* SHIFT F10 */

#define	BACKSPACEKEY			0x0008
#define	TABKEY		    		0x0009
#define	INSERTTOGGLE			0x5200
#define	DELETEKEY				0x5300
#define	RIGHTCURSOR    		0x4D00
#define	LEFTCURSOR     	   0x4B00
#define	UPCURSOR					0x4800
#define	DOWNCURSOR				0x5000
#define	STOPCODEKEY				0x1F00		/* ALT S */
#define	AUTOCENTTOGGLE			0x1E00		/* ALT A */
#define	BOLDTOGGLE				0x3000		/* ALT B */
#define	UNDERWORDTOGGLE		0x1100		/* ALT W */
#define	UNDERALLTOGGLE			0x1600		/* ALT U */
#define	SUPERSCRIPTTOGGLE		0x1900		/* ALT P */
#define	SUBSCRIPTTOGGLE		0x2E00		/* ALT C */
#define	WORDERASEKEY			0x1200		/* ALT E */
#define	LINERASEKEY				0x2600		/* ALT L */
#define	TYPEMODE					0x1400		/* ALT T */
#define	SEARCHKEY				0x2100		/* ALT F */
#define  MARKERKEY				0x3200		/* ALT M */
#define  HARDPAGEKEY				0x2C00		/* ALT Z */
#define  AUTORETURNTOGGLE		0x1300		/* ALT R */

#define	SINGLE_LNSPACE_KEY  0x7800		/* ALT 1 */	
#define	ONEHLF_LNSPACE_KEY  0x7900		/* ALT 2 */	
#define	DOUBLE_LNSPACE_KEY  0x7A00		/* ALT 3 */	
#define	PROTECT_BLOCK_KEY	  0x7B00		/* ALT 4 */	
#define	LEFT_JUSTIFY_KEY    0x7C00		/* ALT 5 */	
#define	RM_JUSTIFY_KEY      0x7D00		/* ALT 6 */	
#define	CENTER_BLOCK_KEY    0x7E00		/* ALT 7 */	
#define	RFLUSH_BLOCK_KEY    0x7F00		/* ALT 8 */	
#define	PAR_INDENT_KEY  	  0x8000		/* ALT 9 */	

#define	FONTCHANGEKEY			0x3B00		/* F1 */
#define	HEADERKEY	   		0x3C00		/* F2 */
#define	FOOTERKEY   			0x3D00		/* F3 */
#define	FOOTNOTEKEY	   		0x3E00		/* F4 */
#define	ENDNOTEKEY  			0x3F00		/* F5 */

#define	COLUMNSKEY  			0x4400		/* F10 */
#define	NEXTCOLUMNKEY			0x5400		/* SHIFT F1 */
#define	PREVCOLUMNKEY			0x5500		/* SHIFT F2 */

#else

#define	BACKSPACEKEY			BACKSPACE_KEY	
#define	FINDKEY			      0x8766
#define	TABKEY		    		TAB_KEY
#define	INSERTTOGGLE			0x0169
#define	DELETEKEY				DELETE_KEY
#define	RIGHTCURSOR    		CURSRIGHT_KEY
#define	LEFTCURSOR     		CURSLEFT_KEY
#define	UPCURSOR					CURSUP_KEY
#define	DOWNCURSOR				CURSDOWN_KEY
#define	STOPCODEKEY				STOP_KEY
#define	AUTOCENTTOGGLE			CENTER_KEY
#define	BOLDTOGGLE				BOLD_KEY
#define	UNDERALLTOGGLE			UNDERLINE_KEY
#define	SUPERSCRIPTTOGGLE		0x0173
#define	SUBSCRIPTTOGGLE		0x0162
#define	WORDERASEKEY			WORDERASE_KEY
#define	LINERASEKEY				LINE_ERASE_KEY
#define	WORDLEFTKEY				WORDLEFT_KEY
#define	WORDRIGHTKEY 			WORDRIGHT_KEY
#define	TYPEMODE					WPTYPE_KEY
#define	SEARCHKEY				0x8873
#define  MARKERKEY				0x886D
#define  HARDPAGEKEY				PAGE_END_KEY
#define  AUTORETURNTOGGLE		AUTO_RETURN_KEY
#define 	UNDOKEY					0x8775
#define	SELECTOUTKEY			SELECT_OUT_KEY
#define	REQUIREDSPACEKEY		0x8720

#define	SINGLE_LNSPACE_KEY  	0x8831  /* these will change */
#define	ONEHLF_LNSPACE_KEY  	0x8832
#define	DOUBLE_LNSPACE_KEY  	0x8833
#define	PROTECT_BLOCK_KEY		0x8864
#define	LEFT_JUSTIFY_KEY		0x886C
#define	RM_JUSTIFY_KEY			0x8872
#define	CENTER_BLOCK_KEY		0x8863
#define	RFLUSH_BLOCK_KEY		0x8875
#define	PAR_INDENT_ON_KEY	  	INDENT_ON_KEY
#define	PAR_INDENT_OFF_KEY  	INDENT_OFF_KEY

#define	FONTCHANGEKEY			0x8778
#define	HEADERKEY	   		0x8868
#define	FOOTERKEY   			0x8834
#define	FOOTNOTEKEY	   		0x8835
#define	ENDNOTEKEY  			0x8836

#define	COLUMNSKEY  			0x8838
#define	NEXTCOLUMNKEY			NEXT_COL_KEY
#define	PREVCOLUMNKEY			PRIOR_COL_KEY

#define  REDRAWTHESCREEN		0x8772
#endif


typedef enum
{
	DONTMOVEFOCUS, MOVEFOCUS
} MOVEFLAG;

typedef enum
{
	PAGESEARCH, LINESEARCH
} ATTRIBUTEFIND;


#define	PCURRBLOCK		pdocpointer->dc_curpage->pg_curblock
#define	PBUFFHEAD		&PCURRBLOCK->bk_bfs
#define	CURR_ATTRIBS	pdocpointer->curr_attribs

/* #defines for editSearchAndReplace */

#define STRING_CASE_SENSITIVE 0
#define STRING_NOT_CASE_SENSITIVE 1
#define SEARCH_ATTRIBUTE 2
#define SEARCH_FORWARD 3
#define SEARCH_REVERSE 4
#define SEARCH_GLOBAL 5

#define HEADER 0
#define FOOTER 1
#define FOOTNOTE 2
#define ENDNOTE 3

#define SINGLEATTLENGTH 3
enum column_style { NEWSPAPER, PARALLEL };
enum pgnum_vert   { NO_PGNUM, TOP_OF_PG, BOTTOM_OF_PG };
enum pgnum_horz   { CENTER_OF_PG, L_MRGN, R_MRGN };
enum pgnum_direct { PG_FORWARD, PG_BACKWARD };
enum paper_sizes  { CUSTOM, LAND_SCAPE, LETTER, LEGAL, A3, A4, A5, B5 };
enum footnote_styles   { NOTE_NUMBERED, NOTE_LETTERED };
enum footnote_sep_line { NO_SEP_LINE, TWO_INCH, L_R_MRGN };

			/* The following #defines refer to pixels values for  */
			/* the 'canned' paper sizes. _H = height, _W = width. */

#define LETTER_W    2550		/* LETTER    = 8.5 * 11  in. */
#define LETTER_H    792

#define LANDSCAPE_W 3300		/* LANDSCAPE = 11 * 8.5  in. */
#define LANDSCAPE_H 612

#define LEGAL_W     2550		/* LEGAL     = 8.5 * 14  in. */
#define LEGAL_H     1008

#define A3_W        3508		/* A3        = 29.7 * 42 cm. */
#define A3_H        1189

#define A4_W        2480		/* A4        = 21 * 29.7 cm. */
#define A4_H        841

#define A5_W        1748		/* A5        = 14.8 * 21 cm. */
#define A5_H        594

#define B5_W        2079		/* B5        = 17.6 * 25 cm. */
#define B5_H        708

/* Punctuation marks */
#define QUOTE 	0x22
#define OP_PAREN '('
#define CL_PAREN ')'
#define OP_BRACKET '['
#define CL_BRACKET ']'
#define APOSTROPHE 0x27
#define EXCL_POINT '!'
#define COMMA ','
#define QUESTION '?'
#define SEMI_COLON ';'
#define COLON ':'

/* Punctuation rules */
#define PUNC_RULE_1 1
#define PUNC_RULE_2 2
#define PUNC_RULE_3 3
#define PUNC_RULE_4 4
#define PUNC_RULE_5 5
#define PUNC_RULE_6 6
#define PUNC_RULE_7 7
#define PUNC_RULE_8 8
#define PUNC_RULE_9 9
#define PUNC_RULE_10 10
#define PUNC_RULE_11 11
#define PUNC_RULE_12 12
#define PUNC_RULE_13 13
#define PUNC_RULE_14 14

#define NO_GRAMMAR_RIGHT_ERROR   0
#define SPELLING_ERROR			   15
#define WORD_ALERT_ERROR         16





/****************************************************************************
*
*		$Workfile:   editypes.h  $	- editor structures
*
*		$Revision:   1.6  $			$Modtime:   23 Jul 1991 14:15:10  $
*
*		$Author:   JeffS  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/editypes.h_v  $
* 
*    Rev 1.6   23 Jul 1991 14:17:34   JeffS
* Added endnote_height and changed type of some of the options variables.
* 
*    Rev 1.5   26 Jun 1991 11:32:58   JeffS
* Added the globals (tab_amount, max_xpos, etc.) to the document structure.
* 
*    Rev 1.4   22 Apr 1991 20:58:58   JeffS
* Added curr sysfont to the structure attstate - it can't be a global
* because probe then can't touch it when it really needs to in case
* a temporary structure was passed to probe.
* 
* 
*    Rev 1.3   22 Apr 1991 19:50:36   JeffS
* Added screen pos to the structure attstate.
* 
*    Rev 1.2   18 Apr 1991 15:04:26   JeffS
* Added plist structure and added plist and dc_path to doc structure.
* 
*    Rev 1.1   04 Apr 1991 10:08:24   JeffS
* Removed slink and dlink structures from here and put them
* back in types.h - they are not really editor types.
* 
*    Rev 1.0   03 Apr 1991 09:14:52   JeffS
* Initial revision.
*
*
*
****************************************************************************/

#define DEFPAGENUMTEXTLEN 35

/* Single link structure for MARKER-MARKER features */
typedef	struct	marklist
{
	struct	marklist	*next_page;
	PAGEID	page_id;
};

typedef struct tabs
{
	struct tabs		*tabs_chain;		/* pointer to next tab table structure */
	BLOCKTABID		tab_id;			/* ID for the tab table */
	SHORT				tabtable[NUMTABS];	/* array for tab positions and types */
} *TABS;

typedef struct buffer
{
	/* Chain field to next buffer. */
	struct dlink dlink;

	/* Counts of characters in each segment */
	SHORT		bf_sl,bf_st;

	/* The actual buffer containing text and attributes. Text that overflows */
	/* this buffer segment is added to a following buffer segment.           */
	UCHAR		bf_uctext[BF_SIZE];
} *BUFFER;

typedef struct pparam
{
	VOID *param;
	UCHAR x;
	BOOL  fwd;								/* Indicates direction of buffer movement */
	BUFFER  pbf;
} *PPARAM;

/* Special function types used in buffer manipulation */
#ifndef NOANSI
typedef VOID (*PROBEFN)( PPARAM );
typedef SHORT (*SKIPFN)( struct buffer **, UCHAR **, SHORT *, VOID * );
typedef SHORT (*MATCHFN)( struct bufhead *, UCHAR *, SHORT, SHORT, SKIPFN, VOID *);
typedef BOOL (*BLOCKFN)(struct doc *);
#else
typedef SHORT (*PROBEFN)();
typedef SHORT (*SKIPFN)();
typedef SHORT (*MATCHFN)();
typedef BOOL (*BLOCKFN)();
#endif
#define PNULL	0L

typedef struct probe
{
	PROBEFN fn;
	VOID *param;
} *PROBE;

typedef struct bufhead
{
	BUFFER	bh_pfocus;					/* buffer segment containing the focus */
	SHORT 	bh_fsplit;					/* Split value for the focus           */
} *BUFHEAD;

typedef struct blockatts
{
	/* ID for a linked list of tab structures which contain tab positions and types */
	BLOCKTABID		user_tabs;

	/* Mode flags that are applicable to this block, thus changing one */
	/* of these modes would create a new block */
	USHORT	bf_modes;
} *BLOCKATTS;

/* This structure is used only by MARKER-MARKER routines */
struct	markinfo
{
	SHORT					curr_pos;
	USHORT				page_number;
	USHORT				line_number;
	PAGEID				page_id;
	SHORT					sl;
	struct	bufhead	markbuf;
	struct	page		*markpage;
	struct	block		*markblock;

};

/*****************************************************************************
*	This is contained in the document as a linked list of font names. Font	  *
*	settings within the document refer to the position within the document 	  *
*	table of the font. There is a limit of 256 possible fonts available in 	  *
*	a document. 																				  *
*****************************************************************************/

typedef struct fonttable
{
	struct fonttable *next;				/* Chain of fonttable entries */
	UCHAR typeface[FONTNAMESIZE];		/* Name of the font */
	UCHAR point_size;						/* size */
	UCHAR	pitch;							/* pitch */
	SHORT fontflags;						/* flags for style */
	SYSFONTID sysfont;					/* system font ID */
	DOCFONTID docfont;					/* document font ID */
} *FONTTABLE;

typedef struct headers
{
	struct headers  *header_chain;   /* linked list of headers for future products */
	HEADERID			  header_id;		/* needed for multiple headers */
	struct doc      *header_doc;		/* pointer to a document structure containing header */
	SHORT				  header_size;		/* total bytes of header data */
	UCHAR				 *header_data;		/* pointer to straight ascii header data */
} *HEADERS;

typedef struct footers
{
	struct footers  *footer_chain;	/* linked list of footers for future products */
	FOOTERID			  footer_id;		/* needed for multiple footers */
	struct doc      *footer_doc;		/* pointer to a document structure containing footer */
	SHORT				  footer_size;		/* total bytes of footer data */
	UCHAR				 *footer_data;		/* pointer to straight ascii footer data */
} *FOOTERS;

typedef struct footnotes
{
	struct footnotes *footnote_chain;	/* linked list of footnotes */
	FOOTNOTEID			footnote_id;		/* needed for multiple footnotes */
	SHORT             footnote_height;	/* actual height of footnote on page */
	struct doc       *footnote_doc;		/* pointer to a document structure containing footnote */
	SHORT					footnote_size;		/* total bytes of footnote data */
	UCHAR				  *footnote_data;		/* pointer to straight ascii footnote data */
} *FOOTNOTES;

typedef struct endnotes
{
	struct endnotes  *endnote_chain;		/* linked list of endnotes */
	ENDNOTEID			endnote_id;			/* needed for multiple endnotes */
	SHORT             endnote_height;	/* actual height of endnote */
	struct doc       *endnote_doc;		/* pointer to a document structure containing endnote */
	SHORT					endnote_size;		/* total bytes of endnote data */
	UCHAR				  *endnote_data;		/* pointer to straight ascii endnote data */
} *ENDNOTES;

typedef struct columns
{
	struct columns *column_chain;
	COLUMNID			 column_id;
	UCHAR				 type_num_cols;	/* type is stored in bit 7-6, number in 0-5 */
	USHORT			 space_btwn;
	USHORT			 width_col;
} *COLUMNS;

typedef struct options
{
	SHORT				footnote_style;				
	SHORT				footnote_line;
	SHORT				endnote_style;				
	SHORT				endnote_line;			
	INT				pgnum_horiz;
	INT				pgnum_vert;
	USHORT			pgnum_start;
	USHORT			pgnum_end;
	USHORT			pgnum_begin;
	UCHAR				pgnum_text[DEFPAGENUMTEXTLEN];
} *OPTIONS;

typedef struct order
{
	PAGEID	page_id;
	BLOCKID	block_id;
} *ORDER;

typedef struct attstate
{
	SHORT		 curr_pos;		/* horizontal position of cursor in 300 dpi */ 
	SHORT		 screen_pos;	/* horizontal position of cursor in display widths */
	DOCFONTID curr_font;
	SYSFONTID curr_sysfont;
	BLOCKTABID curr_tabid;
	USHORT    mode_flags;
	USHORT	 probe_flags;
	USHORT	 line_number;
	struct doc	*curr_doc;
	SHORT		 cr_count;
	SHORT		 tab_amount;
	SHORT		 currently_in;
	SHORT		 curr_maxlineheight;
	SHORT		 max_xpos;
	SHORT		 reformat_flags;
	SHORT	  	 display_flags;
	SHORT	  	 display_pos1;
	SHORT	  	 display_pos2;
} *ATTSTATE;


/**************************************************************************
*  Internal representation for a document. This must contain sufficient 
*  information to map the document coordinates to coordinates of whatever 
*  display device is being used. 
**************************************************************************/

/* Information global to the entire document                             */

typedef struct docglobal 
{
	CHAR			dgchname[DOCNAMESIZE];
	FONTTABLE 	dgfonts;
	TABS		 	dgtabs;
	HEADERS	 	dgheaders;
	FOOTERS	 	dgfooters;
	FOOTNOTES	dgfootnotes;
	ENDNOTES	 	dgendnotes;
	COLUMNS	 	dgcolumns;
	OPTIONS		dgoptions;
} *DOCGLOBAL;

typedef struct doc
{
	/* Linked list of document structures. No ordering of documents is    */
	/* implied.                                                           */
	struct doc 	*dc_pdoc;

	struct docglobal dc_global;
	
	/* current page */
	struct page	*dc_curpage;

	/* current attributes for the document */
	struct attstate curr_attribs;

	/* page list for the document */
	struct plist *dc_plist;

	/* pathname for the document */
	CHAR dc_path[32];

} *DOC;

/**************************************************************************
* Page definition.
* This represents a page. Pages are linked together to form documents.
**************************************************************************/

typedef struct page
{
	/* Doubly linked list of pages. Page 1 precedes page 2 etc on the     */
	/* chain */
	struct dlink pg_dlink;

	/* Block which is first on the page by physical position. */
	/* The block with the smallest y position, x position */
	struct block  *pg_firstblock;

	/* Block containing the focus */
	struct block  *pg_curblock;

	/* Page identifier */
	PAGEID		pg_page_id;

	/* Margins applicable to this page.                                   */
	SHORT			pg_topm, pg_botm, pg_leftm, pg_rightm;			 
	SHORT			pg_paperlength, pg_paperwidth;

	/* ID's to the header and footer structures for this page */
	HEADERID			pg_header_id;
	FOOTERID			pg_footer_id;

	/* Byte of page attribute flags */
	UCHAR			pg_attribs;

	/* checkpoint information attributes */
	struct attstate pg_strt_attribs;

} *PAGE;

/**********************************************************************
* Text block structure.
* Structure used to group lines of text together. The line_segments
* inherit fonts from this structure. All blocks are assumed to be 
* disjoint, that is, no overlapping lines.  Blocks can be the maximum
* of one physical page and the minimum of one physical line.
**********************************************************************/

typedef struct block
{
	/* Linked list of blocks. There is no ordering implied by this list  */
	struct block *bk_chain;

	/* Block identifier */
	BLOCKID	bk_block_id;

	/* Block next and previous identifiers.  These variables can be used */
	/* to provide a method of ordering blocks.  There is a next block and */
	/* previous block ID along with a page ID for speed.  An ID may be -1 */
	/* in which case the block has no next/previous block. */
	struct order bk_next_id;
	struct order bk_prev_id;
	

	/* Block origin and width, relative to the start of the page.        */
	/* The height of the block is either fixed or variable. If variable, */
	/* the height increases with each line added. If fixed, text may be  */
	/* added to each block while the sum of the line heights <= the block  */
	/* height. Text that cannot be accomodated in the block is allowed to */
	/* overflow to another block.                                         */
	SHORT 	bk_ulx,bk_uly;
	SHORT 	bk_width,bk_height;

	/* Chain of buffer segments. These likewise are ordered. Segments con- */
	/* taining text in earlier lines precedes segments containing text in  */
	/* later lines.                                                        */
	struct bufhead bk_bfs; /**/

	/* Block	attributes such as margins, tab positions, and mode flags */
	struct blockatts bk_attribs;

	/* ID to the column structure for this block */
	COLUMNID	bk_col_data;

} *BLOCK;

/* single-link list of pages info ordered by page number. */
typedef struct plist
{
		struct plist *next;
		struct page *ppage;		/* page pointer or NULL if not in RAM */
		PAGEID id;
		BOOL new;					/* TRUE if page is new to the current edit session */
} *PLIST;

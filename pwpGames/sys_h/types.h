#ifndef TYPESH
#define TYPESH

/* Nothing in particular */

typedef char CHAR;
typedef short SHORT;
typedef int  INT;
typedef long LONG;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int  UINT;
typedef unsigned long ULONG;
typedef char *PCHAR;
typedef short *PSHORT;
typedef int *PINT;
typedef long *PLONG;
typedef USHORT UNITS,*PUNITS;
typedef LONG NCHARS;
typedef unsigned char BITUNIT;
typedef unsigned char DATAUNIT;
typedef void VOID;

typedef unsigned short SYSFONTID;
typedef unsigned char  DOCFONTID;
typedef unsigned short BLOCKTABID;
typedef short HEADERID;
typedef short FOOTERID;
typedef short FOOTNOTEID;
typedef short ENDNOTEID;
typedef short COLUMNID;
typedef long PAGEID;
typedef short BLOCKID;

typedef enum
{
	FALSE=0, TRUE=FALSE+1, MAYBE=TRUE+1
} BOOL;

typedef SHORT	SYSCOORD;
typedef USHORT	SYSKEY;
typedef USHORT SYSCOLOR;
typedef USHORT SYSWINTYPE;

#define REGISTER register

/* Single link structure */
typedef struct link
{
	struct link *link;
} *LINK;

/* Double link structure */
typedef struct dlink
{
	struct dlink 	*dl_n;
	struct dlink   *dl_p;
} *DLINK;

						/***** system configuration flags *****/

typedef struct sys_b
{											/* bit flags model type or features */	
	unsigned punctuation : 1;		/* punct on board */
	unsigned backlit     : 1;
	unsigned daisy       : 1; 
	unsigned thermal     : 1; 
	unsigned pwl         : 1; 		/* personal word list */
	unsigned walert      : 1; 		/* word alert */
	unsigned autosave    : 1;
	unsigned envelope    : 1;
	unsigned comm        : 1;
	unsigned conv_ascii  : 1;
	unsigned type        : 1;
	unsigned mouse       : 1; 
	unsigned proportional: 1; 
	unsigned sheetfeeder : 1; 
	unsigned hard_windows: 1;
	unsigned crt_unit    : 1;
	unsigned ml_unit     : 1;
	unsigned laptop_unit : 1;
	unsigned thesaurus   : 1;
	unsigned dflt_ruler  : 1;
	unsigned columns     : 1;
	unsigned             : 11;
};

typedef union system_flags
{
	struct sys_b sys_bits;
	LONG   all_system_flags;
};

#if 0
typedef struct curscb
{
	SHORT page, x, y, start, end;
} *CURSCB;
#endif
#endif

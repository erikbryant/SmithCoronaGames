#define		EMAL_LIST_LEN			15

INT fr_no, max_fr;
INT last_fn;
INT last_fr;
INT sort_no;
INT l_filecnt;
INT d_filecnt;
INT slot[3];
INT merge_order[40];
INT merge_max;
INT plabmrg_order[40];

BOOL ml_exists,printing,disking,sorting,merging;
BOOL doc_saved,list_saved, sorted;
BOOL list_mode,sort_mode,disk_mode,print_mode,merge_mode,doc_mode,select_mode;
BOOL Running, printing_now, mixed_merge, entire_merge, print_label;
BOOL full_merge, selctv_merge, cond_merge, label_option;
BOOL doc_is, mlist_is;
BOOL am_active, doc_window_enabled;
BOOL save;
BOOL recall;
BOOL erase;
BOOL rename;


char *lmfn_ptr;
char *dmfn_ptr;
char rec_no[10];
char * current_list_filename;
char *default_list_filename;
char *l_merge_filename, *d_merge_filename;

#define DOWNLOAD 0
#define AM_PDOCNAME ":\\Document\\Test.doc"
#define AM_SUSPEND 1
#define AM_DISK_SUCCESS 0
#define AM_NORMAL 0
#define AM_RDONLY 1
#define MAX_SIZE 50
#define MAIL_LIST_MAX 	MAX_SIZE-1
#define CODE_X 9			  /* temporary def */
#define SELECT_X 9
#define COND_DATA 0
#define FIELD_DATA 1
#define LESS_THAN 0
#define GREATER_THAN 1



struct sysmsg amrcvmsg;



struct LABEL
{
	char lab[16];
};


struct DATA
{
   char dat[EMAL_LIST_LEN][41];
};

typedef struct MAILING_LIST_DATA
{
int cfr;     /* current frame no being edited  */
int mfr;		 /* max frame no edited            */
int clab;	 /* current label slot number      */
int mlab;	 /* max label field slot valid     */      
int cdat;    /* current data slot being edited */
int mdat;    /* max data slot being edited     */
};

struct MAILING_LIST
{
int cfr;     /* current frame no  */
int cfn;		 /* ??????? dont need??  */
int mfn;		 /* last label field used */      
int mfr;		 /* max frame  edited  */
int mlab;	 /* max label field used */      
int lfr;	    /* last frame  edited  */
int size;
char filename[13]; /* was char fname; */
struct LABEL label[EMAL_LIST_LEN]; 
struct DATA data[MAIL_LIST_MAX];
};



/* old style 
struct MLIST 
 {			  
char  labels[EMAL_LIST_LEN][15];
char data[40][EMAL_LIST_LEN][15];
int  lfr;
 }current;
***/		  

/***
struct MALIST 
 {			  
char labels[EMAL_LIST_LEN][16];
char data[EMAL_LIST_LEN][41];
 }malist[5];
***/		  

struct COND_MERGE
{
char *field;
char *oper;
char *data;
}c_merge[3];

/*  Will replace including doedit.p in amdoc.c      */
short  bufSkipFwd(struct  buffer * *ppbf,unsigned char  * *ppuch,short  *pciInseg,struct  probe *skippm);

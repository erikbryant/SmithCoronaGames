/****************************************************************************
*
*		$Workfile:   psos.h  $	- pSOS+ symbol definitions.
*
*		$Revision:   1.1  $			$Modtime:   25 Feb 1991 16:29:04  $
*
*		$Author:   ajw  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/screen/windows/psos.h_v  $
* 
*    Rev 1.1   26 Feb 1991 08:21:02   ajw
* This is just to unlock this file.
* 
*    Rev 1.0   08 Feb 1991 12:04:28   rwh
* Initial revision.
*
*
*
****************************************************************************/
/**************************************************************/ 
/*                                                            */ 
/*   pSOS+/68K VERSION 1.0 CONSTANT AND SYMBOL DEFINITIONS    */
/*                                                            */ 
/*                        12/06/89                            */
/*                                                            */ 
/*      COPYRIGHT 1988, SOFTWARE COMPONENTS GROUP, INC.       */ 
/*                   ALL RIGHTS RESERVED.                     */ 
/*                                                            */ 
/*   Permission is hereby granted to licensees of Software    */ 
/*   Components Groups products to use or abstract this       */
/*   computer program for the sole purpose of implementing    */ 
/*   a product based on Software Components Group products.   */ 
/*   No other rights to reproduce, use, or disseminate this   */ 
/*   program whether in part or in whole, is granted.         */
/*                                                            */ 
/*   Software Components Group Inc. makes no representation   */ 
/*   or warranties with respect to the performance of this    */ 
/*   computer program, and specifically disclaims any res-    */ 
/*   ponsibilities for any damages, special or consequential, */ 
/*   connected with its use.                                  */ 
/*                                                            */ 
/**************************************************************/ 

/*-------------------------------------------------------------------*/ 
/*  Bit definitions T_CREATE														*/
/*-------------------------------------------------------------------*/ 
#define T_EXPORT 0x00000001   /* Export bit */
#define T_FPU    0x00000001   /* Using FPU bit */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for T_START, T_MODE and AS_CATCH						*/
/*-------------------------------------------------------------------*/ 
#define T_NOPREEMPT 0x00000001   /* Not preemptible bit */
#define T_TSLICE    0x00000002   /* Time-slicing enabled bit */
#define T_NOASR     0x00000004   /* ASRs disabled bit */
#define T_SUPV      0x00002000   /* Supervisor mode bit */
#define T_LEVELMASK 0x00000700   /* Interrupt level mask bits */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for RN_CREATE												*/
/*-------------------------------------------------------------------*/ 
#define RN_EXPORT  0x00000001  /* Export bit */
#define RN_PRIOR   0x00000002  /* Queue by priority bit */
#define RN_DO      0x00000004  /* Delete override bit */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for RN_GETSEG												*/
/*-------------------------------------------------------------------*/ 
#define RN_NOWAIT  0x00000001 /* Don't wait */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for PT_CREATE												*/
/*-------------------------------------------------------------------*/ 
#define PT_EXPORT 0x00000001  /* Export bit */
#define PT_DO     0x00000004  /* Delete override bit */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for Q_CREATE													*/
/*-------------------------------------------------------------------*/ 
#define Q_EXPORT  0x00000001  /* Export */
#define Q_PRIOR   0x00000002  /* queue by priority */
#define Q_LIMIT   0x00000004  /* message queue limit */
#define Q_PRIBUF  0x00000008  /* private buffers */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for Q_RECEIVE												*/
/*-------------------------------------------------------------------*/ 
#define Q_NOWAIT  0x00000001  /* Don't wait */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for EV_RECEIVE												*/
/*-------------------------------------------------------------------*/ 
#define EV_NOWAIT 0x00000001  /* Don't wait */
#define EV_OR     0x00000002  /* Wait for any bit */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for SM_CREATE												*/
/*-------------------------------------------------------------------*/ 
#define SM_EXPORT  0x00000001 /* Export */
#define SM_PRIOR   0x00000002 /* queue by priority */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for SM_P														*/
/*-------------------------------------------------------------------*/ 
#define SM_NOWAIT  0x00000001 /* Don't wait */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for K_FATAL													*/
/*-------------------------------------------------------------------*/ 
#define K_EXPORT  0x00000001  /* Export */

/*-------------------------------------------------------------------*/ 
/*  Bit definitions for MM_L2P, MM_PMAP, and MM_SPROTECT					*/
/*-------------------------------------------------------------------*/ 
#define MM_WPROTECT 0x00000100  /* Write protect */
#define MM_NOCACHE  0x00000200  /* Disable data caching */
#define MM_NOACCESS 0x00000400  /* Deny access */

/*-------------------------------------------------------------------*/ 
/*  pSOS service call definitions												*/
/*-------------------------------------------------------------------*/ 
unsigned long t_create(), t_ident(), t_delete(), t_start(), t_restart();
unsigned long t_setpri(), t_suspend(), t_resume(), t_setreg(), t_getreg();
unsigned long t_mode(), t_ident();
unsigned long ev_send(), ev_receive();
unsigned long q_create(), q_ident(), q_delete();
unsigned long q_send(), q_urgent(), q_receive(), q_broadcast();
unsigned long as_send(), as_catch(), as_return();
unsigned long sm_create(), sm_ident(), sm_delete(), sm_p(), sm_v();
unsigned long tm_set(), tm_get(), tm_wkafter(), tm_wkwhen(), tm_evafter();
unsigned long tm_evwhen(), tm_evevery(), tm_cancel(), tm_tick();
unsigned long rn_create(), rn_ident(), rn_delete(), rn_getseg(), rn_retseg();
unsigned long pt_create(), pt_ident(), pt_delete(), pt_getbuf(), pt_retbuf();
unsigned long mm_l2p(), mm_p2l(), mm_pread(), mm_pwrite();
unsigned long mm_pmap(), mm_unmap(), mm_sprotect();

/*-------------------------------------------------------------------*/ 
/*  I/O service call definitions													*/
/*-------------------------------------------------------------------*/ 
unsigned long de_init(), de_open(), de_close();
unsigned long de_read(), de_write(), de_cntrl();


/*********************************************************************/ 
/*  Timer structures                                                 */
/*********************************************************************/ 
struct t_date        {
        unsigned int  year;  /* Year, A.D. */
        unsigned char month; /* Month, 1->12 */
        unsigned char day;   /* Day, 1->31 */
};

struct t_time        {
        unsigned int  hour;    /* Hour, 0->23 */
        unsigned char minute;  /* Minute, 0->59 */
        unsigned char second;  /* Second, 0->59 */
};

struct time_ds        {
        struct t_date date;    /* Date */
        struct t_time time;    /* Time */
        unsigned long ticks;   /* Current Elapsed Ticks Between Seconds */
};

/*********************************************************************/ 
/*  pSOS Error Codes                                                 */
/*********************************************************************/ 

/*-------------------------------------------------------------------*/ 
/*  Fatal errors during startup													*/
/*-------------------------------------------------------------------*/ 
#define FAT_ALIGN    0x0F00   /* DATA AREA MISALIGNED (4; 16 FOR MMU) */
#define FAT_OVSDA    0x0F01   /* DATA AREA EXHAUSTED: SYSTEM DATA AREA */
#define FAT_OVOBJT   0x0F02   /* DATA AREA EXHAUSTED: OBJECT TABLE */
#define FAT_OVDDAT   0x0F03   /* DATA AREA EXHAUSTED: DEV DATA AREA TABLE */
#define FAT_OVTCB    0x0F04   /* DATA AREA EXHAUSTED: TASK STRUCTURES */
#define FAT_OVQCB    0x0F05   /* DATA AREA EXHAUSTED: QUEUE STRUCTURES */
#define FAT_OVSMCB   0x0F06   /* DATA AREA EXHAUSTED: SEMAPHORES */
#define FAT_OVTM     0x0F07   /* DATA AREA EXHAUSTED: TIMER STRUCTURES */
#define FAT_OVPT     0x0F08   /* DATA AREA EXHAUSTED: PARTITIONS */
#define FAT_OVRPC    0x0F09   /* DATA AREA EXHAUSTED: RPC STRUCTURES */
#define FAT_OVRN     0x0F0A   /* DATA AREA EXHAUSTED: REGION STRUCTURES */
#define FAT_OVMM     0x0F0B   /* DATA AREA EXHAUSTED: MMU STRUCTURES */
#define FAT_ROOT     0x0F0C   /* CANNOT CREATE ROOT TASK. */
#define FAT_CHKSUM   0x0F0E   /* CHECKSUM ERROR */
#define FAT_INVCPU   0x0F0F   /* WRONG PROCESSOR TYPE */
#define FAT_MMUSET   0x0F10   /* MMU ALREADY ENABLED WITH BAD SETTING */
#define FAT_R0UNIT   0x0F11   /* REGION 0 UNIT SIZE MUST MATCH MMU */

/*-------------------------------------------------------------------*/ 
/*  Fatal errors during run-time													*/
/*-------------------------------------------------------------------*/ 
#define FAT_ILLPKT   0x0F12   /* PKT_TYPE OUT OF RANGE IN THE RECVD PKT.*/
#define FAT_MIVERIF  0x0F13   /* MULTI-NODE CONFIG TABLE PARAMETERS */
                              /*   DOESN'T MATCH WITH VERIFY PKT RECVD. */
#define FAT_OBJTENT  0x0F14   /* GLOBAL OBJECT ID DOES NOT MATCH WITH */
                              /*   ID IN THE ENTER SYMBOL PACKET. */

/*-------------------------------------------------------------------*/ 
/*  Common errors (across several service groups)							*/
/*-------------------------------------------------------------------*/ 
#define ERR_TIMEOUT  1        /* TASK TIMED OUT WAITING FOR RESOURCE */
#define ERR_UNIMPL   2        /* UNIMPLEMENTED SYSTEM SERVICE */
#define ERR_SSFN     3        /* ILLEGAL SYSTEM SERVICE FUNCTION NUMBER */
#define ERR_NODENO   4        /* ILLEGAL NODE NUMBER */
#define ERR_OBJDEL   5        /* OBJECT HAS BEEN DELETED */
#define ERR_OBJID    6        /* ILLEGAL/INVALID OBJECT ID */
#define ERR_OBJTYPE  7        /* INCORRECT OBJECT TYPE */
#define ERR_OBJTFULL 8        /* OBJECT TABLE FULL */
#define ERR_OBJNF    9        /* OBJECT NOT FOUND */

/*-------------------------------------------------------------------*/ 
/*  Task service group errors														*/
/*-------------------------------------------------------------------*/ 
#define ERR_NOTCB    14       /* CANNOT CREATE - OUT OF TCBs */
#define ERR_NOSTK    15       /* CANNOT CREATE - NO STACK SPACE */
#define ERR_TINYSTK  16       /* CANNOT CREATE - STACK TOO SMALL */
#define ERR_PRIOR    17       /* CANNOT CREATE - PRIORITY OUT OF RANGE */
#define ERR_ACTIVE   18       /* CANNOT START - ALREADY ACTIVE */
#define ERR_NACTIVE  19       /* CANNOT RESTART - NEVER STARTED */
#define ERR_SUSP     20       /* CANNOT SUSPEND - ALREADY SUSPENDED */
#define ERR_NOTSUSP  21       /* CANNOT RESUME - NOT SUSPENDED */
#define ERR_SETPRI   22       /* CANNOT CHANGE PRIORITY - NEW PRIORITY */
                              /*  OUT OF RANGE */
#define ERR_REGNUM   23       /* ILLEGAL TASK REGISTER NUMBER */
#define ERR_FOPEN    24       /* CANNOT DELETE - FILES OPEN */
 
/*-------------------------------------------------------------------*/ 
/*  Region service group errors													*/
/*-------------------------------------------------------------------*/ 
#define ERR_RNADDR   27       /* CANNOT CREATE - START ADDR NOT ON LONG */
                                      /*  WORD OR MMU PAGE */
#define ERR_UNITSIZE 28       /* CANNOT CREATE - UNIT SIZE NOT POWER OF */
                                      /*  2 OR LESS THAN 4 */
#define ERR_TINYUNIT 29       /* CANNOT CREATE - LENGTH TOO LARGE FOR */
                                      /*  GIVEN UNIT SIZE */
#define ERR_TINYRN   30       /* CANNOT CREATE - REGION LENGTH TOO */
                                      /*  SMALL FOR REQ'D RNCB */
#define ERR_SEGINUSE 31       /* CANNOT DELETE - ONE OR MORE SEG IN USE */
#define ERR_ZERO     32       /* CANNOT GETSEG - REQUEST SIZE IS ZERO */
#define ERR_TOOBIG   33       /* CANNOT GETSEG - REQUEST SIZE TOO BIG */
                              /* FOR REGION */
#define ERR_NOSEG    34       /* CANNOT GETSEG - NOT ENOUGH MEMORY */
#define ERR_NOTINRN  35       /* CANNOT RETSEG - SEG DOESN'T BELONG TO */
                              /* REGION */
#define ERR_SEGADDR  36       /* CANNOT RETSEG - INCORRECT SEG START ADDR */
#define ERR_SEGFREE  37       /* CANNOT RETSEG - SEGMENT ALREADY FREE */
#define ERR_RNKILLD  38       /* CANNOT GETSEG - REGION DELETED WHILE */
                              /* WAITING */
#define ERR_TATRNDEL 39       /* INFORMATIVE - TASKS WERE WAITING AT */
                              /* DELETION */
 
/*-------------------------------------------------------------------*/ 
/*  Partition service group errors												*/
/*-------------------------------------------------------------------*/ 
#define ERR_PTADDR   40       /* CANNOT CREATE - START ADDR NOT ON LONG */
                                      /*  PAGE WORD OR MMU */
#define ERR_BUFSIZE  41       /* CANNOT CREATE - BUF SIZE NOT POWER OF */
                                      /*  2, OR LESS THAN 4 */
#define ERR_TINYPT   42       /* CANNOT CREATE - LENGTH TOO SMALL FOR */
                              /* NECESSARY PTCB */
#define ERR_BUFINUSE 43       /* CANNOT DELETE-ONE OR MORE BUFFS IN USE */
#define ERR_NOBUF    44       /* CANNOT GETBUF-NO FREE BUFFS AVAILABLE. */
#define ERR_BUFADDR  45       /* CANNOT RETBUF-INCORRECT BUFF START ADDR*/
#define ERR_BUFFREE  47       /* CANNOT RETBUF-BUFFER ALREADY FREE */
 
/*-------------------------------------------------------------------*/ 
/*  Queue/Message service group errors											*/
/*-------------------------------------------------------------------*/ 
#define ERR_NOQCB    51       /* CANNOT CREATE - NO MORE QCB */
#define ERR_NOMGB    52       /* CANNOT CREATE OR SEND - NO MORE MESSAGE*/
                              /* BUFFERS */
#define ERR_QFULL    53       /* CANNOT SEND - MESSAGE QUEUE FULL */
#define ERR_QKILLD   54       /* CANNOT RECEIVE - QUEUE DELETED WHILE */
                              /* WAITING. */
#define ERR_NOMSG    55       /* CANNOT RECEIVE - NO PENDING MESSAGE */
                              /* (NOWAIT ONLY) */
#define ERR_TATQDEL  56       /* INFORMATIVE - AT TIME OF DELETION */
                              /* TASKS WERE WAITING */
#define ERR_MATQDEL  57       /* INFORMATIVE - AT TIME OF DELETION */
                              /* MESSAGES WERE PENDING */
 
/*-------------------------------------------------------------------*/ 
/*  Event service group errors													*/
/*-------------------------------------------------------------------*/ 
#define ERR_NOEVS    60       /* NOT ALL WANTED EVENTS WERE PENDING */
                              /* (NO WAIT ONLY) */

/*-------------------------------------------------------------------*/ 
/*  Asynch signal service group errors											*/
/*-------------------------------------------------------------------*/ 
#define ERR_NOTINASR 62       /* CANNOT RETURN - NOT IN ASR */
#define ERR_NOASR    63       /* CANNOT SEND - TASK HAS NO VALID ASR */
 
/*-------------------------------------------------------------------*/ 
/*  Semaphore service group errors												*/
/*-------------------------------------------------------------------*/ 
#define ERR_NOSCB    65       /* CANNOT CREATE - NO MORE SCB */ 
#define ERR_NOSEM    66       /* CANNOT ACQUIRE - SEMAPHORE NOT AVAIL */
                              /* (NOWAIT) */
#define ERR_SKILLD   67       /* CANNOT ACQUIRE - SEMAPHORE DELETED */
                              /* WHILE WAITING */
#define ERR_TATSDEL  68       /* INFORMATIVE - AT TIME OF DELETION */
                              /* TASKS WERE WAITING */
 
/*-------------------------------------------------------------------*/ 
/*  Time service group errors														*/
/*-------------------------------------------------------------------*/ 
#define ERR_NOTIME   71       /* TIME OF DAY HAS NOT BEEN SET YET */
#define ERR_ILLDATE  72       /* DATE INPUT OUT OF RANGE */
#define ERR_ILLTIME  73       /* TIME OF DAY INPUT OUT OF RANGE */
#define ERR_ILLTICKS 74       /* TICKS INPUT OUT OF RANGE */
#define ERR_NOTIMERS 75       /* NO TIMERS LEFT */
#define ERR_BADTMID  76       /* INVALID TIMER ID SPECIFIED */
#define ERR_TMNOTSET 77       /* CANNOT CANCEL - TIMER NOT SET */
#define ERR_TOOLATE  78       /* TIMING REQ TOO LATE- TIME ALREADY PAST */
 
/*-------------------------------------------------------------------*/ 
/*  Multiprocessor support service group errors								*/
/*-------------------------------------------------------------------*/ 
#define ERR_ILLRPC   83       /* THIS SYSTEM SERVICE CALL NOT SUPPORTED */
                              /* AS RPC */
#define ERR_NOAGNT   84       /* CANNOT WAIT ON REMOTE OBJ - NO FREE */
                              /* AGENTS AT NODE */
#define ERR_AGTBLKD  85       /* AGENT BLOCKED. THIS IS NOT AN ERROR. */

/*-------------------------------------------------------------------*/ 
/*  MMU support group errors														*/
/*-------------------------------------------------------------------*/
#define ERR_PALIGN   90       /* PADDR NOT PAGE-ALIGNED */
#define ERR_LALIGN   91       /* LADDR NOT ON SECTION BOUNDARY */
#define ERR_UNMAPPED 92       /* LOGICAL ADDRESS IS NOT MAPPED */
#define ERR_SUPER    93       /* CANNOT AFFECT SUPERVISOR MAP */
#define ERR_TOOMUCH  94       /* COPY TOO LONG - PAST SECTION END */
#define ERR_MAPPED   95       /* SECTION ALREADY MAPPED */
#define ERR_NOSECT   96       /* NO SECTION AVAILABLE */
#define ERR_ZEROMAP  97       /* ZERO MEMORY TO BE MAPPED */
#define ERR_TOOLONG  98       /* LENGTH GREATER THAN SECTION */
#define ERR_SWITCH   99       /* ILLEGAL MODE SWITCH */

/*-------------------------------------------------------------------*/ 
/*  IO service group errors														*/
/*-------------------------------------------------------------------*/
#define ERR_IODN     0x0101   /* ILLEGAL DEVICE (MAJOR) NUMBER */
#define ERR_NODR     0x0102   /* NO DRIVER PROVIDED */
#define ERR_IOOP     0x0103   /* ILLEGAL I/O OPERATION NUMBER */

/* END OF PSOS.H */             


/****************************************************************************
*
*		$Workfile:   msg.h  $	- pSOS messaging header file
*
*		$Revision:   1.109  $			$Modtime:   22 Jan 1992 14:04:24  $
*
*		$Author:   PaulC  $
*
*
*
*	(C) Copyright 1990-1992 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/msg.h_v  $
* 
*    Rev 1.109   22 Jan 1992 14:04:52   PaulC
* added IGNOREKEYS
* 
*    Rev 1.108   14 Jan 1992 09:37:16   WayneM
* added DEMO_UP.
* 
*    Rev 1.107   03 Jan 1992 13:36:50   PaulC
* added lang_switch
* 
*    Rev 1.106   04 Dec 1991 11:35:20   PaulC
* LOW
* 
*    Rev 1.105   01 Dec 1991 17:58:16   ChrisA
* added PCONFIG and PSERIAL for new laptop driver dialogs
* 
*    Rev 1.104   08 Nov 1991 08:49:42   PaulC
* added charge
* 
*    Rev 1.103   05 Nov 1991 09:32:22   TracyK
* 
*    Rev 1.102   05 Nov 1991 08:52:46   TracyK
* added CHARGE_BATTERY to hssb switches
* 
*    Rev 1.101   24 Oct 1991 11:14:24   ChrisA
* printer enum added for fax support
* 
*    Rev 1.100   22 Oct 1991 14:10:12   TracyK
* moved mousedown and mouseup to editor switches
* 
*    Rev 1.99   22 Oct 1991 14:07:46   TracyK
* added MOUSEDOWN and MOUSEUP to keyboard messages
* 
*    Rev 1.98   21 Oct 1991 15:14:34   JimK
* Added disk cache on and off
* 
*    Rev 1.97   17 Oct 1991 15:06:52   WayneM
* envelope added in commands.
* 
*    Rev 1.96   17 Oct 1991 10:10:40   PaulC
* ENGLISH, METRIC
* 
*    Rev 1.95   11 Oct 1991 11:11:40   MikeF
* Added Fax Phone Directory Equates
* 
*    Rev 1.94   02 Oct 1991 14:11:52   WayneM
* ENDNOTEOPTIONS added
* 
*    Rev 1.93   25 Sep 1991 01:31:48   PaulC
* 
*    Rev 1.92   24 Sep 1991 10:20:24   TracyK
* added MOUSE_ON and MOUSE_OFF to hssb switches
* 
*    Rev 1.91   22 Sep 1991 23:16:06   PaulC
* APPL
* 
*    Rev 1.90   19 Sep 1991 14:17:28   JimK
* Added disk text in memory
* 
*    Rev 1.89   18 Sep 1991 15:22:04   PaulC
* 
*    Rev 1.88   18 Sep 1991 01:00:50   PaulC
* SHOWAPPL
* 
*    Rev 1.87   17 Sep 1991 23:10:34   PaulC
* downgo
* 
*    Rev 1.86   17 Sep 1991 15:30:26   PaulC
* No change.
* 
*    Rev 1.85   18 Sep 1991 15:07:46   JohnC
* No change.
* 
*    Rev 1.84   18 Sep 1991 11:53:10   JohnC
* Added FAX EQUATE.
* 
* 
*    Rev 1.83   18 Sep 1991 11:46:12   PaulC
* misc download switches
* 
*    Rev 1.82   12 Sep 1991 12:46:56   TracyK
* changed HALT_KEYS to STOP_KEYS
* 
*    Rev 1.81   12 Sep 1991 11:57:30   TracyK
* added HALT_KEYS to hssb switches
* 
*    Rev 1.80   11 Sep 1991 16:39:08   JimK
* Added PEOC for printer functions, VOLUME and MOUNTED for disk
* 
*    Rev 1.79   11 Sep 1991 15:51:20   PaulC
* suspallbutclip,restallbutclip
* 
*    Rev 1.78   11 Sep 1991 11:35:36   JohnC
* 
* Added an equate for FAX_VIEW_STATUS_MENU_CHK
* 
* 
*    Rev 1.77   10 Sep 1991 16:46:46   JohnC
* 
* Added FAX equate.
* 
* 
*    Rev 1.76   06 Sep 1991 14:38:00   Guest
* No change
* 
*    Rev 1.75   02 Sep 1991 10:52:56   PaulC
* TYPEEXIT, TYPEFAIL
* 
*    Rev 1.74   31 Aug 1991 19:41:00   JohnC
* 
*    Rev 1.73   28 Aug 1991 21:39:42   JohnC
* 
*    Rev 1.72   28 Aug 1991 21:17:12   JohnC
* Added an equate for send status menu.
* 
* 
*    Rev 1.71   26 Aug 1991 14:42:36   PaulC
* No change.
* 
*    Rev 1.70   27 Aug 1991 08:54:16   PaulC
* exec
* 
*    Rev 1.69   23 Aug 1991 09:27:44   JohnC
* Added a fax equate.
* 
* 
*    Rev 1.68   21 Aug 1991 14:57:12   PaulC
* DOWNDOCSAVE
* 
*    Rev 1.67   20 Aug 1991 15:04:08   PaulC
* Added repeat
* 
*    Rev 1.66   15 Aug 1991 19:32:18   WayneM
* Search & Replace msgs.
* 
*    Rev 1.65   12 Aug 1991 16:45:46   TracyK
* added START_KEYS to keyboard messages
* 
*    Rev 1.64   11 Aug 1991 01:17:44   PaulC
* execTStatMsg
* 
*    Rev 1.63   04 Aug 1991 13:58:04   SueB
* added printer control codes DIAGS, PSTOPCODE
* 
*    Rev 1.62   01 Aug 1991 09:05:22   PaulC
* new exec switches
* 
*    Rev 1.61   31 Jul 1991 13:03:18   JimK
* Added disk replace attention return message
* 
*    Rev 1.60   01 Aug 1991 10:43:48   PaulC
* Added susp all
* 
*    Rev 1.59   30 Jul 1991 10:55:22   JimK
* Added disk start save dialog message
* 
*    Rev 1.58   29 Jul 1991 22:03:40   PaulC
* added help
* 
*    Rev 1.57   25 Jul 1991 21:02:42   PaulC
* added demodone
* 
*    Rev 1.56   25 Jul 1991 14:20:02   WayneM
* Added BOLD, UCASE, UNDLN
* 
*    Rev 1.55   22 Jul 1991 14:42:12   JimK
* Added convert to/from messages for disk task
* 
*    Rev 1.54   22 Jul 1991 14:22:54   PaulC
* Added editor redisplay
* 
*    Rev 1.53   21 Jul 1991 18:46:44   JimK
* Added some disk dialog messages and DOCJOIN
* 
*    Rev 1.52   18 Jul 1991 23:51:08   WayneM
* Added CLRMEM.
* 
*    Rev 1.51   18 Jul 1991 15:34:34   PaulC
* added qemptynowait
* 
*    Rev 1.50   18 Jul 1991 14:44:38   rwh
* No change.
* 
*    Rev 1.49   19 Jul 1991 10:18:36   ChrisA
* Added pritner defines for Diagnostics and End of Page (PDIAGS,PEOP)
* 
*    Rev 1.48   18 Jul 1991 10:03:48   PaulC
* Added QEMPTY
* 
*    Rev 1.47   17 Jul 1991 14:30:12   WayneM
* Added CONV2ASC, CONV2SC
* 
*    Rev 1.46   16 Jul 1991 15:55:28   JohnC
* Added fax application equates.
* 
* 
*    Rev 1.45   15 Jul 1991 17:02:08   WayneM
* KBIIDOWN
* 
*    Rev 1.44   12 Jul 1991 03:40:56   ChrisA
* add printer switch for psetup.  This is internal to the printer code and
* is not necessary for other tasks to recompile.... i think.
* 
*    Rev 1.43   11 Jul 1991 14:58:36   PaulC
* Added exec downloadable switches
* 
*    Rev 1.42   10 Jul 1991 13:55:36   TracyK
* added SUSPEND_RESUME & UNSUSPEND RESUME
* 
*    Rev 1.41   04 Jul 1991 16:15:42   WayneM
* Added PUNCT, DICT, & WALERT ERROR commands.
* 
*    Rev 1.40   04 Jul 1991 12:40:08   PaulC
* Added exec downloadable messages
* 
*    Rev 1.39   01 Jul 1991 11:28:04   PaulC
* Added FILENAME and FILENONE
* 
*    Rev 1.38   01 Jul 1991 11:21:30   TracyK
* added CHANGE_REPEAT_SPEED to hssb switches for type mode
* 
*    Rev 1.37   25 Jun 1991 14:25:48   WayneM
* Added PSTAT to printer command enum.
* 
*    Rev 1.36   21 Jun 1991 13:49:58   TracyK
* added CHANGE_MOUSE_POLL and CHANGE_KEYBD_POLL to hssb commands
* 
*    Rev 1.35   21 Jun 1991 11:51:06   WayneM
* Added WALERT & PUNCT.
* 
*    Rev 1.34   20 Jun 1991 11:18:42   WayneM
* KBII added to commands.
* 
*    Rev 1.33   17 Jun 1991 16:48:18   WayneM
* Added REDRAWSCRN for disk.
* 
*    Rev 1.32   10 Jun 1991 15:52:40   ChrisA
* Added PRINTER messge for print task.
* 
*    Rev 1.31   31 May 1991 19:48:06   WayneM
* Added foot/endnote options.
* 
*    Rev 1.30   31 May 1991 09:26:30   WayneM
* Added DIRNAME.
* 
*    Rev 1.29   26 May 1991 18:40:10   ChrisA
* added IMPRESSION message for print!
* This is toggled through using code + w
* 
*    Rev 1.28   24 May 1991 10:08:26   PaulC
* Added enums and sorted applications
* 
*    Rev 1.27   21 May 1991 10:18:34   JimK
* Restored new disk messages
* 
*    Rev 1.26   19 May 1991 22:24:08   ChrisA
* More message added for print task.
* 
*    Rev 1.25   18 May 1991 17:16:48   ajw
* No change.
* 
*    Rev 1.24   18 May 1991 20:01:06   WayneM
* Backed up one revision. 1.23 caused system problems. JAM
* 
* 
* 
*    Rev 1.22   16 May 1991 09:49:00   WayneM
* Added Reference menu bar #defines.
* 
*    Rev 1.21   13 May 1991 16:24:08   WayneM
* Added DOCOPEN & DOCSAVE
* 
*    Rev 1.20   13 May 1991 11:17:44   WayneM
* Added FONTCHG
* 
*    Rev 1.19   10 May 1991 13:52:10   PaulC
* suspLeft
* 
*    Rev 1.18   10 May 1991 10:25:38   PaulC
* SUSPRIGHT
* 
*    Rev 1.17   07 May 1991 08:16:16   TracyK
* added execute and goto to list of command enum
* 
*    Rev 1.16   02 May 1991 09:27:32   TracyK
* Added DEFINE_RESUME and UNDEFINE resume to command enum as commands to
* send to keyboard
* 
*    Rev 1.15   26 Apr 1991 14:14:14   JimK
* Removed several disk messages enum's and added all the disk _S enum's.
* 
* 
*    Rev 1.14   25 Apr 1991 10:31:02   TracyK
* added beep commands to command enum
* 
*    Rev 1.13   24 Apr 1991 08:22:36   TracyK
* added defines for COMMAND_MASK, ACTION_MASK and DRIVER_MASK
* 
* 
*    Rev 1.12   22 Apr 1991 13:34:52   SteveP
* Added Comm driver messages.
* 
* 
*    Rev 1.11   19 Apr 1991 13:19:34   JimK
* Added disk messages.
* 
* 
*    Rev 1.10   19 Apr 1991 09:37:58   PaulC
* Added new msgs.
* 
*    Rev 1.9   11 Apr 1991 16:17:26   PaulC
* Added some system and exec commands.
* 
*    Rev 1.8   10 Apr 1991 15:16:34   WayneM
* Added 'DOCOLUMNS' flag for the editor.
* 
*    Rev 1.7   10 Apr 1991 13:38:54   ChrisA
* No change.
* 
*    Rev 1.6   12 Apr 1991 00:52:32   ChrisA
* New enums added for both print and editor
* 
*    Rev 1.5   09 Apr 1991 08:30:14   TracyK
* added defines of hssb sub-tasks 
* 
*    Rev 1.4   08 Apr 1991 16:09:10   TracyK
* 
* added keyboard/mouse actions 
* 
* 
*    Rev 1.3   03 Apr 1991 11:52:16   ChrisA
* added new actions and messages to printer task
* 
*    Rev 1.2   03 Apr 1991 10:03:36   JeffS
* Added the system message structure that was removed from types.h
* 
*    Rev 1.1   27 Mar 1991 08:43:26   WayneM
* Chg'd #define and #ifndef at the top of file to reflect file name.
* 
*    Rev 1.0   28 Mar 1991 08:19:40   ChrisA
* Initial revision.
*
*
*
****************************************************************************/

#ifndef MSGH
#define MSGH

		/* the enum BELOW is the highest byte in the function (MUST BE
			TYPE CAST TO A SHORT WHEN USED!) that is packaged for a
			subsequent sendmsg(). Class must never have more then 16 entries. */

enum classes { ACTION, RESPONSE = 0x1000, MSG = 0x2000, 
				 ACTION_RESPONSE = 0x3000, EXTEND = 0x4000 };
enum classes class;

/* these are redefined to route the sub-tasks within the HiSpeed bus */
#define KEYBD_DVR  ACTION
#define PRNTR_DVR  RESPONSE
#define FAX_DVR    MSG
#define MODEM_DVR  ACTION_RESPONSE

		/* the enum BELOW describes the commands/states possible that will
			will be placed in the lower 12 bits of the usMsgFunct short.
			These 'command bit patterns' may also be used to 'case' through
			the receiving procedure's code. */

enum commands { 
		
		/* editor/typemode  switches */

		KEYSENT, DOCADDR, RETADDR, ATTRCHG, EDBUFFADR, DOLAYOUT,
		HEADERFOOTER,	DOHEADFOOTER, DOCENTER, DOJUSTIFY, MOUSECLICK,
		DORULER, LINESPACE, MARGINS, PGFORMAT, DOCLIP, DOFOOTNOTE,
		DOPAGENO, DOCOLUMNS, DATAHOLD, DOUNDO, SRCH, SRCHNEXT, SRCHREPL,
		DOEDIT, DOREFER, SRCHREPLNEXT, TYPEOVER, DOGOTO, FONTCHG, DOCOPEN,
		DOCSAVE, LIST, GRAMMER, THESAURUS, WCOUNT, PWL, SPELL, FIND, DIRNAME,
		NOTECREAT, NOTEEDIT, NOTERETURN, NOTEOPTIONS, REDRAWSCRN, KBII, WALERT,
		PUNCT, DICTERROR, WALERTERROR, PUNCTERROR, KBIIDOWN, CONV2ASC, CONV2SC,
		CLRMEM, DOCJOIN, REDISPLAY, BOLD, UNDLN, UCASE, SRCHREPLCHK,
		SRCHREPLCONT,ENDNOTEOPTIONS, ENVELOPE, MOUSE_DOWN, MOUSE_UP, DEMO_UP,

  		/* system and exec switches */

		RESTRIGHT = 300,
		RESTLEFT, RESTALL, LEFT, RIGHT, OUTMEM, FATAL,
 		SUSPRIGHT, SUSPLEFT,	DIAL, DOWN, TYPE, TYPERETURN, DIALCLIP,
		DIALPASTE, CLIPPASTE, CLIPCUT, CLIPCOPY, NOFORM, WITHFORM,
		FILENAME, FILENONE,	FREEDWN,	SUSPLEFTNOBLACKEN, 
		SUSPRIGHTNOBLACKEN, RESTLEFTNOBLACKEN, RESTRIGHTNOBLACKEN,
		HELP, SUSPALL, HIDERIGHTRULER, SHOWRIGHTRULER, DISPLAY,
		DOWNDOCSAVE, DOWNNODOCSAVE, TYPEEXIT, TYPEFAIL,
		SUSPALLBUTCLIP, RESTALLBUTCLIP, STATSUSP, STATREST,
		DOWNGO, SHOWAPPL, EVENTIDLE, APPL, TYPERELEASE, CHARGE, LOW,
		LANG_SWITCH, IGNOREKEYS,

		/* hssb switches */
		
		DEFINE = 400,
		UNDEFINE, SUSPEND, UNSUSPEND, BEEP, WARBLE, CHANGE_BEEP,
		DEFINE_RESUME, UNDEFINE_RESUME, MOUSE_CODE, EXECUTE, GOTO,
		CHANGE_KEYBD_POLL, CHANGE_MOUSE_POLL, CHANGE_REPEAT_SPEED,
		SUSPEND_RESUME, UNSUSPEND_RESUME, START_KEYS, REPEAT,
		STOP_KEYS, MOUSE_ON, MOUSE_OFF, CHARGE_BATTERY,


		/* printer switches */

		CLICK = 600,
		PAUSE, RESUME, CANCEL, RETURN_BUFFER,	
		RSVP_RCV_PTRDOC, RSVP_PRNT_ERROR, RSVP_RCV_BUFFER, RSVP_DIALG_COMPLETE,    /* printer task msg's */	
		RSVP_PORT_OPEN, RSVP_PORT_CLOSE, TYPEWRITER, MAINBAR_CHK, 
		MAINMENU_CHK,	WP_PRINTDOC, IMPRESSION, PRINTER, PSTAT, PSETUP,
		PDIAGS, PEOP, PSTOPCODE, DIAG, PEOC, WP_FAXCONVERT, PCONFIG, PSERIAL,


		/* disk messages */

		DISK_ERROR = 800,
		DISK_DRIVEN, FILE_OPEN, FILE_CLOSE, FILE_READ, FILE_WRITE, FILE_TELL,
		FILE_SEEK, FILE_REWIND, FILE_FLUSH, FILE_EOF, FILE_CLEARERROR,
		DISK_RENAME_FILE, DISK_RENAME_DIR, DISK_REMOVE_FILE, DISK_REMOVE_DIR,
		DISK_MAKE_DIR, DISK_FIND_FIRST, DISK_FIND_NEXT, DISK_DRIVE_SET, 
		DISK_DRIVE_GET, DISK_CHANGE_DIR, 

		FILE_OPEN_S, FILE_CLOSE_S, FILE_READ_S, FILE_WRITE_S, FILE_TELL_S,
		FILE_SEEK_S, FILE_REWIND_S, FILE_FLUSH_S, FILE_EOF_S, 
		FILE_CLEARERROR_S, DISK_RENAME_FILE_S, DISK_RENAME_DIR_S, 
		DISK_REMOVE_FILE_S, DISK_REMOVE_DIR_S, DISK_MAKE_DIR_S,
		DISK_FIND_FIRST_S, DISK_FIND_NEXT_S, DISK_DRIVE_SET_S, 
		DISK_DRIVE_GET_S, DISK_CHANGE_DIR_S, 

		DISK_GET_INFO, DISK_COPY_FILES, DISK_COUNT_SIZES,
		DISK_GET_INFO_S, DISK_COPY_FILES_S, DISK_COUNT_SIZES_S,
		DISK_GET_VOLUME, DISK_CHECK_MOUNTED,
		DISK_GET_VOLUME_S, DISK_CHECK_MOUNTED_S,

		DISK_MENU_BAR_RET, DISK_SAVE_MENU_RET, DISK_RECALL_MENU_RET,
		DISK_ERASE_MENU_RET, DISK_RENAME_MENU_RET, DISK_JOIN_MENU_RET,
		DISK_DIR_MENU_RET, DISK_OTHER_MENU_RET, DISK_PREPARE_MENU_RET,
		DISK_COPY_MENU_RET, DISK_NAME_DISK_MENU_RET, DISK_ERASE_ATTEN_RET,
		DISK_RENAME_NEW_MENU_RET, DISK_APPLICATION_MENU_RET, 
		DISK_CONVERT_MENU_RET, DISK_TO_ASCII_MENU_RET, DISK_TO_SC_MENU_RET,
		DISK_REPLACE_ATTEN_RET, DISK_TEXT_IN_ATTEN_RET,

		DISK_SAVE_MENU_START,
		DISK_CACHE_ON, DISK_CACHE_OFF,

		/* comm switches (printer, fax, modem) */

		OPEN_DEVICE = 1100,
		CLOSE_DEVICE, SEND_BUFFER, RECV_BUFFER,
		FLUSH_BUFFER, DEVICE_ERROR,

		/* fax messages */
		MAIN_COMM_BAR_CHK = 1200,
		FAX_SEND_MENU_CHK,FAX_VIEW_MENU_CHK,
		FAX_PRNT_MENU_CHK,FAX_RECV_MENU_CHK,
		FAX_OPTIONS_MENU_CHK,FAX_SEND_STATUS_MENU_CHK,
		FAX_RECEIVE_STATUS_MENU_CHK,FAX_MANUAL_CHK,
		FAX_VIEW_STATUS_MENU_CHK,FAX_PRINT_MENU_CHECK,
      FAX_PHD_DIR, FAX_PHD_MENU_BAR, FAX_PHD_ADD,
      FAX_PHD_MOD, FAX_PHD_DEL,

		/* don't add at end any more unless new application */

		LASTCMD };	

enum commands command;

#define ACTION_MASK 0xf800
#define DRIVER_MASK 0xf800
#define COMMAND_MASK 0x07ff

/* define for recvnowait to determine if queue was empty */

#define QEMPTY 01
#define QEMPTYNOWAIT 55


#define MSNOTYETDISPLAYED 0 	 /* requested message has not been displayed */
#define MSDISPLAYED 1			 /* requested message is being displayed */
#define MSSUSPENDED 2 		 	 /* requested message was covered over before display finished */
#define MSTAKEDOWN  -1
#define MSLEAVEUP 0
#define MSNOFLASH 0
#define MSNOPREEMPT 0
#define MSRESUME 1
#define MSNORESUME 2

#define ENGLISH 0
#define METRIC 1

/****************************************************************************
* System messages are passed in this format. This should perhaps be moved to 
*   an operating system include file                                         
*
* Notes on the fields:
*       
*   MsgPara1 determines the message (user definable)  
*   MsgPara2 determines the message (user definable)  
*   MsgSender is the queue identifier of the input queue of the sending task.
*       If NULL, then no return is expected.
*   MsgPara3 is the first data field. It can contain object identifiers.
*   MsgBuffer is the address of a buffer or buffer chain. A NULL value
*       implies that no buffer is attached. Attached buffers may contain 
*       further commands.
******************************************************************************/

typedef struct sysmsg
{
	USHORT MsgPara1;  /* The first three fields are the same for all types */
	USHORT MsgPara2;
	ULONG	 MsgSender;
	union
	{
		ULONG  MsgPara3;     
		USHORT MsgSPara3[sizeof(ULONG)/sizeof(USHORT)];
		struct
		{
			USHORT MsgSPara3X;
			USHORT MsgSPara3Y;
		} MsgPara3XY;
	} MsgPara3Z;
	ULONG MsgBuffer;   
} *SYSMSG;

#endif

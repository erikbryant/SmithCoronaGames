/*****************************************************************************
*
*		$Workfile:   pcdisk.h  $		- Defines & structures for ms-dos utilities
*
*		$Revision:   1.46  $			$Modtime:   03 Sep 1991 09:46:04  $
*
*		$Author:   JimK  $
*
*					   
*
* MS-DOS compatible file system utilities -
*
* Copyright Peter Van Oudenaren , March, 1988
* This code may not be redistributed in source or linkable object form
* without the consent of its author.
*
* MS-DOS is a copyright of Microsoft corporation.
*
*	(C) Copyright 1989, Etc Bin Systems
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
* Description: 
*	
*
* $Log:   S:/disk/h/pcdisk.h_v  $
* 
*    Rev 1.46   03 Sep 1991 15:36:02   JimK
* Added disk_file_type list structure
* 
*    Rev 1.45   02 Sep 1991 15:10:12   JimK
* Changed directory allocation to dmalloc
* 
*    Rev 1.44   28 Aug 1991 13:51:12   JimK
* Changed motor stop time to ushort
* 
*    Rev 1.43   27 Aug 1991 21:28:58   JimK
* Updated motor start & stop delays
* 
*    Rev 1.42   27 Aug 1991 21:28:08   JimK
* 
*    Rev 1.41   27 Aug 1991 16:27:08   JimK
* Added PC_FILE_MYHDL, struct dirent malloc, Added FAT dirty bit
* 
*    Rev 1.40   23 Aug 1991 18:59:30   JimK
* Added MAX_CLUSTER_SIZE
* 
*    Rev 1.39   21 Aug 1991 18:41:02   JimK
* Decreased memory usage, smaller disk_dirent structure
* 
*    Rev 1.38   18 Jul 1991 16:12:50   JimK
* Changes made for diskcopy
* 
*    Rev 1.37   02 Jul 1991 11:13:22   JimK
* Moved global variables from vars section to zerovars
* 
*    Rev 1.36   01 Jul 1991 11:58:32   JimK
* Added disk volume name functions & dialog code.
* Cleaned-up disk task. Added some errors to DISK.H
* Improved disk dialogs. 
* Added code & jumptables for application loading.
* 
* 
*    Rev 1.35   14 Jun 1991 18:22:28   JimK
* More improvements for & to the disk dialogs. Mainly for erase & rename.
* Some disk functional changes for erase and findfirst / findnext.
* Some small changes to correct LINT complaints.
* 
* 
*    Rev 1.34   28 May 1991 08:47:38   JimK
* Added drive parameters for 400K disks.
* Misc changes.
* 
* 
*    Rev 1.33   22 May 1991 20:42:34   JimK
* Changed realfindfirst & realfindnext() functions return codes to match the return codes
* interface of the "non-real" pSOS message passing functions.
* Fixed file count error in filecopy function.
* Improved several startegies in diskbio.c for responding to no interrupt and 
* disk not loaded / changed.
* Added serial number handling when formated & reading block 0.
* Add some block 0 data error checking.
* 
* 
*    Rev 1.32   21 May 1991 10:21:24   JimK
* Changed make dir to create multiple directories.
* Created diskPathFirstDir().
* Added testing fo all malloc()'s against no memory.
* 
* 
*    Rev 1.31   20 May 1991 09:04:56   JimK
* Added new functions: diskPathJoin(), diskPathClarify() diskNameClarify(), 
* diskCopyFiles(), diskDriveGetInfo(), diskFileCountSizes().
* Added features to pshell.c to test new functions.
* 
* 
*    Rev 1.30   14 May 1991 17:26:04   JimK
* Improved realdiskRemoveDir() so that the current and parent directories may not be removed.
* Added full path support to realdiskMakeDir(), realdiskRenameFile(), 
* realdiskRenameDir(), realdiskRemoveDir().
* Fixed allocation holes in several structures in pcdisk.h
* Improved pshell.c program to enhance testing abilities.
* 
* 
*    Rev 1.29   02 May 1991 20:41:10   JimK
* Made changes necessary for the disk formatting functions.
* 
* 
*    Rev 1.28   26 Apr 1991 16:50:12   JimK
* Moved MAX() and MIN() to MACROS.H from PCDISK.H
* Fixed up PSHELL.C to use new disk messaging scheme.
* 
* 
*    Rev 1.27   19 Apr 1991 09:21:52   JimK
* Removed nested include file references from PCDISK.H; Split DISK.H from PCDISK.H;
* Used system TYPES.H include file; Renamed and/or deleted several disk include files.
* Removed duplicate functions from INT68K.A68
* Re-named many disk variable & structure member names.
* Removed almost all remaining warnings for both compilers.
* 
* 
*    Rev 1.26   12 Apr 1991 16:46:00   JimK
* Removed un-used values from PCDISK.H
* Renamed disk functions that will be hidden by message passing functions with 
* the same name (ie. fileOpen() etc.).
* Made corrections to fileSeek() & fileTell().
* 
* 
*    Rev 1.25   29 Mar 1991 16:38:58   JimK
* Renamed disk typedefed structures.
* 
* 
*    Rev 1.24   28 Mar 1991 15:50:44   JimK
* Changed disk device driver to use & wait for events when IO is occuring.
* Improved disk interrupt service routine so that it sends an event and returns 
* to pSOS when exiting.
* Changed diskIsrSet() to use assembly ISR function instead of C function.
* Removed usage of inp() & outp() functions.
* Altered DMA read & write setup functions to accept data length as a parameter.
* Removed extra motor on function calls.
* Renamed disk registers to correspond with Motorola names.
* Changed diskBlock0Read to call diskSectorRead() instead of diskBlockRead()
* Added code to automatically re-try lo-level disk operations that fail.
* Added code to detect when disk is changed & when disk is not loaded.
* Added disk changed & disk not loaded errors.
* Added functions to monitor DMA status.
* 
* 
*    Rev 1.23   10 Mar 1991 18:52:56   JimK
* Renamed 68000 assembly files from to .A68 extension from .SRC
* Renamed C prototype header files to .P extension from .H
* Cleaned up #include files used in pcdisk.h. Removed include files that were
* not needed by all files. Moved includes for <string.h>, <io.h>, <conio.h>
* and <stdio.h> to the individual source files where they are needed.
* Added usage of debug.h include file.
* 
* 
*    Rev 1.22   13 Dec 1990 15:52:38   JimK
* Improved & added new file...() functions & file...() function test functions.
* Added & corrected function documentation headers for file...() functions.
* 
* 
*    Rev 1.21   29 Nov 1990 17:45:44   JimK
* Modified code so that it will compile with ANSI Microtec 68K C compiler.
* 
* 
*    Rev 1.20   27 Nov 1990 12:11:02   JimK
* Misc. fixes for disk formating & better track 0 error detection.
* Improved error recognition & decoding in dskbios.c
* Changed .H files so that the contents are only included once.
* 
* 
*    Rev 1.19   16 Nov 1990 10:28:14   JimK
* Removed most old styled declarators.
* Removed unused functions.
* Added support for both forward & backward slashes to pcpath() & diskPathStrip().
* Changed most return codes from BOOL to ERROR where approriate.
* Cleaned up dskbios.c
* 
* 
*    Rev 1.18   26 Oct 1990 14:36:16   JimK
* Fixed bug in pc_mkfs.c when disk track 0 is bad.
* Changed realfileOpen() so that it won't delete hidden & system files on write access.
* Created diskPathMake() and diskPathSplit().
* 
* 
*    Rev 1.17   24 Oct 1990 09:31:22   JimK
* Fixed bug in pcmkfs.c when disk track 0 is bad.  Changed diskRemoveFile() & 
* realdiskRemoveDir() so that they won't delete hidden & system files.
* Created realdiskFindFirst() & realdiskFindNext().
* 
* 
*    Rev 1.16   10 Oct 1990 12:15:40   JimK
* Fixed bugs in realdiskRenameFile(). Created realdiskRenameDir().
* Started changing file*() functions to support more features. 
* Changed realfileOpen() for write access to truncate existing files.
* Changed several manifest constants & cosmetic changes.
* 
* 
*    Rev 1.15   28 Sep 1990 15:13:10   JimK
* Added support for '*' and '?' wildcard characters & misc. cosmetic changes.
* 
* 
*    Rev 1.14   20 Sep 1990 13:09:40   JimK
* Force filenames to uppercase & misc. cosmetic changes.
* 
* 
*    Rev 1.13   07 Sep 1990 13:13:46   JimK
* Renamed disk & file functions to be more descriptive & consistant
* 
* 
*    Rev 1.12   07 Sep 1990 10:23:48   JimK
* Changes required when physical & logical format programs combined 
* & verify function improved. Misc. re-naming & manifest constant cleanup.
* 
* 
*    Rev 1.11   04 Sep 1990 12:36:54   JimK
* Changed TRACING macro so that it can be defined from a makefile.
* 
* 
*    Rev 1.10   04 Sep 1990 07:43:56   JimK
* Re-named disk & file data structures and converted them to typedef's.
* 
* 
*    Rev 1.9   28 Aug 1990 11:20:10   JimK
* Fixed another unsigned LONG problem.
* 
* 
*    Rev 1.8   28 Aug 1990 10:56:02   JimK
* Fixed unsigned INT problem. unsigned int should be UINT.
* 
* 
*    Rev 1.7   27 Aug 1990 13:56:32   JimK
* Misc. cosmetic changes.
* 
* 
*    Rev 1.6   27 Aug 1990 13:42:54   JimK
* Misc. cosmetic changes & several type re-definition changes relating to 
* unsigned types.
* 
* 
*    Rev 1.5   27 Aug 1990 10:05:14   JimK
* Changed names of types and derived types that were used to be consistant 
* with the rest of the system.
* 
* 
*    Rev 1.4   22 Aug 1990 10:31:28   JimK
* Changed include directives to use double quotes instead of angle
* brackets for non-compiler supplied includes for ease of Makedeps
* and Make utilities.
* 
* 
*    Rev 1.3   17 Aug 1990 14:33:26   JimK
* 1st revsion with TRACE() macro implemented.
* 
* 
*    Rev 1.2   15 Aug 1990 11:59:28   JimK
* 1st revision with low-level controller code merged with
* high-level file system code.
* 
* 
*    Rev 1.1   19 Jul 1990 14:48:12   JimK
* 1st version with PVCS keywords
*
*
*****************************************************************************/



#ifndef	PCDISK_H
#define	PCDISK_H


/*---------------------------------------------------------------------------
	GENERAL SYSTEM DEFINITIONS
---------------------------------------------------------------------------*/

#define	BIT_TEST(data,mask)		(((data)&(mask))==(mask))
#define	BIT_ON(data,mask)			((data)|=(mask))
#define	BIT_OFF(data,mask)		((data)&=(~(mask)))

#define	TOHILONG(x)		(((ULONG)(x))<<16)
#define	FROMHILONG(x)	((USHORT)((x)>>16))



/*---------------------------------------------------------------------------
	DISK DEFINITIONS
---------------------------------------------------------------------------*/

#define	DISK_DEFAULT_TIME 	0x9103
#define	DISK_DEFAULT_DATE		0x1715


#define	DISK_RESERVED_36_SIZE	8			/* Size of block 0 reserved area at offset 0x36 */
#define	MS_DOS_V4XX_SIGNATURE	0x29		/* This byte at offset 0x26 in block 0 shows disk as MS-DOS v4.xx or later */

#define	BAD_CLUSTER_MARKER_12	0xFF7		/* 12 bit FAT table entry for a bad cluster */
#define	LAST_CLUSTER_MARKER_12	0xFFF		/* 12 bit FAT table entry for the last cluster in a chain */
#define	RESERVED_CLUSTER_MARKER_12	0xFF0	/* 12 bit FAT table entry for a reserved cluster */

#define	BAD_CLUSTER_MARKER		0xFFF7	/* 16 bit FAT table entry for a bad cluster (truncated for 12 bit FATs) */
#define	LAST_CLUSTER_MARKER		0xFFFF	/* 16 bit FAT table entry for the last cluster in a chain (truncated for 12 bit FATs) */
#define	RESERVED_CLUSTER_MARKER	0xFFF0	/* 16 bit FAT table entry for a reserved cluster */

#define	FREE_CLUSTER_MARKER		0x0		/* FAT table entry for a free cluster */


/* Number of DOS directory entries in a block */
#define	INODE_P_BLOCK					(DISK_SECTOR_SIZE / DISK_DIR_ENTRY_SIZE)


#define	MAX_FAT_12					4087	/* Highest cluster # for 12 bit FATs */
#define	MAX_SECTORS_TRACK			18		/* Maximum nof sectors per track for all formats */
#define	MAX_CLUSTER_SIZE			(2 * DISK_SECTOR_SIZE)	/* Maximum cluster size supported */

#define	DISK_RETRIES				3

/* Manifest constants for struct pc_file flags structure member */
#define	PC_FILE_READ				0x0001	/* == 1 when reading is allowed */
#define	PC_FILE_WRITE				0x0002	/* == 1 when writing is allowed */
#define	PC_FILE_MYHDL				0x0004	/* == 1 when realfileOpen() supplied pc_file struct */
#define	PC_FILE_MYBUF				0x0008	/* == 1 when realfileOpen() supplied r/w buffer */
#define	PC_FILE_EOF					0x0010	/* == 1 when end-of-file detected */
#define	PC_FILE_ERROR				0x0020	/* == 1 when file error detected */

#define	PC_FILE_APPEND				0x0100	/* == 1 when in append mode */
#define	PC_FILE_TEXT				0x0200	/* == 1 when in text mode */
														/* == 0 when in binary mode */
#define	PC_FILE_BUF_VALID			0x0400	/* == 1 when in buffer is valid (doesn't need to be read from disk) */
#define	PC_FILE_BUF_DIRTY			0x0800	/* == 1 when in buffer is dirty (needs to be written to disk) */


#define	MAX_NOF_FILES		25		/* Maximum nof simultaneous open files */

#define	MAX_NOF_DRIVES		2		/* Maximum nof logical drives */


#define	DIR_RESERVED_SIZE			(0x16-0xc)		/* Sizeof disk directory reserved area */
#define	DISK_NAME_SIZE				8		/* Sizeof disk name from boot record */
#define	DIR_DIRENT_MARKER			'.'	/* 1st character in filename to mark directory entries */
#define	ERASED_FILE_MARKER		((CHAR) 0xE5)	/* 1st character in filename to mark an erased file */
#define	E5_MARKER					((CHAR) 0x5)	/* 1st character in filename of filename that starts with 0xE5 */
#define	FORMATED_DISK_MARKER		((CHAR) 0xF6)	/* Fill character written to disk during format */



#define	DISK_EVENT_ISR				0x1	/* Event disk ISR creates */
#define	DISK_EVENT_TIMEOUT		75L	/* in 1/60 seconds / system clock ticks */

#define	DISK_PRE_ALLOCATED_FILES	1		/* Nof file structure and r/w buffers pre-allocated from static memory */


#ifndef	MSDOS

#define	DISK_TIME_OUT_1	120			/* in 1/60 seconds / system clock ticks */
#define	DISK_TIME_OUT_2	300			/* in 1/60 seconds / system clock ticks */

#else

#define	DISK_TIME_OUT_1	36				/* in 1/18 seconds */
#define	DISK_TIME_OUT_2	91				/* in 1/18 seconds */


#endif



#define	DISK_MAJOR_DEVICE				0


/* Manifest constants for function codes in DISK_IOPB */
#define	DISK_IO_VERSION	 			0
#define	DISK_IO_MOTOR_ON				1
#define	DISK_IO_MOTOR_OFF				2
#define	DISK_IO_RECALIBRATE			3
#define	DISK_IO_FORMAT_TRACK			4
#define	DISK_IO_SEEK_TRACK  			5
#define	DISK_IO_READ_TRACK  			6
#define	DISK_IO_READ_ID  				7
#define	DISK_IO_SPECIFY				8



/*---------------------------------------------------------------------------
	TYPE & STRUCTURE DEFINITIONS
---------------------------------------------------------------------------*/

/* Structure to contain block 0 image from the disk */
typedef struct disk_block0 {
	ULONG		boot_addr;
	USHORT	bytes_p_sect;			/* Must be 512 for this implementation */
	USHORT	sect_reserved;			/* Reserved sectors before the FAT */
	USHORT	nof_root;				/* Maximum nof of root dir entries */
	USHORT	nof_sectors;			/* Total nof sectors on the disk */
	USHORT	sect_p_fat;				/* Size of each fat */
	USHORT	sect_p_track;			/* Sectors per track */
	USHORT	nof_heads;				/* Number of heads */
	USHORT	nof_hidden;				/* Number of hidden sectors */
	CHAR		oem_name[ DISK_NAME_SIZE + 1 ];
	UCHAR		sect_p_alloc;			/* Sectors per cluster */
	UCHAR		nof_fats;				/* Number of FATS on the disk */
	UCHAR		media_desc;				/* Media descriptor byte */
	BOOL		dos_4;					/* TRUE if formated with MS-DOS 4.xx or later */
	UCHAR		reserved_25;			/* Reserved byte at offset 0x25 */
	ULONG		serial_n;				/* Disk serial number */
	CHAR		volume[ FILE_NAME_SIZE + 1 ];		/* Volume label */
	CHAR		reserved_36[ DISK_RESERVED_36_SIZE + 1 ];		/* Reserved bytes at offset 0x36 */

	/* Not realy part of block 0 parms, but a handy place to put the following: */
	BOOL		fat_is_dirty;			/* TRUE when FAT needs to be written to the disk */

} *DISK_BLOCK0;



/* Data structure used to represent a directory structure in memory */
typedef struct disk_dirent {
	/* Slightly modified version of the MS-DOS directory entry structure */
	CHAR		filename[ FILE_NAME_SIZE ];
	CHAR		file_ext[ FILE_EXT_SIZE ];
	UCHAR		attribute;				/* File attributes */
	#ifdef DIR_USE_RESERVED
	CHAR		res_area[ DIR_RESERVED_SIZE ];	/* reserved area; should be NULLs */
	#endif
	#ifdef DIR_USE_TIME_DATE
	USHORT	wr_time;					/* time & date last modified */
	USHORT	wr_date;
	#endif
	USHORT	cluster;					/* Cluster for data file */
	ULONG		size;						/* File size */
	/* Data structures used internally */
	/* prev entry in chain., or parent if "..", self if ".", NULL if top
	   of root 
	*/
	struct disk_dirent *pprev;
	struct disk_dirent *pnext;		/* Next in chain or null */
	struct disk_dirent *psub;		/* Sub directory if any */
	USHORT	dir_cluster;			/* Disk cluster for this entry */
} *DISK_DIRENT;


#define	DiskDirentAlloc()	((DISK_DIRENT) dmalloc( sizeof( struct disk_dirent ) ) )



/* User level "FILE" used by realfileOpen() et. al. */
typedef struct pc_file {
	CHAR		*bptr;			/* Current buffer pointer */
	CHAR		*bbase;			/* Base address of I/O buffer */
	USHORT	buff_size;		/* Size of buffer in bytes - currently 1 cluster */
	DISK_DIRENT ent;			/* Pointer to inode */
	DISK_DIRENT dir;			/* Pointer to dir */
	USHORT  	flags;			/* File flags; see PC_FILE_ constants above */
	USHORT	ccl;				/* Current cluster, (to for write, from for read */
	USHORT	cluster_index;	/* Cluster index of cluster in buffer */
	USHORT	cnt;				/* Current byte count for reading & writing in current cluster */
} *PC_FILE;



/* 
	IOPB - IO Parameter Block for disk implementation of pSOS device driver.

	Not all of the parameters in the IOPB may be used for all functions.

*/

typedef struct drive_parms	{
	UCHAR		sect_p_track;	/* Sectors per track										*/
	UCHAR		mfm;				/* FM mode or MFM mode [ 0 - 1 ]						*/
	UCHAR		sector_gap;		/* Read/write intersector gap length				*/
	UCHAR		format_gap;		/* Format intersector gap length						*/
	UCHAR		format_fill;	/* Format fill byte										*/
	UCHAR		bytes_p_sect_code;	/* Number of bytes per sector CODE					*/
	UCHAR		data_length;	/* Data length of sectors when sector CODE == 0	*/
	UCHAR		data_rate;		/* Data rate code											*/
	UCHAR		specify_1;		/* Byte number 1 for specify command				*/
	UCHAR		specify_2;		/* Byte number 2 for specify command				*/
	UCHAR		head_settle;	/* Head settle time after seek in milli-seconds	*/
	UCHAR		motor_start;	/* Motor start-up delay in clock ticks				*/
	USHORT	motor_stop;		/* Motor stop delay in clock ticks					*/
} *DRIVE_PARMS;



typedef struct disk_iopb	{
	UCHAR		function;		/* Code for disk sub-function to perform */
	UCHAR		drive_n;			/* Drive number [ 0 - 3 ]	*/
	UCHAR		trackn;			/* Track number */
	UCHAR		head_n;			/* Head number [ 0 - 1 ]	*/
	UCHAR		sector_n;		/* Starting sector number	*/
	UCHAR		eot;				/* End of track */
	UCHAR		mtind;			/* Multi-track operation indicator [ 0 - 1 ]	*/
	CHAR		*rwbuff;			/* Read or write buffer address */
	DRIVE_PARMS	drv;			/* Additional disk parameters */
} *DISK_IOPB;




typedef struct disk_format {
	CHAR		oem_name[ DISK_NAME_SIZE ];	/* Only first 8 bytes are used */
	USHORT	sect_reserved;		/* Reserved sectors before the FAT */
	USHORT	sect_p_fat;			/* Sectors per fat */
	USHORT	nof_root;			/* Maximum number of root dir entries */
	USHORT	nof_heads;			/* Number of heads */
	USHORT	nof_tracks;			/* Number of tracks */
	UCHAR		sect_p_alloc;		/* Sectors per cluster */
	UCHAR		nof_fats;			/* Number of FATS on the disk */
	UCHAR		media_desc;			/* Media descriptor byte */
	DRIVE_PARMS	drv;				/* Additional disk parameters */
} *DISK_FORMAT;


/* Structure used for disk copy functions */
typedef	struct disk_copy_clusters {
	USHORT	cluster_n;
	CHAR	*cluster_data;
	struct	disk_copy_clusters	*next_cluster;
	struct	disk_copy_clusters	*prev_cluster;
}	*DISK_COPY_CLUSTERS;




/* Structure used to keep track of file types for all files on display in
	the scrolling filename list dialog
*/
#define	MAX_FILE_TYPES	100

typedef struct disk_file_types {
		SHORT	index;
		DISK_DIRENT pdir;
} *DISK_FILE_TYPES;





#endif


/****************************************************************************
*
*		$Workfile:   disk.h  $	- Header file for use with functions from DISK.LIB
*
*		$Revision:   1.23  $			$Modtime:   31 Oct 1991 11:03:40  $
*
*		$Author:   JimK  $
*
*
*
*	(C) Copyright 1990,1991 Smith Corona Corp.
*	All Rights Reserved.
*
*
*
* $Log:   S:/h/disk.h_v  $
* 
*    Rev 1.23   31 Oct 1991 17:11:56   JimK
* Added ILLEGAL BLOCK number error
* 
*    Rev 1.22   23 Aug 1991 18:20:22   JimK
* Added A_DONT_CARE search attribute
* 
*    Rev 1.21   31 Jul 1991 17:26:56   JimK
* Added another dialog error
* 
*    Rev 1.20   23 Jul 1991 08:45:40   JimK
* Added E_FILE_EOF and E_NULL_PTR
* 
*    Rev 1.19   22 Jul 1991 17:51:26   JimK
* Convert & copy improvements; More named disk errors
* 
*    Rev 1.18   17 Jul 1991 14:35:04   JimK
* Renamed disk to editor sturcture member names
* 
*    Rev 1.17   01 Jul 1991 11:58:00   JimK
* Added disk volume name functions & dialog code.
* Cleaned-up disk task. Added some errors to DISK.H
* Improved disk dialogs. 
* Added code & jumptables for application loading.
* 
* 
*    Rev 1.16   14 Jun 1991 18:22:16   JimK
* More improvements for & to the disk dialogs. Mainly for erase & rename.
* Some disk functional changes for erase and findfirst / findnext.
* Some small changes to correct LINT complaints.
* 
* 
*    Rev 1.15   30 May 1991 14:31:04   JimK
* Made more changes for 400K disks.
* Changed & improved re-try strategy in DISKBIO.C
* Added detection of changed disks, recognizes same serial #'d disks.
* Misc changes.
* 
*    Rev 1.14   22 May 1991 20:42:26   JimK
* Changed realfindfirst & realfindnext() functions return codes to match the return codes
* interface of the "non-real" pSOS message passing functions.
* Fixed file count error in filecopy function.
* Improved several startegies in diskbio.c for responding to no interrupt and 
* disk not loaded / changed.
* Added serial number handling when formated & reading block 0.
* Add some block 0 data error checking.
* 
* 
*    Rev 1.13   21 May 1991 10:21:14   JimK
* Changed make dir to create multiple directories.
* Created diskPathFirstDir().
* Added testing fo all malloc()'s against no memory.
* 
* 
*    Rev 1.12   20 May 1991 09:04:46   JimK
* Added new functions: diskPathJoin(), diskPathClarify() diskNameClarify(), 
* diskCopyFiles(), diskDriveGetInfo(), diskFileCountSizes().
* Added features to pshell.c to test new functions.
* 
* 
*    Rev 1.11   14 May 1991 17:25:56   JimK
* Improved realdiskRemoveDir() so that the current and parent directories may not be removed.
* Added full path support to realdiskMakeDir(), realdiskRenameFile(), 
* realdiskRenameDir(), realdiskRemoveDir().
* Fixed allocation holes in several structures in pcdisk.h
* Improved pshell.c program to enhance testing abilities.
* 
* 
*    Rev 1.10   13 May 1991 16:20:44   JimK
* Added fnamestruct structure & a few more errors
* 
*    Rev 1.9   09 May 1991 18:30:12   JimK
* Modified realfileOpen() so it can accept paths as part of the filenames spec.
* Fixed bugs in diskPathTravelAllDir() and diskPathTravelOneDir() functions.
* Added call to disk format code to prepare dialog.
* 
* 
*    Rev 1.8   08 May 1991 20:00:34   JimK
* Renamed & fixed diskNameIsWild().
* Added & corrected some function documentation & disk errors returned.
* Added error to realfileOpen() if wildcards are used and if a file is opened
* multiple times for non-read access.
* Enhanced realdiskChangeDir() to work like DOS CD command.
* Removed pcpath() because it can be replaced by enhanced realdiskChangeDir().
* Added diskPathTravelAllDir() and diskPathTravelOneDir() functions.
* 
* 
*    Rev 1.7   04 May 1991 15:02:22   JimK
* Fixes for disk dialogs & formating.
* Replaced pc_memfill() and copybuf() with compiler supplied functions.
* Removed pause() calls.
* 
* 
*    Rev 1.6   02 May 1991 18:59:24   JimK
* Changed FILE_HDL to short from unsigned short
* 
*    Rev 1.5   02 May 1991 08:34:00   JimK
* Several changes to disk task.
* Added more printf() / TRACE() statements.
* 
* 
*    Rev 1.4   26 Apr 1991 15:28:52   JimK
* Added E_FILE_TOO_MANY error definition.
* Added inclusion of MSG.H in files that included SYSTEM.P
* 
*    Rev 1.3   26 Apr 1991 14:08:56   JimK
* Renamed file_hdl to FILE_HDL;
* Changed disk message interface to allow calling tasks to suspend.
* 
* 
*    Rev 1.2   23 Apr 1991 11:41:34   JimK
* Added conditional definition of file_hdl based on compiler/os.
* 
* 
*    Rev 1.1   19 Apr 1991 10:25:40   JimK
* Added typedef's for ERROR and PFI.
* 
* 
*    Rev 1.0   19 Apr 1991 09:21:16   JimK
* Removed nested include file references from PCDISK.H; Split DISK.H from PCDISK.H;
* Used system TYPES.H include file; Renamed and/or deleted several disk include files.
* Removed duplicate functions from INT68K.A68
* Re-named many disk variable & structure member names.
* Removed almost all remaining warnings for both compilers.
* 
*
*
*
****************************************************************************/



#ifndef	DISK_H
#define	DISK_H



/*---------------------------------------------------------------------------
	Some of the following manifest constants and functions have different 
	names than what might be defined by standard C compiler libraries and 
	ANSI C. This is to make it easier to switch between the different file 
	system libraries. If the constants and functions with different names can 
	always be re-mapped into the compiler specific names using macros. 

	The constants and functions having different names is also a reminder
	that the file system implementations are slightly different. The goal is
	to have the Smith Corona implementation follow the standard implementation
	as much as possible, but strict compatibility is not guaranteed.
---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
	DISK & FILE SPECIFIC DEFINITIONS
---------------------------------------------------------------------------*/

#ifndef	MSDOS
typedef	short			FILE_HDL;		/* Handle to file */
#else
#define	FILE_HDL		FILE	*
#endif


typedef	int	(*PFI)();		/* Pointer to function returning INT */
typedef	long	ERROR;

#define	EOS					'\0'					/* End of string character */


/* The following sizes are the maximum sizes without any path punctuation
	characters or 0-terminator characters.
*/
#define	FILE_NAME_SIZE				8		/* Sizeof file name */
#define	FILE_EXT_SIZE				3		/* Sizeof file name extension */


/* Sizes for buffers used by the diskPathMake() & diskPathSplit() functions.
	These sizes include space for the 0-terminator character & other path
	punctuation characters.
	MS-DOS v3.30 supports a maximum path of 63 characters. MS C v6.00 supports
	a maximum path of 260 characters. These are a reasonable compromise.
*/
#define	MAX_PATH			82		/* Maximum length of full pathname */
#define	MAX_DRIVE		3		/* Maximum length of drive component */
#define	MAX_DIR			64		/* Maximum length of directory component */
#define	MAX_FILE_NAME	10		/* Maximum length of file name component */
#define	MAX_EXT			5		/* Maximum length of extension component */



#define	DISK_DIR_ENTRY_SIZE		32		/* Nof bytes used by 1 directory entry on disk */

#define	DRIVE_MARKER				':'	/* Character to seperate a drive letter from a path */
#define	PATH_MARKER					'\\'	/* Character used to seperate directories within a path */
#define	ALT_PATH_MARKER			'/'	/* Alternate character used to seperate directories within a path */
#define	FILE_NAME_MARKER			'.'	/* Character that seperates a filename from a file extension */
#define	FILE_GLOBAL_WILDCARD		'*'	/* Multiple character wildcard for filenames and extensions */
#define	FILE_SINGLE_WILDCARD		'?'	/* Single character wildcard for filenames and extensions */

#define	DISK_SECTOR_SIZE			512	/* Disk sector size in bytes */
#define	BAD_FILE_HDL	-1					/* File handle for an un-opened file */



/* MS-DOS File attributes */
#define A_NORMAL		0x00	/* Normal file - No read/write restrictions */
#define A_RDONLY		0x01
#define A_HIDDEN		0x02
#define A_SYSTEM		0x04
#define A_VOLUME		0x08 
#define A_DIRENT		0x10
#define A_ARCHIVE		0x20
#define A_DONT_CARE	0xFF


/* fileSeek() origin constants */
#define FILE_SEEK_CUR	1
#define FILE_SEEK_END	2
#define FILE_SEEK_SET	0


/* DISK ERROR RETURN CODES */

#define	DISK_SUCCESS		0
/* -1 is an un-named error */
#define	E_NO_INTERRUPT			-2
#define	E_COMMAND				-3
#define	E_RESULTS				-4
#define	E_TRANSFER_SIZE		-5
#define	E_SEEK					-6
#define	E_NO_TRACK_0			-7
#define	E_CRC_DATA_FIELD		-8
#define	E_CRC_ADDR_FIELD		-9
#define	E_OVERRUN				-10
#define	E_NO_DATA				-11
#define	E_WRITE_PROTECT		-12
#define	E_MISSING_DATA_FIELD	-13
#define	E_MISSING_ADDR_FIELD	-14
#define	E_CONTROL_MARK			-15
#define	E_WRONG_TRACK			-16
#define	E_BAD_TRACK				-17
#define	E_RESET					-18
#define	E_BAD_TRACK_0			-19
#define	E_INIT_DRIVE			-20
#define	E_NO_DISK				-21
#define	E_DISK_CHANGED			-22

#define	E_RD_FILE_SYS			-23
#define	E_RD_BLK_0				-24
#define	E_WR_BLK_0				-25
#define	E_RD_FAT					-26
#define	E_WR_FAT					-27
#define	E_RD_DATA				-28
#define	E_WR_DATA				-29
#define	E_WR_SUB_DIR			-30
#define	E_RD_ROOT				-31
#define	E_WR_ROOT				-32
#define	E_ROOTDIR_FULL			-33
#define	E_DISK_FULL				-34
#define	E_RD_SUB_DIR			-35
#define	E_FILE_EXISTS			-36
#define	E_FILE_NOT_EXISTS		-37
#define	E_DIR_EXISTS			-38
#define	E_DIR_NOT_EXISTS		-39
#define	E_MAKE_FILE				-40
#define	E_MAKE_DIR				-41
#define	E_MAKE_SUB_DIR			-42
#define	E_SMALL_FAT				-43
#define	E_FORMAT_PARM			-44
#define	E_FIND_ROOT				-45
#define	E_FIND_DIR				-46
#define	E_FIND_FILE				-47
#define	E_DIR_RD_VOL			-48
#define	E_NOT_DIR				-49
#define	E_DIR_EMPTY				-50
#define	E_BAD_DRIVE				-51
#define	E_NOT_MOUNTED			-52
#define	E_MOUNTED				-53
#define	E_FILE_OPEN				-54
#define	E_FILE_NOT_OPEN		-55
#define	E_FILE_NOT_ACCESS		-56
#define	E_FILE_SEEK				-57
#define	E_FILE_ACCESS			-58
#define	E_FILE_TOO_MANY		-59
#define	E_BAD_BLOCK_N			-60
#define	E_NO_MEMORY				-61
#define	E_VERIFY_ERROR			-62
#define	E_FILE_DIR_NAME		-63
#define	E_FILE_MODE				-64
#define	E_DEST_IS_SRC			-65
#define	E_REMOVE_DIR			-66
#define	E_BLK_0_INFO			-67
#define	E_FIND_NAME				-68
#define	E_MAKE_VOL				-69
#define	E_MAKE_DIALOG			-70
#define	E_COPY_INCOMPLETE		-71
#define	E_FILE_EOF				-72
#define	E_NULL_PTR				-73
#define	E_DIALOG_BUSY			-74
#define	E_ILLEGAL_BLOCK_N		-75

/* Special error codes used only by pc_mkfs.c */
#define	E_WR_FAT_BLK			E_WR_FAT
#define	E_WR_ROOT_BLK			E_WR_ROOT





/* Modeled after the DOS find_t structure used by _dos_findfirst() and 
	_dosfindnext().
	This is used for the diskFindFirst() and diskFindNext() functions.
*/
typedef struct disk_find_reserved {
	UCHAR	attrib;						/* Search attributes */
	CHAR	pattern[ MAX_PATH ];		/* Search pattern */
	LONG	*pent;						/* This dir entry */
	LONG	*pdir;						/* This dir entry's directory */
} *DISK_FIND_RESERVED;

typedef struct disk_find {
	UCHAR	attrib;						/* File attributes */
	UINT	wr_time;						/* time & date last modified */
	UINT	wr_date;
	LONG	size;	  						/* File size */
	CHAR	name[ FILE_NAME_SIZE + FILE_EXT_SIZE + 2 ];
	struct disk_find_reserved reserved;
} *DISK_FIND;


/*
	This is used for the diskDriveGetInfo() function.
*/
typedef struct disk_info {
	USHORT sectors_per_cluster;
	USHORT bytes_per_sector;
	USHORT total_clusters;
	USHORT avail_clusters;
	USHORT bad_clusters;
	USHORT reserved_clusters;
} *DISK_INFO;


/*
	This is used for the diskCopyFiles() function.
*/
typedef struct disk_copy {
	CHAR	*src_files;
	CHAR	*dest_files;
	INT	nof_files_copied;		/* nof files successfully copied */
	INT	nof_files_missed;		/* nof files not copied */
} *DISK_COPY;



/*
	This is used for the diskFileCountSizes() function.
*/
typedef struct file_count_size {
	CHAR *pattern;
	INT	nof_files;
	ULONG logical_sizes;
	ULONG physical_sizes;
} *FILE_COUNT_SIZE;



typedef struct fnameStruct		/* used to house the path and filename */
{										/*   for editor/disk functions */
	CHAR *to_path;
	CHAR *from_path;
} *FNAMEPTR;


#endif

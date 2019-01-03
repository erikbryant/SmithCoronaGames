/****************************************************************************
*
*		$Workfile:   disk.p  $	- disk library function prototypes
*
*		$Revision:   1.13  $			$Modtime:   04 Nov 1991 10:13:46  $
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
* $Log:   S:/h/disk.p_v  $
* 
*    Rev 1.13   04 Nov 1991 10:16:26   JimK
* Moved FatalErrorCheck to disk.p from pcdisk.p
* 
*    Rev 1.12   01 Nov 1991 10:53:08   JimK
* Added fileLogicalSize proto from PCDISK.P
* 
*    Rev 1.11   01 Nov 1991 10:14:26   JimK
* Added diskMenuGetFileType proto
* 
*    Rev 1.10   23 Oct 1991 10:59:12   JimK
* Added disk track cache, removed disk_bios_error_n
* 
*    Rev 1.9   17 Sep 1991 17:02:38   JimK
* Added proto for DoSaveDialog 
* 
*    Rev 1.8   13 Sep 1991 16:34:48   JimK
* Added proto for mountedcheck 
* 
*    Rev 1.7   28 Aug 1991 13:50:26   JimK
* Added disk motor set stop time function
* 
*    Rev 1.6   14 Jun 1991 18:22:22   JimK
* More improvements for & to the disk dialogs. Mainly for erase & rename.
* Some disk functional changes for erase and findfirst / findnext.
* Some small changes to correct LINT complaints.
* 
* 
*    Rev 1.5   21 May 1991 10:21:18   JimK
* Changed make dir to create multiple directories.
* Created diskPathFirstDir().
* Added testing fo all malloc()'s against no memory.
* 
* 
*    Rev 1.4   20 May 1991 09:04:52   JimK
* Added new functions: diskPathJoin(), diskPathClarify() diskNameClarify(), 
* diskCopyFiles(), diskDriveGetInfo(), diskFileCountSizes().
* Added features to pshell.c to test new functions.
* 
* 
*    Rev 1.3   15 May 1991 14:33:18   JimK
* Fixed include file problem in trace.c
* Added some messaging to the editot task on a file save.
* 
* 
*    Rev 1.2   09 May 1991 18:30:18   JimK
* Modified realfileOpen() so it can accept paths as part of the filenames spec.
* Fixed bugs in diskPathTravelAllDir() and diskPathTravelOneDir() functions.
* Added call to disk format code to prepare dialog.
* 
* 
*    Rev 1.1   26 Apr 1991 14:09:00   JimK
* Renamed file_hdl to FILE_HDL;
* Changed disk message interface to allow calling tasks to suspend.
* 
* 
*    Rev 1.0   19 Apr 1991 09:21:48   JimK
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



#ifndef DISK_P
#define DISK_P


ULONG	diskQid( void );
ULONG diskTCall( USHORT para1, USHORT para2, ULONG para3, ULONG para4 );

USHORT	diskSetMotorStopTime( USHORT stop_delay_ticks );
ULONG	diskDoSaveDialog( PFI after_save_complete );

BOOL	diskIsCacheOn( void );
ERROR diskCacheOn( void );
ERROR diskCacheOff( void );

#ifndef	MSDOS
ERROR	fileOpen( CHAR *filename, CHAR *filemode, FILE_HDL *h_file, INT suspend );
ERROR	fileClose( FILE_HDL h_file, INT suspend );
ERROR	fileWrite( FILE_HDL h_file, CHAR *data, USHORT count, USHORT *return_count, INT suspend );
ERROR	fileRead( FILE_HDL h_file, CHAR *data, USHORT count, USHORT *return_count, INT suspend );
ERROR	fileTell( FILE_HDL h_file, LONG *offset, INT suspend );
ERROR	fileSeek( FILE_HDL h_file, LONG offset, USHORT origin, INT suspend );
ERROR	fileRewind( FILE_HDL h_file, INT suspend );
ERROR	fileFlush( FILE_HDL h_file, INT suspend );
ERROR	fileEof( FILE_HDL h_file, INT suspend );
ERROR	fileClearError( FILE_HDL h_file, INT suspend );
#endif

ERROR	diskRenameFile( CHAR *oldfilename, CHAR *newfilename, INT suspend );
ERROR	diskRenameDir( CHAR *olddirname, CHAR *newdirname, INT suspend );
ERROR	diskRemoveDir( CHAR *dir2remove, INT suspend );
ERROR	diskRemoveFile( CHAR *file2remove, INT suspend );
ERROR	diskMakeDir( CHAR *dir2make, INT suspend );
ERROR	diskFindFirst( CHAR *pattern, UINT attributes, DISK_FIND findbuff, INT suspend );
ERROR	diskFindNext( DISK_FIND findbuff, INT suspend );
ERROR	diskDriveSet( INT driven, INT suspend );
ERROR	diskDriveGet( INT suspend );
ERROR	diskChangeDir( CHAR *newpath, INT suspend );

ERROR	diskCopyFiles( DISK_COPY pdc, INT suspend );
ERROR	diskDriveGetInfo( DISK_INFO pdi, INT suspend );
ERROR	diskFileCountSizes( FILE_COUNT_SIZE pfcs, INT suspend );

ERROR	diskDriveMountedCheck( INT suspend );

void	diskPathSplit( CHAR *path, CHAR *drive, CHAR *dir, CHAR *file_name, CHAR *ext );
void	diskPathMake( CHAR *path, CHAR *drive, CHAR *dir, CHAR *file_name, CHAR *ext );
void	diskPathJoin( CHAR *dest_path, CHAR *src_path, CHAR *src_filename );
void	diskPathClarify( CHAR *dest_path, CHAR *src_path, CHAR *alt_filename );
CHAR	*diskPathFirstDir( CHAR *first_dir, CHAR *full_path );
void	diskPathStrip( CHAR *from, CHAR *to );

void	strCpyPad( CHAR *, CHAR *, INT );
void	strUnPad( CHAR * );

void	str2upper( CHAR * );
void	str2lower( CHAR * );

void	diskMenuGetFileType( INT name_select, CHAR *file_type );

#ifndef	MSDOS
ULONG	fileLogicalSize( FILE_HDL h_file );
#endif

BOOL	diskFatalErrorCheck( ERROR error_to_check );


#endif


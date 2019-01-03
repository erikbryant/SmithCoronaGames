/*  This include file defines macros for the PWP file/disk routines so
 *  programs can be compiled and run under MSC.
 */

#include <stdio.h>

#define fileOpen(name, mode, file_ptr, suspend)  file_ptr=fopen(name,mode)
#define fileClose(file_ptr, suspend)  fclose(file_ptr)
      /* This macro assumes size of items being written are char's  */
#define fileWrite(file_ptr, buffer, count, ret_code, suspend)  \
            fwrite(buffer, (size_t) sizeof(char), count, file_ptr)
      /* This macro assumes size of items being read are char's  */
#define fileRead(file_ptr, buffer, count, ret_code, suspend)  \
            fread(buffer, (size_t) sizeof(char), count, file_ptr)
#define fileEof(file_ptr, suspend)  feof(file_ptr)
#define diskMakeDir(name,suspend)
#define diskError(status)   printf("Disk error occured\n")
#define traceInit( win, string ) printf("%s\n",string)
#define trace(win, string) printf("%s\n",string)
#define trace_clr(win) printf (" Closing Window %d \n",win)
#define intoa(bytestoalloc,allocstr) itoa(bytestoalloc,allocstr,(int) 10)

#define FILE_HDL  FILE *
#define DISK_SUCCESS       0
#define E_FILE_NOT_EXISTS  0
#define E_DIR_NOT_EXISTS   0


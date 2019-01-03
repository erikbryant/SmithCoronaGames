/*****************************************************************************
**
**  Header Name:
**      sc_def.h
**
**  Description:
**      This module contains miscellaneous constant, macro, and type 
**      definitions to be included by every program that is to run on 
**      the Smith Corona machines.
**      Define useful types and some machine independent types.
**      On the 68000 an int is either 16 or 32 bits depending
**      on a compiler switch.  Make some types that will be
**      the same number of bytes no matter what is selected at
**      compile time.
**
**  Notes:
**      none
**
**  Change Log:
**
**      Date        Description of Change
**      ----        ---------------------
**      02/22/92    Initial version
**
*****************************************************************************/

#ifndef sc_def_h
#define sc_def_h

/*
** Boolean values.
*/
#define TRUE  1
#define FALSE 0
#define BOOL  unsigned char

/*
** Unsigned characters.
*/
#define UCHAR unsigned char

/*
** Various sizes of integers.
*/
   #define INT1  char              /* 1 byte   signed integer */
   #define INT2  short             /* 2 byte   signed integer */
   #define INT4  long int          /* 4 byte   signed integer */
   #define UINT1 unsigned char     /* 1 byte unsigned integer */
   #define UINT2 unsigned short    /* 2 byte unsigned integer */
   #define UINT4 unsigned int      /* 4 byte unsigned integer */

/*
** The printf statement on the PWPs is a little different.
** It is called dbprintf and pops up a window.
*/
   extern dbprintf ();             /* It is declared elsewhere     */
   #define printf dbprintf         /* It will replace the printfs  */

/*
** Create a macro for finding the MAX of two numbers.
*/
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

/*
** Define a byte swapping macro because the machines that this
** will be compiled on have different byte organizations.  The
** PC stores bytes in a Low-High order while the PWP stores them
** in High-Low order.  The font files need to use this because 
** they have data that is read into an array of unsigned characters
** which is then accessed as an array of 2-byte integers.  On the
** PC this gets messed up becuase of the way the bytes are stored.
** On the PWP this works fine.
**
**    b = 2 bytes of input
*/
   #define BYTE_SWAP(b) (b)

#endif

/*****************************************************************************
**
**  Module Name:
**      Spin-A-Phrase
**  
**  Description:
**      This module contains the macro and types definitions for Spin-A-Phrase.
**
**  Exported Units:
**      none
**
**  Local Units:
**      none
**      
**  Exported Data:
**
**  Notes:
**      none
**
**  Change Log:
**      Date            Description of Change
**      ----            ---------------------
**      05/20/92        Version 1.0 
**
**
*****************************************************************************/

#ifndef WHEEL_H
#define WHEEL_H

/*
** Various limitations on the size of data.
*/
#define MAX_TOPIC        15       /* Max number of chars in a topic */
#define MAX_RELATIONS    10       /* Max number of topics in phrase file */
#define TOPIC_ADJUSTMENT 33       /* So the topic bytes are not control chars */
#define TILE_AREA_WIDTH  16       /* units = number of tiles */
#define TILE_AREA_HEIGHT  4       /* units = number of tiles */
#define MAX_PUZZLE       (TILE_AREA_WIDTH * TILE_AREA_HEIGHT + 1)
                                  /* Max number of chars in a puzzle */
#define MAX_NAME         12       /* Max number of characters in a name */
#define MAX_PLAYERS      3        /* Max number of players */

/*
** Define the path to the phrase file.
*/
   #define PHRASE_FILE_NAME     "A:\\DATA\\PHRASES.DAT"

/*
** This phrase file has to have this number in the header to be
** a valid phrase file.
*/
#define MAGIC_NUMBER            0x3358

/*
** Information about the header of the phrase file.
*/
#define BUFFER_SIZE             3 /* How many data words at start of file? */
#define MAGIC_NUMBER_IDX        0 /* Which is the magic number? */
#define RELATION_SIZE_IDX       1 /* Which is the size of the relation table? */
#define INDEX_COUNT_IDX         2 /* Which is the size of the index table? */

#endif

#ifndef DEBUGH
#define DEBUGH

#ifdef  TRACING
#define	TRACE(args)		printf args
#else
#define	TRACE(args)		/* nothing */
#endif

#ifdef  MESSAGE
#define	DBPRINTF(args)		dbprintf args
#else
#define	DBPRINTF(args)		/* nothing */
#endif



#endif


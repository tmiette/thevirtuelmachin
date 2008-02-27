#ifndef UTIL_H_
#define UTIL_H_

/**
 --------------------------------------------------------------------------------
 BOOLEAN DEFINITIONS
 --------------------------------------------------------------------------------
 **/
#define true 1
#define false 0

/**
--------------------------------------------------------------------------
 MACRO DEFINITIONS.
 --------------------------------------------------------------------------------
 **/

/* Macro to check an allocation validity */
#define CHECK_ALLOCATION(pt) {\
	if(pt == NULL){\
		fprintf(stderr, "Memory not available\n");\
		exit(1);\
	}\
}

/* Macro to free memory and put the pointer to NULL */
#define FREE(pt){\
	free(pt);\
	pt = NULL;\
}

/* Macro to print debug messages */
#define DEBUG(x,y){\
	if((x) == 1){\
		(y);\
	}\
}


#endif /*UTIL_H_*/

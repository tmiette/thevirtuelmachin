#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 --------------------------------------------------------------------------------
 BOOLEAN DEFINITIONS
 --------------------------------------------------------------------------------
 **/
#define true 1
#define false 0

/**
---------------------------------------------------------------------------------
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

char* trim(char* toTrim);
int isDigit(char* string);


#endif /*UTIL_H_*/

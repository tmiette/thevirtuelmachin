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
 * Macro used to print debug messages.
 */
#define DEBUG(x,y){\
	if((x) == 1){\
		(y);\
	}\
}

/**
 * This function trim a string
 * 
 * @param toTrim the string to trim.
 * @return the trimed string.
 */
char* trim(char* toTrim);
char* trim2(char* toTrim);

/**
 * Tests if a string can be interpreted as an integer. Each character of the string is tested as a digit.
 * 
 * @param the string to test.
 * @return 0 if the string can be interpreted as an integer, or -1.
 */
int isDigit(char* string);

#endif /*UTIL_H_*/

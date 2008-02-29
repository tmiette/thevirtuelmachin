/**
 * @file Util.h
 * 
 * Defines some common functions and constant values usefull
 * all over the program.
 * 
 */
#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Constant value to define true boolean value.
 */
#define true 1

/**
 * Constant value to define false boolean value.
 */
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
 * This function trim a string by removing the spaces charcaters at the
 * beginning and at the end of the string. If the string given is delimited
 * by double quotes, these quotes are removed too. The parameter string 
 * is modified in this function.
 * 
 * @param toTrim the string to trim.
 * @return the trimed string.
 */
char* trim(char* toTrim);

/**
 * This function trim a string by removing the spaces charcaters at the
 * beginning and at the end of the string. The parameter string is modified
 * in this function.
 * 
 * @param toTrim the string to trim.
 * @return the trimed string.
 */
char* trim2(char* toTrim);

/**
 * Tests if a string can be interpreted as an integer. Each character of the string is tested as a digit.
 * 
 * @param string the string to test.
 * @return 0 if the string can be interpreted as an integer, or -1.
 */
int isDigit(char* string);

#endif /*UTIL_H_*/

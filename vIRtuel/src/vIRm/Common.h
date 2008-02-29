/**
 * @file Common.h
 * 
 * author MIETTE Tom
 * author MOURET Sebastien
 * 
 * Defines some constant and structure common for both shell and parser functions.
 */
#ifndef COMMON_H_
#define COMMON_H_

#define CMD_LENGTH 4096
#define VAR_LENGTH 2048
#define NAME_LENGTH 256
#define MAX_OBJECT_NUMBER 256

/**
 * Buffer for command line.
 */
char buffer[CMD_LENGTH];

/**
 * Strcuture representing the command line parsed. It contains command
 *  line patterns.
 */
typedef struct _command {
	char objectName[NAME_LENGTH]; // Object name object created or ongoing
	char functionName[NAME_LENGTH]; // Shell's function call by user
	char argv[VAR_LENGTH]; // Fucntion's arguments
	char var[NAME_LENGTH]; // Memory out variable
	char targetObject[NAME_LENGTH]; // Potential object to wait for
	int waitFor; // Waitfor pattern flag
} command;

#endif /*COMMON_H_*/

/**
 * @file SharedMemory.h
 * 
 * author MIETTE Tom
 * author MOURET Sebastien
 * 
 * Contains all function used to manage file memory mapped.
 */
#ifndef SHAREDMEMORY_
#define SHAREDMEMORY_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "Util.h"

#define BLOC_MEM_LENGTH 1024 // A memory bloc size
#define BLOC_NUMBER 10 // Max bloc number
#define MEM_LENGTH BLOC_MEM_LENGTH * BLOC_NUMBER // File length
extern int debug; // Flag to print print's debug

/**
 * Structure like a pair associates a bloc number to a string useful for variable.
 */
typedef struct Bloc__ {
	int blocValue;
	char blocName[BUFSIZ];
} Bloc;

/**
 * Initialzes the sharedMemory with the given file.
 * @char * file to map
 */
void initSharedMemory(char * memFile);

/**
 * Gets a free memory bloc number or -1 if memory is full.
 * @return free bloc number.
 */
int getFreeBloc();

/**
 * Free a given bloc.
 */
void freeBloc(int bloc);

/**
 * Close the mapped file.
 */
void closeSharedMemory();

/**
 * Enables to write a function arguments provided by shell to memory.
 * @param int bloc to write in.
 * @param char * data to write.
 */
void fillBloc(int index, char * data);

/**
 * Returns a bloc number by passing its name.
 * @param char * bloc name look for.
 * @return bloc number corresponding.
 */
int getBlocByName(char * name);

/**
 * Associates a bloc number to the given name.
 * @param int bloc number.
 * @param char * name to give to the bloc number.
 */
int nameBloc(int bloc, char * name);

/**
 * Returns a bloc name by passing its number.
 * @param int bloc to look for.
 * @return char * bloc name.
 */
char * getBlockName(int bloc);

#endif /*SHAREDMEMORY_*/

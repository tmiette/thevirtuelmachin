/**
 * @file Shell.h
 * 
 * author MIETTE Tom
 * author MOURET Sebastien
 * 
 * Contains all function to interpret virtuel machin commands.
 */
#ifndef SHELL_H_
#define SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "Util.h"
#include "Common.h"
#include "Parser.h"
#include "SharedMemory.h"

extern int debug; // Flag to print debug's printf

/**
 * Structure representing an object
 */
typedef struct _object {
	char name[NAME_LENGTH]; // Object's name
	pid_t pid; // Object's pid
	int pipe[2]; // Object's pipe
} object;

/**
 * Structure to send through the pipe to the object.
 */
typedef struct _job {
	pid_t pid; // Target's pid
	char functionName[NAME_LENGTH]; // Job's fucntion
	int memIn; // Memory to read
	int memOut; // Memory bloc to write for
} job;

void launch();

#endif /*SHELL_H_*/

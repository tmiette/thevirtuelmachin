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

extern int debug;

char * LIB_PATH = "../../objects/";

typedef struct _object {
	char name[NAME_LENGTH];
	pid_t pid;
	int pipe[2];
} object;

typedef struct _job {
	pid_t pid;
	char functionName[NAME_LENGTH];
	int memIn;
	int memOut;
} job;

void launch();

#endif /*SHELL_H_*/

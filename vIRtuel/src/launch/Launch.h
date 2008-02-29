#ifndef LAUNCH_H_
#define LAUNCH_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dlfcn.h>

#include "Util.h"

#define NAME_LENGTH 256
#define FIFO_LENGTH BUFSIZ
#define BLOC_MEM_LENGTH 1024

#define next(i) ((i)<FIFO_LENGTH-1 ? ((i)+1) : 0)


extern int debug;

typedef struct _job {
	pid_t pid;
	char functionName[NAME_LENGTH];
	int memIn;
	int memOut;
} job;

void waitJob();
void endLaunch();
void initSignals();
void openMemMapFile(char * memFile);

#endif /*LAUNCH_H_*/

#ifndef LAUNCH_H_
#define LAUNCH_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include "Util.h"

#define NAME_LENGTH 256
#define FIFO_LENGTH BUFSIZ

#define next(i) ((i)<FIFO_LENGTH-1 ? ((i)+1) : 0)


extern int debug;

typedef struct _job {
	pid_t pid;
	char functionName[NAME_LENGTH];
	int memIn;
	int memOut;
} job;

void handleMem();
void waitJob();

#endif /*LAUNCH_H_*/

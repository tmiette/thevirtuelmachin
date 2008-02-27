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

#define BLOC_MEM_LENGTH 1024
#define BLOC_NUMBER 10
#define MEM_LENGTH BLOC_MEM_LENGTH * BLOC_NUMBER

extern int debug;

void initSharedMemory();
int getFreeBloc();
void freeBloc(int bloc);
void closeSharedMemory();
void fillBloc(int bloc, char * data);
char * getBloc(int bloc);

#endif /*SHAREDMEMORY_*/

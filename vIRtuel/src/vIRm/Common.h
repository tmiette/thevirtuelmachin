#ifndef COMMON_H_
#define COMMON_H_

#define CMD_LENGTH 4096
#define VAR_LENGTH 2048
#define NAME_LENGTH 256
#define MAX_OBJECT_NUMBER 256

char buffer[CMD_LENGTH];

typedef struct _command {
	char objectName[NAME_LENGTH];
	char functionName[NAME_LENGTH];
	char argv[VAR_LENGTH];
	char var[NAME_LENGTH];
	char targetObject[NAME_LENGTH];
	int waitFor;
} command;

#endif /*COMMON_H_*/

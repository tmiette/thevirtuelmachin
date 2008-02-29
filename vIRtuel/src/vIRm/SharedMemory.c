#include "SharedMemory.h"

static void * memory= NULL;
static int memoryLength = 0;
static Bloc blocs[BLOC_NUMBER];
static char END_BLOC_CHAR = '\n';
static void writeParameters(char * par, void * output);
static int isValidBloc(int bloc);
static void * getBloc(int index);

void initSharedMemory(char * memFile) {
	int file;
	struct stat info;
	char * mem;
	int memLength;
	int i;

	for (i = 0; i < BLOC_NUMBER; ++i) {
		blocs[i].blocValue = -1;
		blocs[i].blocName[0] = '\0';
	}

	if ((file = open(memFile, 
	O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {
		perror("initSharedMemory -> open");
		exit(-1);
	}

	/* fill file with '0' */
	for (i = 0; i < MEM_LENGTH; ++i) {
		if (write(file, &END_BLOC_CHAR, 1) == -1) {
			perror("initSharedMemory -> write");
			exit(-1);
		}
	}

	/* retrieve stats from file */
	if (stat(memFile, &info) == -1) {
		perror("initSharedMemory -> stat");
		exit(-1);
	}

	memLength = info.st_size;

	/* map the file in memory */
	mem = mmap(NULL,
	memLength,
	PROT_READ | PROT_WRITE,
	MAP_SHARED,
	file,
	0);

	if (mem == (void *)-1) {
		perror("initSharedMemory -> mmap");
		exit(-1);
	}

	memory = mem;
	memoryLength = memLength;

	close(file);

}

int getFreeBloc() {
	int i;
	for (i = 0; i < BLOC_NUMBER; ++i) {
		printf("getFreeBloc -> (%d) (%s)\n", blocs[i].blocValue,
				blocs[i].blocName);
		if (blocs[i].blocValue == -1) {
			return i;
		}
	}
	return -1;
}

void freeBloc(int index) {
	int i = 0;
	char * pt = (char*)getBloc(index);
	
	if (isValidBloc(index)) {
		blocs[index].blocValue = -1;
		blocs[index].blocName[0] = '\0';
		while(i != BLOC_MEM_LENGTH){
			pt[i] = END_BLOC_CHAR;
			i++;
		}
	}
}

void closeSharedMemory() {
	if (munmap(NULL, memoryLength) == -1) {
		perror("initSharedMemory -> munmap");
		exit(-1);
	}

}

void fillBloc(int index, char * data) {
	void * memHead;
	if (isValidBloc(index)) {
		DEBUG(debug, printf("fillBloc -> data to copy (%s), bloc (%d, %s)\n",
				data, index, blocs[index].blocName));
		/* Set memory pointer to bloc specified */
		memHead = memory + (index * BLOC_MEM_LENGTH);
		writeParameters(data, memHead);
		blocs[index].blocValue = index;
	}
}

static void writeParameters(char * par, void * output) {

	char* tokens= NULL;
	char* trimed= NULL;
	int integer = 0;
	int bloc = -1;

	tokens = strtok(par, ",");
	while (tokens != NULL) {

		trimed = trim(tokens);

		// integer case
		if (isDigit(trimed)) {
			integer = atoi(trimed);
			memcpy(output, &integer, sizeof(int));
			output += sizeof(int);
		}
		// variable case
		else if ((bloc = getBlocByName(trimed)) != -1) {
			/* Get memory pointer corresponding to variable */
			void * mem = getBloc(bloc);
			if (mem != NULL) {
				char * pt = (char *)mem;
				int len = 0;
				int match = 0;
				while (len < BLOC_MEM_LENGTH) {
					if (*pt == END_BLOC_CHAR) {
						pt++;
						if (++match == 2) {
							break;
						}
					} else {
						match = 0;
						pt++;
						len++;
					}
				}
				memcpy(output, mem, len);
				printf("maaaaaaaa len = %d", len);
				output += len;
			}
			DEBUG(
					debug,
					printf(
							"writeParameters -> variable (%s) at bloc (%d) (mem=%d, out=%d)\n",
							trimed, bloc, mem, output));
		}
		// string case
		else {
			sprintf(output, "%s\0", trimed);
			output += strlen(trimed) + 1;
		}

		tokens = strtok(NULL, ",");
	}
}

/**
 * Checks validy of the memory bloc index. Returns 1 if index is valid, 0 otherwise.
 * @param int index index to check the validity.
 */
int isValidBloc(int index) {
	if (index >= 0 && index < BLOC_NUMBER && memoryLength != 0) {
		return 1;
	}
	return 0;
}

/**
 * Returns the bloc corresponding to the given name or -1 if name is unknow.
 */
int getBlocByName(char * name) {
	int i;
	for (i = 0; i < BLOC_NUMBER; ++i) {
		if (name[0] != '\0' && strcmp(blocs[i].blocName, name) == 0) {
			return blocs[i].blocValue;
		}
	}
	return -1;
}

char * getBlockName(int bloc) {
	if (isValidBloc(bloc)) {
		return blocs[bloc].blocName;
	}
	return NULL;
}

int nameBloc(int bloc, char * name) {
	if (isValidBloc(bloc) && blocs[bloc].blocValue == -1) {

		blocs[bloc].blocValue = bloc;
		strcpy(blocs[bloc].blocName, name);
		DEBUG(debug, printf("nameBloc -> name (%s) given to bloc (%d)\n",
				blocs[bloc].blocName, blocs[bloc].blocValue));
		return 1;
	}
	return -1;
}

static void * getBloc(int index) {
	char * head= memory;
	head += index * BLOC_MEM_LENGTH;
	return head;
}


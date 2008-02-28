#include "SharedMemory.h"

static char * MEM_NAME ="sharedmem";
static char * memory= NULL;
static int memoryLength = 0;
static Bloc blocs[BLOC_NUMBER];

static int isValidBloc(int bloc);

void initSharedMemory() {
	int file;
	struct stat info;
	char * mem;
	char c = '0';
	int memLength;
	int i;

	for (i = 0; i < BLOC_NUMBER; ++i) {
		blocs[i].blocValue = -1;
		blocs[i].blocName[0] = '\0';
	}

	if ((file = open(MEM_NAME, 
	O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {
		perror("initSharedMemory -> open");
		exit(-1);
	}

	/* fill file with '0' */
	for (i = 0; i < MEM_LENGTH; ++i) {
		if (write(file, &c, 1) == -1) {
			perror("initSharedMemory -> write");
			exit(-1);
		}
	}

	/* retrieve stats from file */
	if (stat(MEM_NAME, &info) == -1) {
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

	DEBUG(debug, printf("initSharedMemory -> done\n"));

}

int getFreeBloc() {
	int i;
	for (i = 0; i < BLOC_NUMBER; ++i) {
		if (blocs[i].blocValue == -1) {
			return i;
		}
	}
	return -1;
}

void freeBloc(int index) {
	if (isValidBloc(index)) {
		blocs[index].blocValue = -1;
		blocs[index].blocName[0] = '\0';
	}
}

void closeSharedMemory() {
	if (munmap(NULL, memoryLength) == -1) {
		perror("initSharedMemory -> munmap");
		exit(-1);
	}

}

void fillBloc(int index, char * data) {
	int i = 0;
	if (isValidBloc(index)) {
		DEBUG(debug, printf("fillBloc -> data to copy (%s), bloc (%d, %s)\n",
				data, index, blocs[index].blocName));
		while (data[i] != '\0') {
			memory[index * BLOC_MEM_LENGTH + i] = data[i];
			i++;
		}
		memory[index * BLOC_MEM_LENGTH + i] = '\0';
		blocs[index].blocValue = index;
	}
}

void writeParameters(char * par, void * output) {

	char* tokens= NULL;
	char* trimed= NULL;
	int integer = 0;

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
		else if (0) {

		}
		// string case
		else {
			sprintf(output, "%s\0", trimed);
			output += strlen(trimed) + 1;
		}

		tokens = strtok(NULL, ",");
	}
}

char * getBloc(int index) {
	char * head= NULL;
	if (isValidBloc(index)) {
		head = &memory[index * BLOC_MEM_LENGTH];
	}
	return head;
}

/**
 * Checks validy of the memory bloc index. Returns 1 if index is valid, 0 otherwise.
 */
int isValidBloc(int index) {
	if (index >= 0 && index < BLOC_NUMBER && memoryLength != 0) {
		DEBUG(debug, printf("isValidBloc -> valid bloc (%d)\n", index));
		return 1;
	}
	DEBUG(debug, printf("isValidBloc -> not a valid bloc (%d)\n", index));
	return 0;
}

/**
 * Returns the bloc corresponding to the given name or -1 if name is unknow.
 */
int getBlocByName(char * name) {
	int i;
	for (i = 0; i < BLOC_NUMBER; ++i) {
		printf("index (%d), blocs[i].blocName (%s)\n", i, blocs[i].blocName);
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
	if (name[0] != '\0' && isValidBloc(bloc) && blocs[bloc].blocValue == -1) {
		DEBUG(debug, printf("nameBloc -> name (%s) given to bloc (%d)\n",
				blocs[bloc].blocName, blocs[bloc].blocValue));
		blocs[bloc].blocValue = bloc;
		strcpy(blocs[bloc].blocName, name);
	}
	return -1;
}


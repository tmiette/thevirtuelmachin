#include "SharedMemory.h"

static char * MEM_NAME ="sharedmem";
static int blocs[BLOC_NUMBER];
static char * memory= NULL;
static int memoryLength = 0;

static int isValidBloc(int bloc);

void initSharedMemory() {
	int file;
	struct stat info;
	char * mem;
	char c = '0';
	int memLength;
	int i;

	for (i = 0; i < BLOC_NUMBER; ++i) {
		blocs[i] = -1;
	}

	if ((file = open(MEM_NAME, O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {
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
		if (blocs[i] == -1) {
			return i;
		}
	}
	return -1;
}

void freeBloc(int bloc) {
	if (isValidBloc(bloc)) {
		blocs[bloc] = -1;
	}
}

void closeSharedMemory() {
	if (munmap(NULL, memoryLength) == -1) {
		perror("initSharedMemory -> munmap");
		exit(-1);
	}

}

void fillBloc(int bloc, char * data) {
	int i = 0;
	DEBUG(debug, printf("fillBloc -> data to copy (%s)\n", data));
	if (isValidBloc(bloc)) {
		while (data[i] != '\0') {
			memory[bloc * BLOC_MEM_LENGTH + i] = data[i];
			i++;
		}
		memory[bloc * BLOC_MEM_LENGTH + i] = '\0';
	}
}

char * getBloc(int bloc) {
	char * head= NULL;
	if (isValidBloc(bloc)) {
		head = &memory[bloc * BLOC_MEM_LENGTH];
	}
	return head;
}

int isValidBloc(int bloc) {
	if (bloc >= 0 && bloc < BLOC_NUMBER && memoryLength != 0) {
		DEBUG(debug, printf("isValidBloc -> valid bloc (%d)\n", bloc));
		return 1;
	}
	DEBUG(debug, printf("isValidBloc -> not a valid bloc (%d)\n", bloc));
	return 0;
}


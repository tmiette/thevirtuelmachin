#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "Launch.h"
sigset_t mask;
int debug = true;
void (*ptWork)(void*, void*)= NULL;
void * memory= NULL;

void handler();
static void openMemMapFile(char * memFile);
static void closeSharedMemory();
static int memoryLength = 0;

int main(int argc, char **argv) {
	void * handle= NULL;

	if (argc != 3) {
		perror("launch usage : 'launch' 'object' 'shared memory' ");
		exit(-1);
	}

	printf("memfile %s", argv[2]);
	openMemMapFile(argv[2]);

	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR2);
	sigprocmask( SIG_BLOCK, &mask, NULL);

	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigaction(SIGUSR2,&sa,NULL);

	sigfillset(&mask);
	sigdelset(&mask, SIGUSR2);

	setvbuf(stdout, NULL, _IONBF, 0);

	/* Open tube mkfifo */
	//	if ((fd = open("/home/akiri/Documents/workspace/vIRtuel/bin/montube", O_WRONLY)) == -1) {
	//		perror("launch : open mkfifo");
	//		exit(-1);
	//	}
	//	dup2(fd, 1);

	/* Open target library */
	handle = dlopen("../../objects/int2string.so", 
	RTLD_LAZY);
	if (!handle) {
		perror("unable to open library\n");
		exit(-1);
	}

	/* Initialize work function pointer */
	*(void**)(&ptWork) = dlsym(handle, "work");
	if (ptWork == NULL) {
		perror("error dlsym");
		exit(-1);
	}

	/* Handle memory */
	handleMem();

	/* Start waiting for command */
	waitJob();

	/* Close library */
	dlclose(handle);

	return 0;
}

static void openMemMapFile(char * memFile) {
	int file;
	struct stat info;
	int memLength;

	if ((file = open(memFile, 
	O_RDWR)) == -1) {
		perror("openMemMapFile -> open");
		exit(-1);
	}

	/* retrieve stats from file */
	if (stat(memFile, &info) == -1) {
		perror("openMemMapFile -> stat");
		exit(-1);
	}

	memLength = info.st_size;

	/* map the file in memory */
	memory = mmap(NULL,
	memLength,
	PROT_READ | PROT_WRITE,
	MAP_SHARED,
	file,
	0);

	if (memory == (void *)-1) {
		perror("openMemMapFile -> mmap");
		exit(-1);
	}

	memoryLength = memLength;
	
	close(file);
}

static void closeSharedMemory() {
	if (munmap(NULL, memoryLength) == -1) {
		perror("initSharedMemory -> munmap");
		exit(-1);
	}

}

void handler(int signal) {
	printf("coucou, je suis %d, reception du signal %d\n", getpid(), signal);
}

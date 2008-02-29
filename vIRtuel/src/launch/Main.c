#include <dlfcn.h>
#include "Launch.h"
sigset_t mask;
int debug = true;
void (*ptWork)(void*, void*)= NULL;
void * handle= NULL;

int main(int argc, char **argv) {
	int fd;

	if (argc != 3) {
		perror("launch usage : 'launch' 'object' 'shared memory' ");
		exit(-1);
	}

	openMemMapFile(argv[2]);

	initSignals();

	setvbuf(stdout, NULL, _IONBF, 0);

	/* Open tube mkfifo */
		if ((fd = open("../../bin/montube",
			O_WRONLY)) == -1) {
		perror("launch : open mkfifo");
	}
	dup2(fd, 1);

	/* Open target library */
	printf("lib : %s\n", argv[1]);
	handle = dlopen(argv[1], 
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

	/* Start waiting for command */
	waitJob();

	return 0;
}


#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include "Launch.h"

int debug = true;

void handler();

int main(int argc, char **argv) {
	void * handle= NULL;
	int fd;
	FILE * file = NULL;

	if (argc != 3) {
		perror("launch usage : 'launch' 'object' 'shared memory' ");
		exit(-1);
	}
	
	signal(SIGCONT, handler);
	
	setvbuf(stdout, NULL, _IONBF, 0);

	/* open tube mkfifo */
//	if ((fd = open("/home/akiri/Documents/workspace/vIRtuel/bin/montube", O_WRONLY)) == -1) {
//		perror("launch : open mkfifo");
//		exit(-1);
//	}
//	dup2(fd, 1);
	
	/* open target library */
	handle
			= dlopen(
					"/home/akiri/Documents/workspace/vIRtuel/objects_src/int2string.so",
					RTLD_LAZY);
	if (!handle) {
		perror("unable to open library\n");
		exit(-1);
	}
	
	/* handle memory */
	handleMem();

	/* start waiting for command */
	waitJob();

	return 0;
}

void handler(){
	printf("\n\n\nreveille (%d)\n\n\n", getpid());
}

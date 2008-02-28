#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include "Launch.h"
sigset_t mask;
int debug = true;
void (*ptWork)(void*, void*) = NULL;

void handler();

int main(int argc, char **argv) {
	void * handle= NULL;
	int fd;
	FILE * file = NULL;

	if (argc != 3) {
		perror("launch usage : 'launch' 'object' 'shared memory' ");
		exit(-1);
	}
	
	
	
	sigemptyset (&mask);
	sigaddset (&mask, SIGUSR2);
	sigprocmask ( SIG_BLOCK, &mask, NULL);


	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigaction(SIGUSR2,&sa,NULL);

	sigfillset (&mask);
	sigdelset (&mask, SIGUSR2);

	setvbuf(stdout, NULL, _IONBF, 0);

	/* Open tube mkfifo */
//	if ((fd = open("/home/akiri/Documents/workspace/vIRtuel/bin/montube", O_WRONLY)) == -1) {
//		perror("launch : open mkfifo");
//		exit(-1);
//	}
//	dup2(fd, 1);
	
	/* Open target library */
	handle
			= dlopen(
					"/home/akiri/workspace/vIRtuel/objects_src/int2string.so",
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

void
handler (int signal)
{
printf("coucou, je suis %d, reception du signal %d\n", getpid(), signal);
}

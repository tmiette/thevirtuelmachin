#include "Launch.h"

extern void (*ptWork)(void*, void*); // Work pointer function
extern void * handle;
static sigset_t mask;
static void * memory= NULL;
static int memoryLength = 0;
static void handleJob(job * j);
static void * getBloc(int index);
static void handler();

/**
 * Read on stdin and wait a job structure
 */
void waitJob() {
	job j;

	while (1) {
		switch (read(0, &j, sizeof(job))) {
		case -1:
			perror("read job");
			break;
		case sizeof(job):
			DEBUG(
					debug,
					printf(
							"waitJob -> pid (%d) function (%s) memIn (%d) memOut (%d)\n",
							j.pid, j.functionName, j.memIn, j.memOut));
			handleJob(&j);
			break;
		default:
			break;
		}
	}
}

/**
 * Handle a job structure comes from shell
 */
static void handleJob(job * j) {
	DEBUG(debug, printf("handleJob -> object (%d) function (%s)\n", getpid(),
			j->functionName));
	/* do it */
	if (strcmp(j->functionName, "work") == 0) {
		DEBUG(debug, printf("handleJob -> work (%d) (%s) (%d) (%d)\n", j->pid,
				j->functionName, j->memIn, j->memOut));

		/* Execute work function */
		void * memIN = getBloc(j->memIn);
		void * memOUT = getBloc(j->memOut);
		(*ptWork)(memIN, memOUT);

		/* Send signal to my father to say to free in memory zone */
		union sigval sVal;
		sVal.sival_int = j->memIn;
		if (sigqueue(getppid(), SIGRTMIN, sVal) == -1) {
			perror("handleJob -> send SIGRTMIN with mem zone to free");
			exit(-1);
		}

		sVal.sival_int = j->memOut;
		if (sigqueue(getppid(), SIGRTMIN, sVal) == -1) {
			perror("handleJob -> send SIGRTMIN with mem zone to free");
			exit(-1);
		}

	} else if (strcmp(j->functionName, "waitall") == 0) {
		DEBUG(debug, printf("handleJob -> waitall send by (%d)\n", j->pid));

		/* I send a ready signal to my father to say */
		if (kill(getppid(), SIGUSR1) == -1) {
			perror("handleJob -> kill SIGUSR1");
			exit(-1);
		}

		DEBUG(debug, printf("handleJob -> I'm going to sleep (%d)\n", getpid()));
		/* I'm waiting for a signal from my father to re-start */
		sigsuspend(&mask);
		DEBUG(debug, printf("handleJob -> I wake up (%d)\n", getpid()));

	} else if (strcmp(j->functionName, "waitfor") == 0) {
		DEBUG(debug, printf(
				"handleJob -> SIGCONT sent to (%d) from object (%d)\n", j->pid,
				getpid()));
		kill(j->pid, SIGCONT);
	} else if (strcmp(j->functionName, "free") == 0) {
		DEBUG(debug, printf("handleJob -> free\n"));
		/* Send signal to my father to say to free in memory zone */
		union sigval sVal;
		sVal.sival_int = j->memIn;
		if (sigqueue(getppid(), SIGRTMIN+1, sVal) == -1) {
			perror("handleJob -> send SIGRTMIN+1 with mem zone to free");
			exit(-1);
		}
	}

}

/**
 * Gets a memory pointer thanks to the bloc number
 */
static void * getBloc(int index) {
	char * head= memory;
	head += index * BLOC_MEM_LENGTH;
	return head;
}

/**
 * Map the given file in memory.
 */
void openMemMapFile(char * memFile) {
	int file;
	struct stat info;
	int memLength;

	if ((file = open(memFile, O_RDWR)) == -1) {
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

/**
 * Close the mapped memory file
 */
static void closeSharedMemory() {
	if (munmap(NULL, memoryLength) == -1) {
		perror("initSharedMemory -> munmap");
		exit(-1);
	}

}

/**
 * Initializes signals
 */
void initSignals() {

	if (signal(SIGINT, endLaunch) == SIG_ERR) {
		perror("signal");
		exit(-1);
	}

	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR2);
	sigprocmask( SIG_BLOCK, &mask, NULL);

	struct sigaction sa;
	sa.sa_sigaction = handler;
	sa.sa_flags = 0;
	sigaction(SIGUSR2,&sa,NULL);

	sigfillset(&mask);
	sigdelset(&mask, SIGUSR2);

}

/**
 * Close processus
 */
void endLaunch() {
	DEBUG(debug, printf("endLaunch -> end object (%d)\n", getpid()));
	closeSharedMemory();
	/* Close library */
	dlclose(handle);
	exit(EXIT_SUCCESS);
}

/**
 * Default handler nothing to do in .
 */
void handler(){
	return;
}

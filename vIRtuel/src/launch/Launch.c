#include "Launch.h"

job jobs[BUFSIZ];
static int firstJob = 0;
static int lastJob = 0;
static int jobsNumber = 0;

static void initJobs();
static int pushJob(job * j);
static int popJob();
static void copyJob(job * src, job * dest);
static void handleJob(job * j);

void handleMem() {

}

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

static void handleJob(job * j) {
	DEBUG(debug, printf("handleJob -> function (%s)\n", j->functionName));
	/* do it */
	if (strcmp(j->functionName, "work") == 0) {
		DEBUG(debug,
				printf("handleJob -> object (%d) received function (work)\n",
						getpid()));
	} else if (strcmp(j->functionName, "waitall") == 0) {
		DEBUG(debug, printf("handleJob -> waitall send by (%d)\n", j->pid));

		if (kill(getppid(), SIGUSR1) == -1) {
			perror("handleJob -> kill SIGUSR1");
			exit(-1);
		}
				if (raise(SIGSTOP) == -1) {
			perror("handleJob -> raise SIGSTOP");
			exit(-1);
		}
	} else if (strcmp(j->functionName, "waitfor") == 0) {
		DEBUG(debug, printf(
				"handleJob -> SIGCONT sent to (%d) from object (%d)\n", j->pid,
				getpid()));
		kill(j->pid, SIGCONT);
	}

	/* Job executed */

}

static int pushJob(job * j) {
	int tmp;
	if (jobsNumber < FIFO_LENGTH) {
		copyJob(&jobs[firstJob], j);
		tmp = firstJob;
		firstJob = next(firstJob);
		jobsNumber++;
		DEBUG(debug, printf("addNewJob -> new job added index (%d)", tmp));
		return 0;
	} else {
		DEBUG(debug, printf("addNewJob -> too many job"));
		return -1;
	}
}

static void copyJob(job * src, job * dest) {
	strcpy(dest->functionName, src->functionName);
	dest->memIn = src->memIn;
	dest->memOut = src->memOut;
	dest->pid = dest->pid;
}

static int popJob() {
	int tmp;
	if (jobsNumber > 0) {
		tmp = lastJob;
		lastJob=next(lastJob);
		jobsNumber--;
		DEBUG(debug, printf("popJob -> job found index (%d)", lastJob));
		return lastJob;
	} else {
		DEBUG(debug, printf("popJob -> empty fifo"));
		return -1;
	}
}

static void initJobs() {
	int i;
	for (i = 0; i < BUFSIZ; ++i) {
		jobs[i].pid = -1;
	}
}

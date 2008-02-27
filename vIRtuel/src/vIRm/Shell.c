#include "Shell.h"

static char request[BUFSIZ];
static object objects[MAX_OBJECT_NUMBER];
static int pendingObjects[MAX_OBJECT_NUMBER]; //O -> pendind; 1 -> ready
static void printPrompt();
static void handleCommand(command * cmd);
static void printCommand(command * cmd);
static void initCommandStruct(command * cmd);
static void initObjectStruct(object * obj);
static void initObjectsTab();
static int getObjIndexByName(char * objectName);
static int getObjIndexByPid(int pid);
static int getFreeObjectsTabIndex();
static int initMkfifo(char * fifoName);
static void endShell();
static void sendJob(int indexObject, job * j);
static void createJob(job * j, pid_t pid, char * fun, int memIn, int memOut);
static void initPendingObjects();
static void initSigActionHandler();
static void pendingObjectHandler(int sig, siginfo_t * info, void * useless);
static int allObjectsReady();
static void sendWorkCommand(command * cmd);
static void waitAllObjects();
static void freeVar();
static void createNewObject(command * cmd);

/**
 * Displays the shell prompt. Copy the user data entered to buffer and delete
 *  possible '\n' caracter.
 *
 * @param   void
 * @return  void
 */
static void printPrompt() {
	int i = 0;
	char * bs= NULL;
	DEBUG(debug, printf("\n\n\n\n"));
	setvbuf(stdin, NULL, _IONBF, 0);
	printf(">");
	fgets(buffer, CMD_LENGTH, stdin);

	/* Search and delete '\n' caracter */
	bs = strchr(buffer, '\n');
	while (bs != NULL) {
		while (buffer[i] != '\n') {
			i++;
		}
		buffer[i] = ' ';
		bs = strchr(bs, '\n');
	}
	DEBUG(debug, printf("printPrompt -> line entered : %s\n", buffer));
}

static void handleCommand(command * cmd) {
	if (cmd != NULL) {
		if (strcmp(cmd->functionName, "work") == 0) {
			DEBUG(debug, printf("handleCommand -> work job\n"));
			sendWorkCommand(cmd);
		} else if (strcmp(cmd->functionName, "free") == 0) {
			DEBUG(debug, printf("handleCommand -> free\n"));
			freeVar();
		} else if (strcmp(cmd->functionName, "new") == 0) {
			DEBUG(debug, printf("handleCommand -> new\n"));
			createNewObject(cmd);
		} else if (strcmp(cmd->functionName, "waitall") == 0) {
			DEBUG(debug, printf("handleCommand -> waitall\n"));
			waitAllObjects();
		} else if (strcmp(cmd->functionName, "quit") == 0) {
			DEBUG(debug, printf("handleCommand -> quit\n"));
			endShell();
		}
	}
}

static void waitAllObjects() {
	int i = 0;
	job j;

	/* Send a waitall command to all objects */
	/* Objects will stop them and wait for shell to restart */
	while (objects[i].pid != -1) {
		createJob(&j, getpid(), "waitall", -1, -1);
		sendJob(i, &j);
		pendingObjects[i] = 0;
		DEBUG(debug, printf("handleCommand -> pending object (%d)\n",
			objects[i].pid));
		i++;
	}
}

static void freeVar() {
	//TODO 
}

static void createNewObject(command * cmd) {
	/* Create object only if it is new */
	if (cmd->targetObject[0] != '\0' && getObjIndexByName(cmd->targetObject)
			== -1) {
		int index = getFreeObjectsTabIndex();
		strcpy(objects[index].name, cmd->targetObject);

		if (pipe(objects[index].pipe) == -1) {
			perror("pipe");
			exit(-1);
		}

		pid_t pid = fork();
		switch (pid) {
		case -1:
			perror("fork");
			exit(-1);
		case 0:
			/* Son code */
			/* Redirect pipe out on stdin of the son */
			dup2(objects[index].pipe[0], 0);

			/* Close useless pipe in */
			close(objects[index].pipe[1]);

			if (execl("/home/akiri/Documents/workspace/vIRtuel/bin/launch", "launch",
				"POUET", "mem", NULL) == -1) {
				perror("execl");
				exit(-1);
			}
			break;
		default:
			/* Father code */
			/* Store son pid */
			objects[index].pid = pid;
			DEBUG(debug, printf(
				"handleCommand -> new object created (name=%s) (pid=%d)\n",
				objects[index].name, objects[index].pid));
			/* close useless pipe out */
			close(objects[index].pipe[0]);
			break;
		}
	}
}

static void sendWorkCommand(command * cmd) {

	int objectIndex;
	/* Object exists */
	if (cmd->objectName[0] != '\0' && ((objectIndex
			= getObjIndexByName(cmd->objectName)) != -1)) {

		int targetObjIndex = getObjIndexByName(cmd->targetObject);
		job j;
		if (cmd->waitFor == 1 && (targetObjIndex != -1)) {

			DEBUG(debug, printf("sendWorkCommand -> send SIGSTOP to (pid=%d)\n",
				objects[objectIndex].pid));

			/* Send SIGSTOP to object */
			kill(objects[objectIndex].pid, SIGSTOP);

			/* Build a job to send to target object */
			/* Target object have to wake up the object */
			createJob(&j, objects[objectIndex].pid, "waitfor", -1, -1);
			sendJob(targetObjIndex, &j);

		} else {
			DEBUG(debug, printf("sendWorkCommand -> no waitfor job\n"));
		}

		/* Build job object and send job to object */
		createJob(&j, getpid(), "work", -1, -1);
		sendJob(objectIndex, &j);
	}
	/* Object doesn't exist */
	else {
		DEBUG(debug, printf("sendWorkCommand -> object doesnt exist\n"));
		return;
	}
}

static void createJob(job * j, pid_t pid, char * fun, int memIn, int memOut) {
	j->pid = pid;
	strcpy(j->functionName, fun);
	j->memIn = memIn;
	j->memOut = memOut;
}

static void sendJob(int objectIndex, job * j) {
	/* Send job to object */
	if (write(objects[objectIndex].pipe[1], j, sizeof(job)) == -1) {
		perror("sendJob -> write");
		exit(-1);
	}
	DEBUG(
		debug,
		printf(
			"sendJob -> job sent (fun=%s) (memIn=%d) (memOut=%d) (pid=%d) to (pid=%d)\n",
			j->functionName, j->memIn, j->memOut, j->pid, objects[objectIndex].pid));

}

void launch() {
	command cmd;
	DEBUG(debug, printf("launch -> Shell pid (%d)\n", getpid()));

	if (signal(SIGINT, endShell) == SIG_ERR) {
		perror("signal");
		exit(-1);
	}

	initSigActionHandler();
	initPendingObjects();

	initSharedMemory();
	fillBloc(getFreeBloc(), "test");
	printf("(%s)", getBloc(0));

	initObjectsTab();

	//	if (initMkfifo("/home/akiri/Documents/workspace/vIRtuel/bin/montube")
	//			== MKFIFO_ERROR) {
	//		perror("mkfifo");
	//		exit(MKFIFO_ERROR);
	//	}

	while (1) {
		initCommandStruct(&cmd);
		printPrompt();
		splitCommand(&cmd);
		//		printCommand(&cmd);
		handleCommand(&cmd);
		bzero(buffer, CMD_LENGTH);
	}

}

static void endShell() {

	DEBUG(debug, printf("endShell -> end shell\n"));
	exit(EXIT_SUCCESS);
}

static void printCommand(command * cmd) {
	if (cmd != NULL) {
		printf(
			"ObjectName=%s, Function=%s, Arguments=%s, Variable=%s, TargetObject=%s, WaitFor=%d\n",
			cmd->objectName, cmd->functionName, cmd->argv, cmd->var,
			cmd->targetObject, cmd->waitFor);
	}
}

static void initCommandStruct(command * cmd) {
	cmd->argv[0] = '\0';
	cmd->functionName[0] = '\0';
	cmd->objectName[0] = '\0';
	cmd->targetObject[0] = '\0';
	cmd->var[0] = '\0';
	cmd->waitFor = 0;
}

static void initObjectStruct(object * obj) {
	obj->name[0] = '\0';
	obj->pid = -1;
}

static void initObjectsTab() {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		initObjectStruct(&objects[i]);
	}
}

static int getFreeObjectsTabIndex() {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		if (objects[i].pid == -1) {
			return i;
		}
	}
	return -1;
}
static int getObjIndexByName(char * objectName) {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		if (strcmp(objectName, objects[i].name) == 0) {
			DEBUG(debug, printf("getObjIndexByName -> object (%s) found\n",
				objectName));
			return i;
		}
	}
	DEBUG(debug, printf("getObjIndexByName -> object (%s) not found\n",
		objectName));
	return -1;
}

static int getObjIndexByPid(int pid) {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		if (objects[i].pid == pid) {
			DEBUG(debug, printf("getObjIndexByPid -> object (%d) found\n", pid));
			return i;
		}
	}
	DEBUG(debug, printf("getObjIndexByPid -> object (%d) not found\n", pid));
	return -1;
}

static int initMkfifo(char * fifoName) {
	mode_t mode= S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH;
	if (mkfifo(fifoName, mode) == -1) {
		perror("mkfifo");
		return -1;
	}
	return 0;
}

static void initPendingObjects() {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		pendingObjects[i] = -1;
	}
}

static void pendingObjectHandler(int sig, siginfo_t * info, void * useless) {
	DEBUG(debug, printf("pendingObjectHandler -> SIGUSR1 received from (%d)\n",
		info->si_pid));
	
	int index, i = 0;
	
	for (i = 0; i < 10; ++i) {
				printf("index pending object %d\n", i);
				printf("val pending object %d\n", pendingObjects[i]);
			}
	
	
	if ((index = getObjIndexByPid(info->si_pid)) != -1) {
		pendingObjects[index] = 1; // Set object to ready
		DEBUG(debug, printf("pendingObjectHandler -> object (%d) ready\n",
										info->si_pid));
		printf("pending : index %d %d\n", index, pendingObjects[index]);
	}
	if (allObjectsReady() == true) {
		//Wake up pending objects
		for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
			if (pendingObjects[i] != -1) {
				DEBUG(debug, printf("pendingObjectHandler -> send SIGCONT to (%d)\n",
								objects[i].pid));
				kill(objects[i].pid, SIGCONT);
			}
		}
		initPendingObjects();
	}

}

static int allObjectsReady() {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		if (pendingObjects[i] == 0) {
			DEBUG(debug, printf("allObjectsReady -> object not ready\n"));
			return false;
		}
	}
	DEBUG(debug, printf("allObjectsReady -> all objects are ready\n"));
	return true;
}

static void initSigActionHandler() {
	sigset_t mask;
	struct sigaction action;

	if (sigemptyset(&mask) == -1) {
		perror("initSigActionHandler -> sigemptyset");
		exit(-1);
	}

	sigaddset(&action.sa_mask, SIGUSR1);

	sigprocmask(SIG_BLOCK, &mask, NULL);

	action.sa_sigaction = pendingObjectHandler;
	action.sa_mask = mask;
	action.sa_flags = 0;
	action.sa_mask = mask;
	action.sa_flags = SA_SIGINFO;

	if (sigaction(SIGUSR1, &action, NULL) == -1) {
		perror("initSigActionHandler -> sigaction");
		exit(-1);
	}

	//	struct sigaction action;
	//	action.sa_sigaction = pendingObjectHandler;
	//	sigemptyset(&action.sa_mask);
	//	action.sa_flags = SA_SIGINFO;
	//	sigaction(SIGRTMIN, & action, NULL);
	//
	//	siginfo_t info;
	//	sigset_t sigset;
	//	sigaddset(&sigset, SIGRTMIN);
	//	sigwaitinfo(&sigset, &info);

	//		if (signal(SIGUSR1, pendingObjectHandler) == -1) {
	//			perror("signal");
	//			exit(-1);
	//		}

	//	sleep(2);
	//
	//	sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

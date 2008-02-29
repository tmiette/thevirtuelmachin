#include "Shell.h"

static object objects[MAX_OBJECT_NUMBER]; // Shell's objects
static int pendingObjects[MAX_OBJECT_NUMBER]; //O -> pendind; 1 -> ready
static char * MEM_NAME ="sharedmem"; // Shared memory file name
static char * LIB_PATH = "../objects/"; // Library path
static char * PIPE_NAMED = "pipeNamed"; // Pipe name file name
static char * LAUNCH = "launch"; // Launch executable

static void printPrompt();
static void handleCommand(command * cmd);
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
static void freeVar(command * cmd);
static void createNewObject(command * cmd);
static void freeMemSignalHandler(int sig, siginfo_t * info, void * useless);

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

/**
 * Handles a command by call corresponding function.
 * @param command * command to handle
 */
static void handleCommand(command * cmd) {
	if (cmd != NULL) {
		if (strcmp(cmd->functionName, "work") == 0) {
			DEBUG(debug, printf("handleCommand -> work job\n"));
			sendWorkCommand(cmd);
		} else if (strcmp(cmd->functionName, "free") == 0) {
			DEBUG(debug, printf("handleCommand -> free\n"));
			freeVar(cmd);
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

/**
 * Sends a waitall job structure to all objects.
 */
static void waitAllObjects() {
	int i = 0;
	job j;

	/* Send a waitall command to all objects */
	/* Objects will stop them and wait for shell to restart */
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		if (objects[i].pid != -1) {
			createJob(&j, getpid(), "waitall", -1, -1);
			sendJob(i, &j);
			pendingObjects[i] = 0;
			DEBUG(debug, printf("handleCommand -> pending object (%d)\n",
					objects[i].pid));
		}
	}
}

/**
 * Interprets a free command
 * @param command * command
 */
static void freeVar(command * cmd) {
	int targetObjIndex = getObjIndexByName(cmd->targetObject);
	job j;
	/* Get variable to free */
	int memBloc = getBlocByName(cmd->var);
	if (memBloc == -1) {
		DEBUG(debug, printf("freeVar -> invalid variable (%s)\n", cmd->var));
		return;
	}

	/* Detect a waitfor job */
	if (cmd->waitFor == 1 && (targetObjIndex != -1)) {
		DEBUG(debug, printf("freeVar -> waitfor object (%s)\n",
				cmd->targetObject));

		/* Build a job to send to target object */
		createJob(&j, getpid(), "free", memBloc, -1);
		sendJob(targetObjIndex, &j);
	} else {
		freeBloc(memBloc);
	}
}

/**
 * Interprets a new command by creating object.
 * @param command * command
 */
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

			/* Close useless pipe out */
			close(objects[index].pipe[0]);

			/* Create object's library name : objectName.so */
			char libName[NAME_LENGTH];
			strcpy(libName, LIB_PATH);
			strcat(libName, cmd->argv);
			strcat(libName, ".so");

			if (execl(LAUNCH, "launch", libName, MEM_NAME, NULL)
					== -1) {
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

/**
 * Interprets a send command by sending a job structure to objct.
 * @param command * command
 */
static void sendWorkCommand(command * cmd) {

	int objectIndex;
	/* Test if object exists */
	if (cmd->objectName[0] != '\0' && ((objectIndex
			= getObjIndexByName(cmd->objectName)) != -1)) {

		int targetObjIndex = getObjIndexByName(cmd->targetObject);
		job j;
		if (cmd->waitFor == 1 && (targetObjIndex != -1)) {

			DEBUG(debug, printf(
					"sendWorkCommand -> send SIGSTOP to (pid=%d)\n",
					objects[objectIndex].pid));

			/* Send SIGSTOP to object */
			/* This object will be prevent by target object to restart */
			kill(objects[objectIndex].pid, SIGSTOP);

			/* Build a job to send to target object */
			/* Target object have to wake up the object */
			createJob(&j, objects[objectIndex].pid, "waitfor", -1,
					getFreeBloc());
			sendJob(targetObjIndex, &j);

		} else {
			DEBUG(debug, printf("sendWorkCommand -> no waitfor job\n"));
		}

		int memIN = -1;
		int memOUT = -1;

		/* Check if variable name is already associated */
		memOUT = getBlocByName(cmd->var);
		if (memOUT == -1) {
			/* Get a free bloc memory */
			if ((memOUT = getFreeBloc()) == -1) {
				fprintf(stderr, "SharedMemory : no more space available");
				exit(-1);
			}
			/* Associate variable name to bloc number */
			nameBloc(memOUT, cmd->var);
		}

		if (strlen(cmd->argv) != 0) {
			/* Get a free bloc memory */
			if ((memIN = getFreeBloc()) == -1) {
				fprintf(stderr, "SharedMemory : no more space available");
				exit(-1);
			}

			/* Fill bloc memIN with work function arguments */
			fillBloc(memIN, cmd->argv);
		}

		/* Build job object and send it to object */
		createJob(&j, getpid(), "work", memIN, memOUT);
		sendJob(objectIndex, &j);
	}
	/* Object doesn't exist */
	else {
		DEBUG(debug, printf("sendWorkCommand -> object doesnt exist\n"));
		return;
	}
}

/**
 * Fills the given job structure with parameters.
 * @param job * job to fill
 * @param pid_t pid to set
 * @param char * function name to set
 * @param int bloc to set
 * @param int bloc to set
 */
static void createJob(job * j, pid_t pid, char * fun, int memIn, int memOut) {
	j->pid = pid;
	strcpy(j->functionName, fun);
	j->memIn = memIn;
	j->memOut = memOut;
}

/**
 * Sends a job structure to the given object
 * @param int object's index
 * @param job * job to send
 */
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
					j->functionName, j->memIn, j->memOut, j->pid,
					objects[objectIndex].pid));

}

/**
 * Launch the shell
 */
void launch() {
	command cmd;
	DEBUG(debug, printf("launch -> Shell pid (%d)\n", getpid()));
	if (signal(SIGINT, endShell) == SIG_ERR) {
		perror("signal");
		exit(-1);
	}

	initSigActionHandler();
	
	initPendingObjects();

	initSharedMemory(MEM_NAME);

	initObjectsTab();

	initMkfifo(PIPE_NAMED);

	while (1) {
		initCommandStruct(&cmd);
		printPrompt();
		splitCommand(&cmd);
		handleCommand(&cmd);
		bzero(buffer, CMD_LENGTH);
	}

}

/**
 * Wash shell when it is ending.
 */
static void endShell() {
	int i = 0;
	pid_t pid;
	DEBUG(debug, printf("endShell -> end shell\n"));

	/* Kill all my objects */
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		if ((pid = objects[i].pid) != -1) {
			kill(pid, SIGINT);
		}
	}
	unlink(PIPE_NAMED);
	exit(EXIT_SUCCESS);
}

/**
 * Initializes a command structure
 * @param command * command to intializes
 */
static void initCommandStruct(command * cmd) {
	cmd->argv[0] = '\0';
	cmd->functionName[0] = '\0';
	cmd->objectName[0] = '\0';
	cmd->targetObject[0] = '\0';
	cmd->var[0] = '\0';
	cmd->waitFor = 0;
}

/**
 * Initializes an object
 * @param object * 
 */
static void initObjectStruct(object * obj) {
	obj->name[0] = '\0';
	obj->pid = -1;
}

/**
 * Initializes a objects array
 */
static void initObjectsTab() {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		initObjectStruct(&objects[i]);
	}
}

/** 
 * returns a free index array
 */
static int getFreeObjectsTabIndex() {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		if (objects[i].pid == -1) {
			return i;
		}
	}
	return -1;
}

/**
 * Returns an object index by its name
 * @param char * object name
 * @return int object index
 */
static int getObjIndexByName(char * objectName) {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		if (objectName[0] != '\0' && strcmp(objectName, objects[i].name) == 0) {
			DEBUG(debug, printf("getObjIndexByName -> object (%s) found\n",
					objectName));
			return i;
		}
	}
	DEBUG(debug, printf("getObjIndexByName -> object (%s) not found\n",
			objectName));
	return -1;
}

/**
 * Returns an object index by its pid
 * @param char * object name
 * @return int object index
 */
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

/**
 * Created the named fifo
 * @param char * named fifo file
 */
static int initMkfifo(char * fifoName) {
	if (mkfifo(fifoName, 0666) == -1) {
		perror("mkfifo");
		return -1;
	}
	return 0;
}

/**
 * Sets pending objects to null.
 */
static void initPendingObjects() {
	int i;
	for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
		pendingObjects[i] = -1;
	}
}

/**
 * Handler called when a object is ready to be started
 */
static void pendingObjectHandler(int sig, siginfo_t * info, void * useless) {
	DEBUG(debug, printf("pendingObjectHandler -> SIGUSR1 received from (%d)\n",
			info->si_pid));

	int index, i = 0;

	if ((index = getObjIndexByPid(info->si_pid)) != -1) {
		pendingObjects[index] = 1; // Set object to ready
		DEBUG(debug, printf("pendingObjectHandler -> object (%d) ready\n",
				info->si_pid));
	}
	if (allObjectsReady() == true) {
		/* Wake up pending objects */
		for (i = 0; i < MAX_OBJECT_NUMBER; ++i) {
			if (pendingObjects[i] != -1) {
				DEBUG(debug, printf(
						"pendingObjectHandler -> send SIGUSR2 to (%d)\n",
						objects[i].pid));
				kill(objects[i].pid, SIGUSR2);
			}
		}
		/* Initialize pending objets */
		initPendingObjects();
	}

}

/**
 * Test if all object are ready
 */
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

/**
 * Handler called when an object wants to free a bloc
 */
static void freeMemSignalHandler(int sig, siginfo_t * info, void * useless) {

	/* A new shared memory bloc can be used by shell */
	if (strlen(getBlockName(info->si_value.sival_int)) == 0
			|| getBlockName(info->si_value.sival_int) == NULL) {
		DEBUG(debug, printf("freeMemSignalHandler -> free memory zone (%d)\n",
				info->si_value.sival_int));
		freeBloc(info->si_value.sival_int);
	}
}

/**
 * Handler called when an object wants to free a variable
 */
static void freeNamedBloc(int sig, siginfo_t * info, void * useless) {
	DEBUG(debug, printf("freeNamedBloc -> free memory zone (%d) named (%s)\n",
			info->si_value.sival_int, getBlockName(info->si_value.sival_int)));
	/* A new shared memory bloc can be used by shell */
	freeBloc(info->si_value.sival_int);
}

/**
 * Init process to response to signals
 */
static void initSigActionHandler() {
	sigset_t mask;
	struct sigaction action;
	struct sigaction action2;
	struct sigaction action3;

	/* Initializes the SIGUSR1 signal */
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

	/* Initializes the SIGRTMIN signal */
	if (sigemptyset(&mask) == -1) {
		perror("initSigActionHandler -> sigemptyset");
		exit(-1);
	}

	sigaddset(&action2.sa_mask, SIGRTMIN);

	sigprocmask(SIG_BLOCK, &mask, NULL);

	action2.sa_sigaction = freeMemSignalHandler;
	action2.sa_mask = mask;
	action2.sa_flags = 0;
	action2.sa_mask = mask;
	action2.sa_flags = SA_SIGINFO;

	if (sigaction(SIGRTMIN, &action2, NULL) == -1) {
		perror("initSigActionHandler -> sigaction");
		exit(-1);
	}

	/* Initializes the SIGRTMIN+1 signal */
	if (sigemptyset(&mask) == -1) {
		perror("initSigActionHandler -> sigemptyset");
		exit(-1);
	}

	sigaddset(&action3.sa_mask, SIGRTMIN+1);

	sigprocmask(SIG_BLOCK, &mask, NULL);

	action3.sa_sigaction = freeNamedBloc;
	action3.sa_mask = mask;
	action3.sa_flags = 0;
	action3.sa_mask = mask;
	action3.sa_flags = SA_SIGINFO;

	if (sigaction(SIGRTMIN+1, &action2, NULL) == -1) {
		perror("initSigActionHandler -> sigaction");
		exit(-1);
	}

}

#include "Parser.h"

static void extractVar(char ** bufferHead, command * cmd);
static void extractObjectName(char ** bufferHead, command * cmd);
static void extractFunction(char ** bufferHead, command * cmd);
static void extractFreeFun(char ** bufferHead, command * cmd);
static void extractWaitfor(char ** bufferHead, command * cmd);
static void extractNew(char ** bufferHead, command * cmd);

void splitCommand(command * cmd) {
	char * bufferHead= buffer;

	/* Extract 'free' function command */
	extractFreeFun(&bufferHead, cmd);
	/* Extract a variable if present */
	extractVar(&bufferHead, cmd);
	/* Extract an object if present */
	extractObjectName(&bufferHead, cmd);
	/* Extract waitfor command */
	extractWaitfor(&bufferHead, cmd);
	/* Extract new command */
	extractNew(&bufferHead, cmd);

	/* Extract waitall command and quit */
	if (strstr(bufferHead, "waitall")) {
		strcpy(cmd->functionName, "waitall");
		return;
	}

	/* Extract quit command and quit */
	if (strstr(bufferHead, "quit")) {
		strcpy(cmd->functionName, "quit");
		return;
	}
	return;
}

/**
 * Permits to fill command structure with a potential variable.
 * @param char ** bufferHead string pointer to split.
 * @param command * cmd command to fill.
 */
static void extractVar(char ** bufferHead, command * cmd) {
	char * pEq = strchr(*bufferHead, '=');
	char * pQuote = strchr(*bufferHead, '"');
	if ((pEq != NULL && pQuote == NULL) || (pEq != NULL && pQuote != NULL
			&& (pQuote > pEq))) {
		strcpy(trim(cmd->var), strtok(*bufferHead, "="));
		DEBUG(debug, printf("extractVar -> variable (%s)\n", cmd->var));
		(*bufferHead) = pEq + 1;
	}
	return;
}

/**
 * Permits to fill command structure with a potential object nale.
 * @param char ** bufferHead string pointer to split.
 * @param command * cmd command to fill.
 */
static void extractObjectName(char ** bufferHead, command * cmd) {
	char * pPt = strchr(*bufferHead, '.');
	char * pQuote = strchr(*bufferHead, '"');
	if ((pPt != NULL && pQuote == NULL) || (pPt != NULL && pQuote != NULL
			&& (pQuote > pPt))) {
		strcpy(cmd->objectName, strtok(*bufferHead, "."));
		DEBUG(debug, printf("extractObjectName -> object name (%s)\n",
				cmd->objectName));
		(*bufferHead) = pPt + 1;
		/* Extract an object's function */
		extractFunction(bufferHead, cmd);
	}
	return;
}

/**
 * Permits to fill command structure with a potential function name.
 * @param char ** bufferHead string pointer to split.
 * @param command * cmd command to fill.
 */
static void extractFunction(char ** bufferHead, command * cmd) {
	char * pQuote = strchr(*bufferHead, '"');
	char * p = strchr(*bufferHead, '(');
	if ((p != NULL && pQuote == NULL) || (p != NULL && pQuote != NULL
			&& (pQuote > p))) {
		strcpy(cmd->functionName, strtok(*bufferHead, "("));
		DEBUG(debug, printf("extractFunction -> function (%s)\n",
				cmd->functionName));
		(*bufferHead) = p + 1;
		/* Retrieving arguments of the function */
		char * p = strchr(*bufferHead, ')');
		if (p != NULL) {
			strcpy(trim(cmd->argv), strtok(*bufferHead, ")"));
			DEBUG(debug, printf("extractFunction -> arguments (%s)\n",
					cmd->argv));
			(*bufferHead) = p + 1;
		}
	}
	return;
}

/**
 * Permits to fill command structure with a potential free function name.
 * @param char ** bufferHead string pointer to split.
 * @param command * cmd command to fill.
 */
static void extractFreeFun(char ** bufferHead, command * cmd) {
	char * fr = "free";
	char * pQuote = strchr(*bufferHead, '"');
	char * p = strstr(*bufferHead, fr);
	if ((p != NULL && pQuote == NULL) || (p != NULL && pQuote != NULL
			&& (pQuote > p))) {
		strcpy(cmd->functionName, fr);
		DEBUG(debug, printf("extractFreeFun -> function (%s)\n",
				cmd->functionName));
		(*bufferHead) = p + strlen(fr);
		/* Extract argument of free command */
		p = strtok(*bufferHead, " ");
		if (p != NULL) {
			strcpy(trim(cmd->var), p);
			DEBUG(debug, printf("extractFreeFun -> variable (%s)\n", cmd->var));
			(*bufferHead) = p + 1;
		}
	}
	return;
}

/**
 * Permits to fill command structure with a potential waitfor pattern.
 * @param char ** bufferHead string pointer to split.
 * @param command * cmd command to fill.
 */
static void extractWaitfor(char ** bufferHead, command * cmd) {
	char * fr = "waitfor";
	char * pQuote = strchr(*bufferHead, '"');
	char * p = strstr(*bufferHead, fr);
	if ((p != NULL && pQuote == NULL) || (p != NULL && pQuote != NULL
			&& (pQuote > p))) {
		cmd->waitFor = 1;
		DEBUG(debug, printf("extractWaitfor -> waitfor (%d)\n", cmd->waitFor));
		(*bufferHead) = p + strlen(fr);
		/* Extract argument of waitfor command */
		p = strtok(*bufferHead, " ");
		if (p != NULL) {
			strcpy(cmd->targetObject, p);
			DEBUG(debug, printf("extractWaitfor -> target object found : %s\n",
					cmd->targetObject));
			(*bufferHead) = p + 1;
		}
	}
	return;
}

/**
 * Permits to fill command structure with a potential new pattern and
 * its arguments.
 * @param char ** bufferHead string pointer to split.
 * @param command * cmd command to fill.
 */
static void extractNew(char ** bufferHead, command * cmd) {
	char * token = "new";
	char * pQuote = strchr(*bufferHead, '"');
	char * p = strstr(*bufferHead, token);
	if ((p != NULL && pQuote == NULL) || (p != NULL && pQuote != NULL
			&& (pQuote > p))) {
		strcpy(cmd->functionName, token);
		DEBUG(debug, printf("extractNew -> function (%s)\n", token));
		(*bufferHead) = p + strlen(token);
		/* Extract target object name */
		p = strchr(*bufferHead, '(');
		if ((p != NULL && pQuote == NULL) || (p != NULL && pQuote != NULL
				&& (pQuote > p))) {
			strcpy(cmd->targetObject, strtok(*bufferHead, " ("));
			DEBUG(debug, printf("extractNew -> object name (%s)\n",
					cmd->targetObject));
			(*bufferHead) = p + 1;
			/* Retrieving arguments of the function */
			char * p = strchr(*bufferHead, ')');
			if (p != NULL) {
				p = strtok(*bufferHead, ")");
				strcpy(cmd->argv, trim(p));
				DEBUG(debug,
						printf("extractNew -> arguments (%s)\n", cmd->argv));
				(*bufferHead) = p + 1;
			}
		}
	}
	return;
}

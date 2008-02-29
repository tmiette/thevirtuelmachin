#include "Util.h"

char* trim(char* toTrim) {

	char* head= NULL;
	char endChar = 0;

	if (toTrim == NULL || strlen(toTrim) == 0) {
		return toTrim;
	}

	while (*toTrim == ' ') {
		toTrim++;
	}

	if (*toTrim == '"') {
		endChar = '"';
		toTrim++;
	} else {
		endChar = ' ';
	}

	head = toTrim;

	while (*toTrim != '\0' && *toTrim != endChar) {
		toTrim++;
	}

	*toTrim = '\0';

	return head;

}

char* trim2(char* toTrim) {

	char* head= NULL;
	char endChar = 0;

	if (toTrim == NULL || strlen(toTrim) == 0) {
		return toTrim;
	}

	while (*toTrim == ' ') {
		toTrim++;
	}

	if (*toTrim == '"') {
		endChar = '"';
		head = toTrim;
		toTrim++;
	} else {
		head = toTrim;
		endChar = ' ';
	}

	while (*toTrim != '\0' && *toTrim != endChar) {
		toTrim++;
	}

	if (endChar == '"') {
		toTrim++;
		*toTrim = '\0';
	} else {
		*toTrim = '\0';
	}

	return head;

}

int isDigit(char* string) {

	int i = 0;
	int lenght = 0;

	if (string == NULL) {
		return 0;
	}

	lenght = strlen(string);
	for (i = 0; i < lenght; i++) {
		if (!isdigit(string[i])) {
			return 0;
		}
	}

	return 1;

}

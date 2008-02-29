#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

void work(void* input, void* output) {

	int i = 0, j = 0, res = 0;
	int * integer= NULL;
	char op = 0;

	integer = (int*)input;
	i = integer[0];
	input += sizeof(int);

	integer = (int*)input;
	j = integer[0];
	input += sizeof(int);

	op = ((char*)input)[0];

	switch (op) {
	case '+':
		res = i + j;
		break;
	case '-':
		res = i - j;
		break;
	case '*':
		res = i * j;
		break;
	case '/':
		res = i / j;
		break;
	default:
		fprintf(stderr, "%c isn't a correct operator.", op);
		exit(EXIT_FAILURE);
		break;
	}

	fprintf(stdout, "%d %c %d = %d;", i, op, j, res);

	memcpy(output, &res, sizeof(int));

}

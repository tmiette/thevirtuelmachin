#include <string.h> 
#include <stdio.h>

void work(void* input, void* output) {
	int i1 = 0, i2 = 0, res = 0;
	char op;

	i1 = ((int*)input)[0];
	input += sizeof(int);
	i2 = ((int*)input)[1];
	input += sizeof(int);
	op = ((char*)input)[0];

	switch (op) {
	case '+':
		res = i1 + i2;
		break;
	case '-':
		res = i1 - i2;
		break;
	case '*':
		res = i1 * i2;
		break;
	case '/':
		res = i1 / i2;
		break;
	default:
		fprintf(stderr, "%c isn't a correct operator.", op);
		break;
	}

	fprintf(stdout, "%d %c %d = %d;", i1, i2, op, res);

	memcpy(output, &res, sizeof(int));

}

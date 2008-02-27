#include <stdlib.h>

void work(void* input, void* output) {
	
	char* string = NULL;
	int integer = 0;
	
	string = (char*)input;
	integer = atoi(string);
	
	((int*)output)[0] = integer;
}

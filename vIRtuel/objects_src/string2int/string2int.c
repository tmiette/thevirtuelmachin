#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void work(void* input, void* output) {
	
	char* string = NULL;
	int integer = 0;
	
	string = (char*)input;
	integer = atoi(string);
	
	memcpy(output, &integer, sizeof(int));
	
	
	printf("STRING2INT -> (%s) (%d)\n", string, integer);
}

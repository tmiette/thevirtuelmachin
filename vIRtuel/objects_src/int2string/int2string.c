#include <stdio.h> 

void work(void* input, void* output) {
	
	int* t = (int*)input;
	char* s = (char*)output;
	sprintf(s, "%d", t[0]);
	
	printf("INT2STRING -> (%d) (%s)\n", t[0], s);
}

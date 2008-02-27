#include <stdio.h> 

void work(void* input, void* output) {
	
	int* t = (int*)input;
	char* s = (char*)output;
	sprintf(s, "%d", t[0]);
}

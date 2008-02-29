#include <unistd.h>

void work(void* input, void* output) {
	
	int* t = (int*)input;
	sleep(t[0] * 1000);
	
}

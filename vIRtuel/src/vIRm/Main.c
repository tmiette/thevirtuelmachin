/**
 * @file Main.C
 * 
 * author MIETTE Tom
 * author MOURET Sebastien
 * 
 * VIRTUEL MACHIN main.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Shell.h"
#include "Util.h"

int debug = false;

int main(int argc, char **argv) {
	printf("Welcome to vIRtuel shell.\n");
	launch();
	return EXIT_SUCCESS;
}


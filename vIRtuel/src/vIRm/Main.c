/**
 ********************************************************************************
 --------------------------------------------------------------------------------
 Project : vIRtuel
 --------------------------------------------------------------------------------
 File : Main.c
 --------------------------------------------------------------------------------
 Author : Tom MIETTE - Sebastien MOURET
 --------------------------------------------------------------------------------
 Organisation : Ecole Ingenieurs 2000
 --------------------------------------------------------------------------------
 Release date : 2 févr. 08
 --------------------------------------------------------------------------------
 Description :
 This is the main file of the shell.
 --------------------------------------------------------------------------------
 ********************************************************************************
 **/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Shell.h"
#include "Util.h"

int debug = true;

void testMmap();

/**
 FUNCTION
 --------------------------------------------------------------------------------
 Prototype :
 int main(int argc, char **argv)
 --------------------------------
 Input arguments :
 Type    Name        Function
 ----------------------------
 int argc, char **argv
 --------------------------------
 Output arguments :
 Type    Name         Function
 ----------------------------
 void
 --------------------------------
 Return value :
 Type        Fonction
 ----------------------------
 int
 --------------------------------
 Description :
 Main function.
 --------------------------------------------------------------------------------
 **/
int main(int argc, char **argv) {
	printf("Welcome to vIRtuel shell.\n");
	launch();
	return EXIT_SUCCESS;
}

void testMmap() {
	int fdout;
	if ((fdout = open("ficher_temp2.txt", O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
		perror("error opening file");
		exit(EXIT_FAILURE);
	}
	write(fdout, "essai", 5);
	close(fdout);
}

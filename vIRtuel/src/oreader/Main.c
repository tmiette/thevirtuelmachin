#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Util.h"

static char * PIPE_NAMED = "pipeNamed";

int main(int argc, char **argv) {
	int fd;
	char c;

	if ((fd = open(PIPE_NAMED, 0666)) == -1) {
		perror("open mkfifo");
		exit(-1);
	}

	while (read(fd, &c, 1)) {
		write(STDOUT_FILENO, &c, 1)
			;
	}

	close(fd);

	exit(0);
}

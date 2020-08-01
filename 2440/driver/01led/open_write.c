#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int fd = -1;
	int flag = 0;
	if (3 != argc) {
		fprintf(stderr, "usege :open_write <on|off> </dev/first_dev_file>x\n");
		exit(EXIT_FAILURE);
	}
	fd = open(argv[2], O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "open error :%m\n");
		return -1;
	}
	if (0 == strcmp(argv[1], "on"))
		flag = 1;
	else if (0 == strcmp(argv[1], "off"))
		flag = 0;
	write(fd, &flag, sizeof(flag));
	return 0;
}

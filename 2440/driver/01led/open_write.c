#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
	int fd = -1;
	fd = open("/dev/first_dev_file", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "open error :%m\n");
		return -1;
	}
	write(fd, "1", 1);
	return 0;
}

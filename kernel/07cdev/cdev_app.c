#include <stdio.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include <stdlib.h>

#define DEV_FILE "/dev/test/mydev"

int main(void)
{
	int fd = -1;
	char buff[1024] = {0};

	fd = open(DEV_FILE, O_RDWR);
	if (0 > fd) {
		exit(EXIT_FAILURE);
	}
	write(fd, "123", 3);
	read(fd, buff, sizeof(buff));
	ioctl(fd, 1, 1);
	close(fd);
	return 0;
}

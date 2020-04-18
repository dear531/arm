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
	int ret = 0;

	fd = open(DEV_FILE, O_RDWR | O_NONBLOCK);
	if (0 > fd) {
		fprintf(stderr, "open error :%m\n");
		exit(EXIT_FAILURE);
	}
	ret = read(fd, buff, sizeof(buff));
	if (0 > ret) {
		fprintf(stderr, "%m\n");
	} else {
		write(1, buff, ret);
	}
	close(fd);
	return 0;
}
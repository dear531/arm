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

	fd = open(DEV_FILE, O_RDWR);
	if (0 > fd) {
		exit(EXIT_FAILURE);
	}
#if 0
	ret = write(fd, "123", 3);
	if (-1 == ret) {
		fprintf(stderr, "write %s error :%m\n", DEV_FILE);
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "write length ret :%d\n", ret);
#endif
	ret = read(fd, buff, sizeof(buff));
	fprintf(stdout, "read length ret :%d\n", ret);
	write(1, buff, ret);
#if 0
	int i = 0;
	for (i = 0; i < ret; i ++) {
		fprintf(stdout, "buff[%d]:%c\n", i, buff[i]);
	}
#endif
	ioctl(fd, 1, 1);
	close(fd);
	return 0;
}

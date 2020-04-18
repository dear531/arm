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
		fprintf(stderr, "open error :%m\n");
		exit(EXIT_FAILURE);
	}
	ret = write(fd, "123", 3);
	printf("write count :%d\n", ret);
	close(fd);
	return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int fd = -1;
	unsigned char key_values = 0;
	int i = 0;
	int ret = 0;
	if (2 != argc)
	{
		fprintf(stderr, "usage %s <dev_file_name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	fd = open(argv[1], O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "open error :%m\n");
		exit(EXIT_FAILURE);
	}
	for ( ; ; ) {
		ret = read(fd, &key_values, sizeof(key_values));
		if (-1 == ret) {
			fprintf(stderr, "read error :%m\n");
			close(fd);
			exit(EXIT_FAILURE);
		}
		fprintf(stdout, "key_values :%#x\n", key_values);
	}
	return 0;
out:
	if (0 < fd) {
		close(fd);
		fd = -1;
	}

	return -1;
}

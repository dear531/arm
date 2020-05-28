#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define DEV_FILE "/dev/test/adc"

struct adc_value_st {
	int value;
};

int main(void)
{
	int fd = -1;
	struct adc_value_st avs = {};
	int ret = 0;
	int i = 0;

	fd = open(DEV_FILE, O_RDWR);
	if (0 > fd) {
		fprintf(stderr, "open %s error :%m\n", DEV_FILE);
		exit(EXIT_FAILURE);
	}
#if 0
	for (i = 0; i < 20; i++)
#else
	for ( ; ; )
#endif
	{
		ret = read(fd, &avs, sizeof(avs));
		fprintf(stdout, "avs.vale:%d\n", avs.value);
		sleep(1);
	}
	close(fd);
	return 0;
}

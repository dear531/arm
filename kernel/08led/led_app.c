#include <stdio.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include <stdlib.h>

#define DEV_FILE "/dev/test/mydev"

struct led_pro_st {
	int led_no;
	int light;
	/* mapbit,位图表示 */
	int status;
};

int main(void)
{
	int fd = -1;
	struct led_pro_st lps = {
		.led_no = 1,
		.light = 1,
	};
	int ret = 0;

	fd = open(DEV_FILE, O_RDWR);
	if (0 > fd) {
		exit(EXIT_FAILURE);
	}
	write(fd, &lps, sizeof(lps));
	ret = read(fd, lps, sizeof(lps));
	fprintf(stdout, "%d\n", lps.status);
	ioctl(fd, 0, 0);
	close(fd);
	return 0;
}

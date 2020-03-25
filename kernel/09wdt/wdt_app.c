#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sched.h>

#define DEV_FILE "/dev/test/wdt"

struct wdt_feed_st {
	int time;
};

int main(void)
{
	int fd = -1;
	struct wdt_feed_st  wfs = {
		.time = 3000,
	};
	int ret = 0;
	struct sched_param param;

	fd = open(DEV_FILE, O_RDWR);
	if (0 > fd) {
		exit(EXIT_FAILURE);
	}

	param.sched_priority = sched_get_priority_max(SCHED_FIFO);

	sched_setscheduler(0, SCHED_FIFO, &param);
	/* init wdt */
	ioctl(fd, 0, 1);
	while (1) {
		write(fd, &wfs, sizeof(wfs));
		sleep(2);
	}

	close(fd);

	return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sched.h>

#define DEV_FILE "/dev/test/mywdt"

int main(void)
{
	int fd = -1;
	int ret = 0;
	struct sched_param param;

	fd = open(DEV_FILE, O_RDWR);
	if (0 > fd) {
		fprintf(stderr,
			"%s:%d %m\n",
			__FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

	param.sched_priority = sched_get_priority_max(SCHED_FIFO);

	sched_setscheduler(0, SCHED_FIFO, &param);
	/* init wdt */
	while (1) {
		/* 开启中断 */
		ioctl(fd, 0, 1);
		sleep(10);
		/* 关闭中断 */
		ioctl(fd, 1, 1);
		sleep(10);
	}

	close(fd);

	return 0;
}

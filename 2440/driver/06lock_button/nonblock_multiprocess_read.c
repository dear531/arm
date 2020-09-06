#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

#if 0
       #include <signal.h>
       typedef void (*sighandler_t)(int);
       sighandler_t signal(int signum, sighandler_t handler);
#endif

int fd = -1;

void sigio_handler(int signum)
{
	int ret = 0;
	unsigned char key_values = 0;
	ret = read(fd, &key_values, sizeof(key_values));
	if (-1 == ret) {
		fprintf(stderr, "read error :%m\n");
		goto out;
	}
	fprintf(stdout, "key_values :%#x\n", key_values);
out:
	return;
}

int main(int argc, char *argv[])
{
	int i = 0;
	int ret = 0;
	long flags = 0;
	if (2 != argc)
	{
		fprintf(stderr, "usage %s <dev_file_name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* 设置信号处理函数，发生信号时读 */
	signal(SIGIO, sigio_handler);

	fd = open(argv[1], O_RDONLY | O_NONBLOCK);
	if (-1 == fd) {
		fprintf(stderr, "open error :%m\n");
		exit(EXIT_FAILURE);
	}

	/* 设置本进程在fd文件描述符上接收信号事件处理 */
	fcntl(fd, F_SETOWN, getpid());
	/* 设置异步处理 */
	flags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, flags | O_ASYNC);

	flags = fcntl(fd, F_GETFL);
	fcntl(fd, F_GETFL, flags | O_NONBLOCK);

	while (1) {
		sleep(1000);
	}
	return 0;
out:
	if (0 < fd) {
		close(fd);
		fd = -1;
	}

	return -1;
}

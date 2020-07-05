#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
	struct input_event event;
	int fd = open(argv[1], O_RDONLY);
	if (2 != argc) {
		fprintf(stderr, "usage:./a.out filename\n");
		exit(EXIT_FAILURE);
	}
	while (1) {
		read(fd, &event, sizeof(event));
#if 0
struct input_event {
	struct timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
};
time
type
code
value

#endif

		fprintf(stdout,
			"time.tv_sec:%ld, time.tv_usec:%ld, type:%d, "
			"code:%d, value:%d\n",
			event.time.tv_sec, event.time.tv_usec, event.type,
			event.code, event.value);
	}
	close(fd);
	return 0;
}

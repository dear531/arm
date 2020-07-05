#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
	struct input_event event;
	int fd = -1;
	int i = 0, j = 0;
	int ret = 0;
	if (2 != argc) {
		fprintf(stderr, "usage:./a.out filename\n");
		exit(EXIT_FAILURE);
	}
	fd = open(argv[1], O_RDWR);
	if (-1 == fd) {
		fprintf(stdout, "open error :%m\n");
		exit(EXIT_FAILURE);
	}
	while (1) {
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
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 10; j++) {
				if (0 == i) {
					event.type = EV_REL;
					event.code = REL_X;
					event.value = 10;
				} else if (1 == i) {
					event.type = EV_REL;
					event.code = REL_Y;
					event.value = 10;
				} else if (2 == i) {
					event.type = EV_REL;
					event.code = REL_X;
					event.value = -10;
				} else {
					event.type = EV_REL;
					event.code = REL_Y;
					event.value = -10;
				}
				ret = write(fd, &event, sizeof(event));
				if (-1 == ret) {
					fprintf(stderr, "write error :%m\n");
					exit(EXIT_FAILURE);
				}
				event.type = EV_SYN;
				event.code = 0;
				write(fd, &event, sizeof(event));
				usleep(100000);
			}
		}
	}
	close(fd);
	return 0;
}

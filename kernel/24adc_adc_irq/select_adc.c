#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


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

	fd_set rset;

	fd = open(DEV_FILE, O_RDWR);
	if (0 > fd) {
		fprintf(stderr, "open %s error :%m\n", DEV_FILE);
		exit(EXIT_FAILURE);
	}
#if 0
       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

       void FD_CLR(int fd, fd_set *set);
       int  FD_ISSET(int fd, fd_set *set);
       void FD_SET(int fd, fd_set *set);
       void FD_ZERO(fd_set *set);
#endif
	while(1) {
		FD_ZERO(&rset);
		FD_SET(fd, &rset);
		ret = select(fd + 1, &rset, NULL, NULL, NULL);
		if (0 > ret) {
			fprintf(stderr, "select error :%m\n");
			goto select_error;
		}
		ret = read(fd, &avs, sizeof(avs));
		if (0 > ret) {
			fprintf(stderr, "read error :%m\n");
			goto read_error;
		}
		fprintf(stdout, "avs.vale:%d\n", avs.value);
		sleep(1);
	}
read_error:
select_error:
	if (0 <= fd) {
		close(fd);
		fd = -1;
	}
	return 0;
}

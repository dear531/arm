#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#if 0
       /* According to POSIX.1-2001 */
       #include <sys/select.h>

       /* According to earlier standards */
       #include <sys/time.h>
       #include <sys/types.h>
       #include <unistd.h>

       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

       void FD_CLR(int fd, fd_set *set);
       int  FD_ISSET(int fd, fd_set *set);
       void FD_SET(int fd, fd_set *set);
       void FD_ZERO(fd_set *set);

#endif


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
		fd_set rset;
		FD_SET(fd, &rset);
		ret = select(fd + 1, &rset, NULL, NULL, NULL);
		if (-1 == ret) {
			fprintf(stderr, "select error :%m\n");
			goto out;
		}
		ret = read(fd, &key_values, sizeof(key_values));
		if (-1 == ret) {
			fprintf(stderr, "read error :%m\n");
			goto out;
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

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

int myadd(int a, int b)
{
	return syscall(398, a, b);
}

#include <stdio.h>
#include <sched.h>

int main(void)
{
	while (1) {
		printf("cpu :%d\n", sched_getcpu());
		sleep(1);
	}
	return 0;
}

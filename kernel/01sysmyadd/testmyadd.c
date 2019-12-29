#include <stdio.h>
#include <sysmyadd.h>
#include "sysmyadd.h"

int main(void)
{
	int a = 1, b = 2;
	fprintf(stdout, "%d + %d = %d\n", a, b, myadd(a, b));
	return 0;
}

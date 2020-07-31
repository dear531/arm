#include <common.h>


void swi1(int no)
{
	printf("%s:%d:no:%d\n", __FILE__, __LINE__, no);
}

void swi2(int no)
{
	printf("%s:%d:no:%d\n", __FILE__, __LINE__, no);
}

void swi3(int no)
{
	printf("%s:%d:no:%d\n", __FILE__, __LINE__, no);
}

void swi4(int no)
{
	printf("%s:%d:no:%d\n", __FILE__, __LINE__, no);
}

void (*swi_array[5])(int) = {
	0,
	swi1,
	swi2,
	swi3,
	swi4,
};

void do_swi(int no)
{
	swi_array[no](no);
	return;
}

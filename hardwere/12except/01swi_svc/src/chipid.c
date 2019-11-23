#include <common.h>
#include <chipid.h>
void printf_chip_id(void)
{
	printf("id = %#x\n", PRO_ID);
}

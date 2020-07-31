#include <common.h>
#include <clock.h>

void clock_for_uart2(void)
{
	printf("CLK_DIV_PERIL0\t%#010x\n", CLK_DIV_PERIL0);
	printf("CLK_SRC_PERIL0\t%#010x\n", CLK_SRC_PERIL0);
	printf("CLK_SRC_TOP1\t%#010x\n", CLK_SRC_TOP1);
	printf("CLK_SRC_DMC\t%#010x\n", CLK_SRC_DMC);
	printf("MPLL_CON0\t%#010x\n", MPLL_CON0);

	return;
}

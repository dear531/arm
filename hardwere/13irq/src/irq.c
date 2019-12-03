#include <irq.h>
#include <common.h>
typedef void (*irq_action)(void);
irq_action ias[1024] = {0x00};

#define ICCICR_CPU0 (*(volatile unsigned int *)(0x10480000 + 0x0000))
#define ICCPMR_CPU0 (*(volatile unsigned int *)(0x10480000 + 0x0004))
#define ICCIAR_CPU0 (*(volatile unsigned int *)(0x10480000 + 0x000C))
#define ICCEOIR_CPU0 (*(volatile unsigned int *)(0x10480000 + 0x0010))
#define ICDDCR (*(volatile unsigned int *)(0x10490000 + 0x0000))
#define ICDISER1_CPU0 ((volatile unsigned int *)(0x10490000 + 0x0104))
#define ICDIPR8_CPU0 ((volatile unsigned int *)(0x10490000 + 0x0420))
#define ICDIPTR8_CPU0 ((volatile unsigned int *)(0x10490000 + 0x0820))
 
/* cpu产生终端异常跳转到该函数 */
void do_irq(void)
{
	/*
	 * inactive-->没有产生
	 * pending -->产生，等待处理
	 * active  -->正在处理
	 * inactive-->处理结束
	 */
	/* 读中断时，会使中断状态从pending变为active
	 * 从未处理变为正在处理
	 */
	int no = ICCIAR_CPU0;
	/* 1.判断中断号（中断源） */
	/* 2.去执行相应的中断处理函数 */
	ias[no & ~0x3ff]();
	/* 3.清除中断 */
	ICCEOIR_CPU0  = no;
}

void request_irq(int no, void (*handler)(void))
{
	/* 1.使能cpu接口 */
	ICCICR_CPU0 = 1;
	/* 2.使能中断分配器 */
	ICDDCR = 1;
	/* 3.设置门槛 */
	ICCPMR_CPU0 = 0xff;
	/* 4.使能设备中断 */
	ICDISER1_CPU0[no / 32 - 1] = 1 << (no % 32);
	/* 5.设置设备中断优先级 */
	ICDIPR8_CPU0[(no - 32) / 4] &= ~(0xff << ((no - 32) % 4) * 8);
	/* 6.设置目标cpu */
	ICDIPTR8_CPU0[(no - 32) / 4] &= ~(0xff << ((no - 32) % 4) * 8);
	ICDIPTR8_CPU0[(no - 32) / 4] |= 0x1 << ((no - 32) % 4) * 8;
	/* 7.设置中断处理函数 */
	ias[no] = handler;
	/* 8.开启中断(I=0) */
#if 0
	__asm__ __volatile__ (
		"mrs r0, cpsr\n"
		"bic r0, r0, #(1 << 7)\n"
		"msr r0, cpsr\n"
		:
		:
		:"r0"
	);
#endif
	__asm__ __volatile__ (
		/* cpsie enable */
		/* cpsid dnable */
		"cpsie i\n"
	);
}

#include <stdio.h>
/*
 * 1.位置相关代码反汇编
 * 通过以下命令将程序添加基地址0x5000000位置，编译成可执行程序后，反汇编
 * 得到与位置相关代码：
 * 追溯过程如下：500013c位置代码段执行将pc寄存器+32地址后取该地址中内容
 * 即程序中标注地址5000164，通过该地址取得该代码段中存储地址为05000148
 * 将其当作值存入r1中，然后又将该值作为地址，找到该地址，该地址即是nihao标号
 * 代码段中的地址，5000148中存的即是0x10，下一条地址中存的是0x20
 *
 * 简要过程:
 * ldr     r1, [pc, #32](找地址5000164内容）->  5000164:       05000148
 * ldr     r0, [r1] ->5000148:       00000010
 *
 * zhangly@u12:~/Arm/myroot/root/arm$ arm-none-linux-gnueabi-gcc -o 21location -Ttext 0x5000000 21location.c 
 * zhangly@u12:~/Arm/myroot/root/arm$ arm-none-linux-gnueabi-objdump -D 21location > 21location.s
 * 
 * 05000134 <main>:
 *  5000134:       e52db004        push    {fp}            ; (str fp, [sp, #-4]!)
 *  5000138:       e28db000        add     fp, sp, #0
 *  500013c:       e59f1020        ldr     r1, [pc, #32]   ; 5000164 <end+0x14>
 *  5000140:       e5910000        ldr     r0, [r1]
 *  5000144:       ea000001        b       5000150 <end>
 * 
 * 05000148 <nihao>:
 *  5000148:       00000010        andeq   r0, r0, r0, lsl r0
 *  500014c:       00000020        andeq   r0, r0, r0, lsr #32
 * 
 * 05000150 <end>:
 *  5000150:       e3a03000        mov     r3, #0
 *  5000154:       e1a00003        mov     r0, r3
 *  5000158:       e24bd000        sub     sp, fp, #0
 *  500015c:       e49db004        pop     {fp}            ; (ldr fp, [sp], #4)
 *  5000160:       e12fff1e        bx      lr
 *  5000164:       05000148        streq   r0, [r0, #-328] ; 0xfffffeb8
 * 
 * 2. 位置无关代码反汇编：
 * 可以看到r0直接取得pc寄存器中的值，即当前指令的下下条指令地址中的值0x10
 * 05000134 <main>:
 *  5000134:       e52db004        push    {fp}            ; (str fp, [sp, #-4]!)
 *  5000138:       e28db000        add     fp, sp, #0
 *  500013c:       e59f0000        ldr     r0, [pc]        ; 5000144 <nihao>
 *  5000140:       ea000001        b       500014c <end>
 * 
 * 05000144 <nihao>:
 *  5000144:       00000010        andeq   r0, r0, r0, lsl r0
 *  5000148:       00000020        andeq   r0, r0, r0, lsr #32
 * 
 */


int main(void)
{
	__asm__ __volatile__(
#if 0	/* 执行与代码位置无关 */
		"ldr r0, nihao\n"
#else	/* 执行与代码位置相关 */
		"ldr r1, =nihao\n"
		"ldr r0, [r1]\n"
#endif
		"b end\n"
		"nihao:\n"
		".word 0x10, 0x20\n"
		"end:\n"
		:
		:
		:"r0", "r1"
	);
	return 0;
}

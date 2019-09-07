#include <stdio.h>

int main(void)
{
	int lsl = 0xfffffff0, lsr = 0xfffffff0, asr = 0xfffffff0;
	int ror = 0xf0f0f0f0;
	fprintf(stdout, "lsl:%x, lsr:%08x, asr:%x, ror:%08x\n",
		lsl, lsr, asr, ror);
	__asm__ __volatile__ (
		/* 逻辑左移 */
		"lsl %[lsl], %[lsl], #4\n"
		/* 逻辑右移 */
		"lsr %[lsr], %[lsr], #4\n"
		/* 算术右移，即带符号位 */
		"asr %[asr], %[asr], #4\n"
		/* 循环右移，移出的部分重新移动进左侧缺少部分 */
		"ror %[ror], %[ror], #4\n"
		:[lsl]"+&r"(lsl), [lsr]"+&r"(lsr) ,[asr]"+&r"(asr) ,[ror]"+&r"(ror)
		:
		:
	);
	fprintf(stdout, "lsl:%x, lsr:%08x, asr:%x, ror:%08x\n",
		lsl, lsr, asr, ror);
	lsl = 0xfffffff0, lsr = 0xfffffff0, asr = 0xfffffff0;
	ror = 0xf0f0f0f0;
	fprintf(stdout, "lsl:%x, lsr:%08x, asr:%x, ror:%08x\n",
		lsl, lsr, asr, ror);
	__asm__ __volatile__(
		/* 第二种语法写法风格 */
		"mov %[lsl], %[lsl], lsl #4\n"
		"mov %[lsr], %[lsr], lsr #4\n"
		"mov %[asr], %[asr], asr #4\n"
		"mov %[ror], %[ror], ror #4\n"
		:[lsl]"+&r"(lsl), [lsr]"+&r"(lsr) ,[asr]"+&r"(asr) ,[ror]"+&r"(ror)
		:
		:
	);
	fprintf(stdout, "lsl:%x, lsr:%08x, asr:%x, ror:%08x\n",
		lsl, lsr, asr, ror);
	return 0;
}

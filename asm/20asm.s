.align 2 @2^n对齐，其中我们这里的n为2
.global main @声明main为全局标号，默认情况下为static类型，这里与c语言的情况不同，此操作叫符号导出

.text @代码段
main:
	@main函数之后第一件事，保存调用main函数的寄存器以及调用main代码的下一条指令
	push {r0-r12, lr}
	@printf("%s:%d\n", "nihao", 50);
	ldr r0, =s
	ldr r1, =ss
	ldr r2, =d1
	ldr r2, [r2, #8]
	bl printf
	@main函数执行结束，返回调用main的代码之前，恢复调用代码的寄存器和lr下一条指令地址
	pop {r0-r12, lr}
	bx lr
.data
d1: @通过标号数据，32数据
	.word 30,40,50,60 @相当与c语言中的int d1 = 30, d1+4 = 40, ...；
d2:
	.short 30,40,50,60 @16位数据
d3:
	.byte 30,40,50,60 @8字节
s:
	.string "%s:%d\n"
	@.ascii "%s:%d\n" @字符串类型
ss:
	.string "nihao"
	@.ascii "nihao"

.bss
@ascii类型的数据段情况，并没有加0结尾符
@00010768 <s>:
@   10768:       253a7325        ldrcs   r7, [sl, #-805]!        ; 0xfffffcdb
@   1076c:       696e0a64        stmdbvs lr!, {r2, r5, r6, r9, fp}^
@
@0001076e <ss>:
@   1076e:       6168696e        cmnvs   r8, lr, ror #18
@   10772:       Address 0x00010772 is out of bounds.
@执行效果如下，没有和后面的字符串分割开
@[@itop /root/arm]# ./20asm
@nihao:50
@nihao[@itop /root/arm]#

@string类型的则被分割开了
@执行效果如下
@[@itop /root/arm]# ./20asm
@nihao:50
@[@itop /root/arm]# 
@00010768 <s>:
@   10768:       253a7325        ldrcs   r7, [sl, #-805]!        ; 0xfffffcdb
@   1076c:       6e000a64        vmlsvs.f32      s0, s0, s9
@
@0001076f <ss>:
@   1076f:       6168696e        cmnvs   r8, lr, ror #18
@   10773:       Address 0x00010773 is out of bounds.


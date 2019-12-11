@[uboot@arm]# dwn 0x40008000
@[root@pc]# dnw arm.bin
@[uboot@arm]# go 0x40008000

@start.s hardware.c main.c
@start.o hardware.o main.o
@[ELF][s h m]
@[s h m]

.align 2
.global _start

_start:
	b reset @40008000
	ldr pc, unde @0x40008004 --bl region +/- 32M
	ldr pc, swi @0x40008008
	nop @0x4000800c
	nop @0x40008010
	nop @0x40008014
	ldr pc, irq @0x40008018
reset:
	push {r0-r12, lr}
	mrc p15, 0, r4, c1, c0, 0
	orr r4, r4, #(1 << 13)
	mcr p15, 0, r4, c1, c0, 0
	bl main
	pop {r0-r12, pc}
unde:
	.word unde
	ldr sp, =0x52100000
	push {r0-r12, lr}
	bl do_unde
	pop {r0-r12, lr}
	movs pc, lr
swi:
	.word swi
	@uboot已经初始化过swi所在的svc模式的堆栈，不用重复初始化
	@直接用已有的堆栈即可
	@ldr sp, =0x52110000
	push {r0-r12, lr}
	@APCS，ARM 过程调用标准(ARM Procedure Call Standard)
	@r0=*(lr-4)
	ldr r0, [lr, #-4]
	bic r0, r0, #0xff000000
	bl do_swi
	pop {r0-r12, lr}
	@from ARM_Architecture_Reference_Manual.pdf 
	@excetions - software interrupt
	movs pc, lr
irq:
	.word irq
	ldr sp, =0x49000000
	push {r0-r12, lr}
	bl do_irq
	pop {r0-r12, lr}
	subs pc, lr, #4 @from architecture pdf A2.6.8

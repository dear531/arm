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
reset:
	push {r0-r12, lr}
	mrc p15, 0, r4, c1, c0, 0
	orr r4, r4, #(1 << 13)
	mcr p15, 0, r4, c1, c0, 0
	bl main
	pop {r0-r12, pc}
unde:
	.word unde
	mov sp, #0x53000000
	push {r0-r12, lr}
	bl do_unde
	pop {r0-r12, lr}
	movs pc, lr

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
	b reset
reset:
	push {r0-r12, lr}
	bl main
	pop {r0-r12, pc}

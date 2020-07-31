#CC      =arm-none-linux-gnueabi-gcc
#CFLAGS  +=-march=armv7-a -nostdlib
#all:
#	arm-none-linux-gnueabi-gcc -c -nostdlib -o main.o main.c -I./include
#	arm-none-linux-gnueabi-gcc -c -nostdlib -o hardware.o hardware.c -I./include
#	arm-none-linux-gnueabi-as start.s -o start.o
#	arm-none-linux-gnueabi-ld start.o main.o hardware.o -o arm  -Ttext 0x40008000
#	#-O binary 输出为二进制文件格式
#	arm-none-linux-gnueabi-objcopy -O binary arm arm.bin
TARGET		:=arm
BIN		:=$(TARGET).bin
LD_ADDR		:=0x40008000
START_OBJ	:=start.o
OBJ		:=main.o hardware.o lib.o mmu.o
CROSS_COMPILE	:=arm-none-linux-gnueabi-
CC		:=$(CROSS_COMPILE)gcc
LD		:=$(CROSS_COMPILE)ld
AS		:=$(CROSS_COMPILE)as
OBJCOPY		:=$(CROSS_COMPILE)objcopy -O binary
OBJDUMP		:=$(CROSS_COMPILE)objdump -D
NM		:=$(CROSS_COMPILE)nm

CFLAGS		:=-I../include -march=armv7-a -nostdlib
LDFLAGS		:=

RM		:=rm -rf

all:$(TARGET)
$(TARGET):$(START_OBJ) $(OBJ)
	$(LD) $^ -o $@ -Ttext $(LD_ADDR)

clean:
	$(RM) $(TARGET) $(START_OBJ) $(OBJ) $(BIN) bin.s System.map
.PHONY:
	all clean







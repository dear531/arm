TARGET		:=arm
SRC_TARGET	:=src/$(TARGET)
BIN		:=$(TARGET).bin
CROSS_COMPILE	:=arm-none-linux-gnueabi-
CC		:=$(CROSS_COMPILE)gcc
LD		:=$(CROSS_COMPILE)ld
AS		:=$(CROSS_COMPILE)as
OBJCOPY		:=$(CROSS_COMPILE)objcopy -O binary
OBJDUMP		:=$(CROSS_COMPILE)objdump -D -S
NM		:=$(CROSS_COMPILE)nm

LDFLAGS		:=

RM		:=rm -rf

all:$(SRC_TARGET)
	$(OBJCOPY) $< $(BIN)
	$(OBJDUMP) $< > bin.s
	$(NM) $< > System.map
	#$(RM) $(TARGET)
$(SRC_TARGET):
	$(MAKE) -C src

clean:
	$(MAKE) clean -C src
	$(RM) $(SRC_TARGET) $(BIN) bin.s System.map
.PHONY:
	all clean

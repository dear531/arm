LINUX_SRC	:=/home/zhangly/Arm/itop_official/itop4412_linux_official

obj-m += irq_test.o

#obj-m += module1.o
all:kernel
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean

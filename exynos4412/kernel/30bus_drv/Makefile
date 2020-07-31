LINUX_SRC	:=/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0

obj-m += redled_dev.o ledbus.o greenled_dev.o blueled_dev.o \
redled_drv.o greenled_drv.o blueled_drv.o

#obj-m += module1.o
all:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean

LINUX_SRC	:= /home/zhangly/Arm/topeet/iTop4412_Kernel_3.0

obj-m += irq_test.o

#obj-m += module1.o
all:kernel
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean

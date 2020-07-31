LINUX_SRC	:=/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0

obj-m += button_dev.o button_drv.o

#obj-m += module1.o
all:kernel read_evnt
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
read_evnt:app.c
	arm-none-linux-gnueabi-gcc $^ -o $@	
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf read_evnt a.out

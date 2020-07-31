LINUX_SRC :=/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0

obj-m += wdt_drv.o

#obj-m += module1.o
all:kernel a.out
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf a.out
a.out:wdt_app.c
	arm-none-linux-gnueabi-gcc $^ -o $@

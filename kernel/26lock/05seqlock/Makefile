LINUX_SRC	:= /home/zhangly/Arm/topeet/iTop4412_Kernel_3.0

obj-m += cdev_test2.o

#obj-m += module1.o
all:kernel a.out
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
a.out:cdev_app.c
	arm-none-linux-gnueabi-gcc cdev_app.c
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf a.out

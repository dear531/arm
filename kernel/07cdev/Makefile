LINUX_SRC	:=/home/zhangly/Arm/itop_official/itop4412_linux_official

obj-m += cdev_test1.o cdev_test2.o

#obj-m += module1.o
all:kernel a.out
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf a.out
a.out:
	arm-none-linux-gnueabi-gcc cdev_app.c

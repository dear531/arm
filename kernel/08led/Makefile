LINUX_SRC	:=/home/zhangly/Arm/itop_official/itop4412_linux_official

obj-m += led_driver.o

#obj-m += module1.o
all:kernel a.out
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf a.out
a.out:led_app.c
	arm-none-linux-gnueabi-gcc $^ -o $@

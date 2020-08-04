LINUX_SRC	:=/home/wds2440/Arm/source/linux-2.6.22.6/

obj-m += buttom_drv.o
module-objs = buttom_drv.o

#obj-m += module1.o
all:kernel
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
open_write:%:%.c
	arm-linux-gcc $^ -o $@
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf Module.symvers open_write

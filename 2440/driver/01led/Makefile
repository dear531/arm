LINUX_SRC	:=/home/wds2440/Arm/source/linux-2.6.22.6/

obj-m += first_drv.o
module-objs = first_drv.o

#obj-m += module1.o
all:kernel open_write
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
open_write:%:%.c
	arm-linux-gcc $^ -o $@
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf Module.symvers open_write

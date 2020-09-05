LINUX_SRC	:=/home/wds2440/Arm/source/linux-2.6.22.6/

obj-m += atomic_button_drv.o
module-objs = atomic_button_drv.o
app	:=multiprocess_read

#obj-m += module1.o
all:kernel $(app)
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
$(app):%:%.c
	arm-linux-gcc $^ -o $@
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf Module.symvers $(app)

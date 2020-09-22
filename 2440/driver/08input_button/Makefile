EXTRA_CFLAGS	+= -g
LINUX_SRC	:=/home/wds2440/Arm/source/linux-2.6.22.6/

obj-m += input_button_drv.o
module-objs = input_button_drv.o

#obj-m += module1.o
all:kernel
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf Module.symvers $(app)

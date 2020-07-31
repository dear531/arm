LINUX_SRC	:=/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0

obj-m += ts_drv.o
module-objs = ts_drv.o

#obj-m += module1.o
all:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean

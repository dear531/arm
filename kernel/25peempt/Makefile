LINUX_SRC	:=/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0
user_bin	:=app

obj-m += module_test1.o
module-objs = module_test1.o

#obj-m += module1.o
all:kernel $(user_bin)
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
$(user_bin):%:%.c
	arm-none-linux-gnueabi-gcc $< -o $@
	
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf $(user_bin)

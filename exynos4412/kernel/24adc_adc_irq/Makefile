LINUX_SRC :=/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0
bin_files :=select_adc read_adc ioctl_adc
EXTRA_CFLAGS := -O1

obj-m += adc_drv.o

#obj-m += module1.o
all:kernel $(bin_files)
kernel:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
$(bin_files):%:%.c
	arm-none-linux-gnueabi-gcc $< -o $@
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean
	-rm -rf $(bin_files)


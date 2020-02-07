LINUX_SRC	:=/home/zhangly/Arm/itop_official/itop4412_linux_official

obj-m += module_mm1.o module_mm2.o module_mm3.o module_mm4.o \
	module_mm5.o
#obj-m += module1.o
all:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean

LINUX_SRC	:=/home/zhangly/Arm/itop_official/itop4412_linux_official

obj-m += module_test1.o module_test2.o

#obj-m += module1.o
all:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules
clean:
	$(MAKE)	 -C $(LINUX_SRC) M=$(shell pwd) modules clean

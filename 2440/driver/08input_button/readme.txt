注意，以下实验都是通过按开发板上的按键完成的，开发板上的按键s2，s3，s4，s5
分别被做成了l，s，shift及回车键。

在input目录下的evdev.c中找到read函数读的结构体为以下形式，
层层往下找。拆分结构体分布如下：
struct input_event {
        struct timeval time;
        __u16 type;
        __u16 code;
        __s32 value;
};

struct timeval {
	time_t		tv_sec;		/* seconds */
	suseconds_t	tv_usec;	/* microseconds */
};

typedef __kernel_time_t		time_t;
typedef long			__kernel_time_t;

typedef __kernel_suseconds_t	suseconds_t;
typedef long			__kernel_suseconds_t;

# hexdump /dev/event1 
        --秒----- --微秒--- type code --value--	 
0000000 225a 0000 48d1 000c 0001 0026 0001 0000
0000010 225a 0000 48df 000c 0000 0000 0000 0000
0000020 225a 0000 a636 000e 0001 0026 0000 0000
0000030 225a 0000 a641 000e 0000 0000 0000 0000
0000040 225f 0000 7850 0008 0001 001f 0001 0000
0000050 225f 0000 785c 0008 0000 0000 0000 0000
0000060 225f 0000 21af 000c 0001 001f 0000 0000

type值为：
#define EV_KEY                  0x01
符合定义。

按键值为：
define KEY_L                   38
十进制38，为16进制的26。

另外，在qt界面打开终端或者记事本，按下对应按键，都能得对应的结果
特别是在终端下输入ls回车，则会打印当前目录下的内容。


3.另外一种方法，关闭qt启动，借助tty设备完成验证，效果较好，但过程稍微麻烦，并且不太容易理解
借用tty_io.c实现的tty终端查看验证
当按键按下时，也能够匹配到tty的事件，tty实现也支持键盘事件。
所以在对tty进行读，也能读到相应的值，并且只是键盘码。

关闭qt启动方法为：
vi /etc/init.d/rcS
将最后一行注释掉，记得不要改错，修改时不显示光标位置
#/bin/qpe.sh &
然后重启。

重新插入模块

验证方法1：

# cat /dev/tty1
ls
ls

注意回车之后才显示内容。


验证方法2：
执行命令
exec 0</dev/tty1
意思是将tty1替换为终端的标准输入，然后再按开发板上的按键


# exec 0</dev/tty1
# ls
async_button_drv.ko         nonblock_multiprocess_read
atomic_button_drv.ko        open_read_key
block_read_key              open_write
buttom_drv.ko               poll_button_drv.ko
first_drv.ko                second_drv.ko
input_button_drv.ko         select_read
irq_buttom_drv.ko           semaphore_button_drv.ko
multiprocess_read           signal_read
nonblock_button_drv.ko      timer_button_drv.ko
# 

想恢复正常，只能重启了。


按下不动，产生重复按键

# lllllllllllllllllll

对切换终端的标准输入的查看及解释：

# ps
  PID  Uid        VSZ Stat Command
    1 0          3092 S   init     
    2 0               SW< [kthreadd]
    3 0               SWN [ksoftirqd/0]
    4 0               SW< [watchdog/0]
    5 0               SW< [events/0]
    6 0               SW< [khelper]
   55 0               SW< [kblockd/0]
   56 0               SW< [ksuspend_usbd]
   59 0               SW< [khubd]
   61 0               SW< [kseriod]
   73 0               SW  [pdflush]
   74 0               SW  [pdflush]
   75 0               SW< [kswapd0]
   76 0               SW< [aio/0]
  710 0               SW< [mtdblockd]
  745 0               SW< [kmmcd]
  770 0          3096 S   -sh 
  774 0          3096 R   ps 
# ls /proc/770/fd -l  
lrwx------    1 0        0              64 Jan  1 00:00 0 -> /dev/s3c2410_serial0
lrwx------    1 0        0              64 Jan  1 00:00 1 -> /dev/s3c2410_serial0
lrwx------    1 0        0              64 Jan  1 00:00 10 -> /dev/tty
lrwx------    1 0        0              64 Jan  1 00:00 2 -> /dev/s3c2410_serial0



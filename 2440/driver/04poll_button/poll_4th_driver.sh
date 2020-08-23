# cat /proc/devices  |grep buttom        
252 buttom_drv
# ls /sys/class/buttom_drv_class/buttom_class_dev/
dev        subsystem  uevent
# ls /dev/buttom_class_dev -l
crw-rw----    1 0        0        252,   0 Jan  1 02:01 /dev/buttom_class_dev
# 
#./open_read_key  /dev/buttom_class_dev 
key_values[3]:0
key_values[3]:0
key_values[3]:0
key_values[3]:0

## 打开文件描述符
$ exec 5</dev/irq_buttom_class_de
buttom_open:/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c
## 查看当前终端进程号，sh为836
$ ps
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
  771 0         15984 S   /opt/Qtopia/bin/qpe
  785 0          9044 S < /opt/Qtopia/bin/qss
  786 0         11492 S N /opt/Qtopia/bin/quicklauncher
  836 0          3096 S   -sh
  838 0          3096 R   ps
# ls /proc/836/fd -l
lrwx------    1 0        0              64 Jan  1 03:04 0 -> /dev/s3c2410_serial0
lrwx------    1 0        0              64 Jan  1 03:04 1 -> /dev/s3c2410_serial0
lrwx------    1 0        0              64 Jan  1 03:04 10 -> /dev/tty
lrwx------    1 0        0              64 Jan  1 03:04 2 -> /dev/s3c2410_serial0
lr-x------    1 0        0              64 Jan  1 03:04 5 -> /dev/irq_buttom_class_de
## 关闭文件描述符
$ exec 5<&-
## 重新绑定的文件描述符被关闭了
$ ls /proc/836/fd -l
lrwx------    1 0        0              64 Jan  1 03:04 0 -> /dev/s3c2410_serial0
lrwx------    1 0        0              64 Jan  1 03:04 1 -> /dev/s3c2410_serial0
lrwx------    1 0        0              64 Jan  1 03:04 10 -> /dev/tty
lrwx------    1 0        0              64 Jan  1 03:04 2 -> /dev/s3c2410_serial0
#    

##查看注册的中断
$ cat /proc/interrupts 
           CPU0
 16:          0    s3c-ext0  IRQ_EINT0
 18:          0    s3c-ext0  IRQ_EINT2
 30:    2480603         s3c  S3C2410 Timer Tick
 32:          0         s3c  s3c2410-lcd
 33:          0         s3c  s3c-mci
 34:          0         s3c  I2SSDI
 35:          0         s3c  I2SSDO
 37:         12         s3c  s3c-mci
 42:          0         s3c  ohci_hcd:usb1
 43:          0         s3c  s3c2440-i2c
 51:        889     s3c-ext  eth0
 55:          0     s3c-ext  IRQ_EINT11
 60:          0     s3c-ext  s3c-mci
 63:          0     s3c-ext  IRQ_EINT19
 70:       2255   s3c-uart0  s3c2440-uart
 71:       4214   s3c-uart0  s3c2440-uart
 79:          0     s3c-adc  s3c2410_action
 80:          0     s3c-adc  s3c2410_action
 83:          0           -  s3c2410-wdt
Err:          0


# ./block_read_key /dev/irq_buttom_class_de  &
# buttom_open:/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c

# 
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
  771 0         15984 S   /opt/Qtopia/bin/qpe 
  772 0          3096 S   -sh 
  785 0          9044 S < /opt/Qtopia/bin/qss 
  786 0         12632 S   datebook                      
  792 0         11496 S N /opt/Qtopia/bin/quicklauncher 
  793 0          1308 S   ./block_read_key /dev/irq_buttom_class_de  ##sleep状态
  794 0          3096 R   ps 
# 
# 
# /home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c:86:buttom_irq:i
rq:18
key_values :0x2
/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c:86:buttom_irq:irq
:18
key_values :0x82
/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c:86:buttom_irq:irq
:16
key_values :0x1
/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c:86:buttom_irq:irq
:16
key_values :0x81
/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c:86:buttom_irq:irq
:55
key_values :0x3
/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c:86:buttom_irq:irq
:55
key_values :0x83
/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c:86:buttom_irq:irq
:63
key_values :0x4
/home/wds2440/Arm/arm/2440/driver/03irq_buttom/irq_buttom_drv.c:86:buttom_irq:irq
:63
key_values :0x84


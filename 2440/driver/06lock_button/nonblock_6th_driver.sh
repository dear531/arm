# insmod  nonblock_button_drv.ko 
nonblock_button_init:/home/wds2440/Arm/arm/2440/driver/06lock_button/nonblock_button_
drv.c
# ./nonblock_multiprocess_read /dev/nonblock_button_dev &
# button_open:/home/wds2440/Arm/arm/2440/driver/06lock_button/nonblock_button_drv.c

# 
# ./nonblock_multiprocess_read /dev/nonblock_button_dev &
# button_open:/home/wds2440/Arm/arm/2440/driver/06lock_button/nonblock_button_drv.c
open error :Resource temporarily unavailable

[2] + Done(1)                    ./nonblock_multiprocess_read /dev/nonblock_button_de
v
# /home/wds2440/Arm/arm/2440/driver/06lock_button/nonblock_button_drv.c:132:button_ir
q:irq:16
key_values :0x1
/home/wds2440/Arm/arm/2440/driver/06lock_button/nonblock_button_drv.c:132:button_irq:
irq:16
key_values :0x81
/home/wds2440/Arm/arm/2440/driver/06lock_button/nonblock_button_drv.c:132:button_irq:
irq:18
key_values :0x2
/home/wds2440/Arm/arm/2440/driver/06lock_button/nonblock_button_drv.c:132:button_irq:
irq:18
key_values :0x82

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
  786 0         11492 S N /opt/Qtopia/bin/quicklauncher 
  793 0          1312 S   ./nonblock_multiprocess_read /dev/nonblock_button_dev
  795 0          3096 R   ps 


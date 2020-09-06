# ./multiprocess_read  /dev/semaphore_button_de  &
# button_open:/home/wds2440/Arm/arm/2440/driver/06lock_button/semaphore_button_drv.c

# ./multiprocess_read  /dev/semaphore_button_de  &
button_open:/home/wds2440/Arm/arm/2440/driver/06lock_button/semaphore_button_drv.c
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
  786 0         11492 S N /opt/Qtopia/bin/quicklauncher 
  822 0          1308 S   ./multiprocess_read /dev/semaphore_button_de 
  823 0          1308 D   ./multiprocess_read /dev/semaphore_button_de 
  824 0          3096 R   ps 
## 其中ps命令显示的D状态为不可中断的睡眠状态
# kill 822
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
  823 0          1308 S   ./multiprocess_read /dev/semaphore_button_de 
  825 0          3096 R   ps 
[1] - Terminated                 ./multiprocess_read /dev/semaphore_button_de
# /home/wds2440/Arm/arm/2440/driver/06lock_button/semaphore_button_drv.c:116:button_5
key_values :0x3
/home/wds2440/Arm/arm/2440/driver/06lock_button/semaphore_button_drv.c:116:button_ir5
key_values :0x83


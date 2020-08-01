#/bin/bash
##  $ cat /proc/devices |grep first
##  252 first_drv
##  $ cat /sys/class/first_drv_class/first_dev_file/dev
##  252:0
##  $ ls /dev/first_dev_file -l
##  crw-rw----    1 0        0        252,   0 Jan  1 03:08 /dev/first_dev_file
##  $ ./open_read
##  first_open:/home/wds2440/Arm/arm/2440/driver/01led/first_drv.c:7
##  first_write:/home/wds2440/Arm/arm/2440/driver/01led/first_drv.c:13

cat /proc/devices |grep first
cat /sys/class/first_drv_class/first_dev_file/dev
ls /dev/first_dev_file -l
./open_read

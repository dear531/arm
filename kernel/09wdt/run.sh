#! /bin/bash
#[root@iTOP-4412]# insmod wdt_drv.ko 
#[   65.806972] finished cdev_add
#[   65.808544] wdt->v:ef7b0000
#[root@iTOP-4412]# cat /proc/devices |grep my
# 248 mywdt
#[root@iTOP-4412]# mkdir /dev/test
#[root@iTOP-4412]# mknod /dev/test/mywdt c 248 1
#[root@iTOP-4412]# chmod +x a.out 
#[root@iTOP-4412]# ./a.out 
# rm -rf /dev/test/


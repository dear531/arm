zhangly@u12:arch$ cd ..
zhangly@u12:iTop4412_Kernel_3.0$ arm-none-linux-gnueabi-addr2line
c0226178 -e vmlinux  -f
strcmp
/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0/lib/string.c:248
zhangly@u12:iTop4412_Kernel_3.0$ arm-none-linux-gnueabi-addr2line
c02211a0 -e vmlinux  -f
kset_find_obj_hinted
/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0/lib/kobject.c:765
zhangly@u12:iTop4412_Kernel_3.0$ arm-none-linux-gnueabi-addr2line
c0221288 -e vmlinux  -f
kset_find_obj
/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0/lib/kobject.c:750
zhangly@u12:iTop4412_Kernel_3.0$ arm-none-linux-gnueabi-addr2line
c0221210 -e vmlinux  -f
kset_find_obj_hinted
/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0/lib/kobject.c:787
zhangly@u12:iTop4412_Kernel_3.0$ arm-none-linux-gnueabi-addr2line
c022618c -e vmlinux  -f
strcmp
/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0/lib/string.c:249
zhangly@u12:iTop4412_Kernel_3.0$ arm-none-linux-gnueabi-addr2line
c0221210 -e vmlinux  -f
kset_find_obj_hinted
/home/zhangly/Arm/topeet/iTop4412_Kernel_3.0/lib/kobject.c:787

如果word文件无法在linux打开，就复制到windows系统下打开察看内容。


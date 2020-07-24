##   内核模块调试方法：
##   先监控insmod命令调用的系统调用sys_init_module函数
##   内核模块插入停止之后，再监控想要监控的有问题函数，或是逐步监控调试的函数

zhangly@u12:iTop4412_Kernel_3.0$ arm-none-linux-gnueabi-gdb vmlinux -tui
Error opening terminal: xterm.
zhangly@u12:iTop4412_Kernel_3.0$ arm-none-linux-gnueabi-gdb vmlinux
GNU gdb (Sourcery CodeBench Lite 2014.05-29) 7.7.50.20140217-cvs
Copyright (C) 2014 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later
<http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show
copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-pc-linux-gnu
--target=arm-none-linux-gnueabi".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://sourcery.mentor.com/GNUToolchain/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from vmlinux...done.
(gdb) set serial baud 115200
(gdb) set target-re
Display all 499 possibilities? (y or n)
(gdb) set remote   
remote             remotecache        remotelogbase      remotetimeout
remoteaddresssize  remoteflow         remotelogfile      remotewritesize
(gdb) set remote localhost:2331         
Undefined set remote command: "localhost:2331".  Try "help set remote".
(gdb) target remote loca
local_bh_disable       local_cpu_mask         local_port_range_lock
local_t
local_bh_enable        local_exit             local_port_range_max
local_timer_ack
local_bh_enable_ip     local_init             local_port_range_min
local_timer_setup
local_clock            local_port_range       local_ports
locate_module_kobject
(gdb) target remote locahost:2331
locahost: unknown host
locahost:2331: 没有那个文件或目录.
(gdb) target remote localhost:2331
Remote debugging using localhost:2331
0xc01327e4 in check_poison_mem (mem=<optimized out>, bytes=<optimized
out>)
    at mm/debug-pagealloc.c:66
66			if (*start != PAGE_POISON)
(gdb) break sys_init_module 
Breakpoint 1 at 0xc00cf658: file kernel/module.c, line 2895.
(gdb) c     
Continuing.

Breakpoint 1, sys_init_module (umod=0xef040, len=64680, 
    uargs=0xe3358 <error: Cannot access memory at address 0xe3358>) at
kernel/module.c:2895
2895	{
(gdb) break kset_find_obj_hinted
Breakpoint 2 at 0xc02211ac: file lib/kobject.c, line 765.
(gdb) c
Continuing.

Breakpoint 2, kset_find_obj_hinted (kset=0xd6138040, name=0xbf0001d8
"ts_drv", hint=0x0)
    at lib/kobject.c:765
765	{
(gdb) where
#0  kset_find_obj_hinted (kset=0xd6138040, name=0xbf0001d8 "ts_drv",
hint=0x0)
    at lib/kobject.c:765
#1  0xc0221288 in kset_find_obj (kset=0xd6138040, name=0xbf0001d8
"ts_drv")
    at lib/kobject.c:749
#2  0xc00cef98 in mod_sysfs_init (mod=<optimized out>) at
kernel/module.c:1472
#3  mod_sysfs_setup (num_params=<optimized out>, kparam=<optimized out>, 
    info=<optimized out>, mod=<optimized out>) at kernel/module.c:1501
#4  load_module (umod=<optimized out>, len=<optimized out>,
uargs=<optimized out>)
    at kernel/module.c:2843
#5  0xc00cf694 in sys_init_module (umod=0xef040, len=64680,
uargs=0xe3358 "")
    at kernel/module.c:2904
#6  0xc0060340 in ?? ()
Backtrace stopped: frame did not save the PC
(gdb) up 
#1  0xc0221288 in kset_find_obj (kset=0xd6138040, name=0xbf0001d8
"ts_drv")
    at lib/kobject.c:749
749		return kset_find_obj_hinted(kset, name, NULL);
(gdb) list
744	 * looking for a matching kobject. If matching object is found
745	 * take a reference and return the object.
746	 */
747	struct kobject *kset_find_obj(struct kset *kset, const char
*name)
748	{
749		return kset_find_obj_hinted(kset, name, NULL);
750	}
751	
752	/**
753	 * kset_find_obj_hinted - search for object in kset given a
predecessor hint.
(gdb) down
#0  kset_find_obj_hinted (kset=0xd6138040, name=0xbf0001d8 "ts_drv",
hint=0x0)
    at lib/kobject.c:765
765	{
(gdb) up 
#1  0xc0221288 in kset_find_obj (kset=0xd6138040, name=0xbf0001d8
"ts_drv")
    at lib/kobject.c:749
749		return kset_find_obj_hinted(kset, name, NULL);
(gdb) up
#2  0xc00cef98 in mod_sysfs_init (mod=<optimized out>) at
kernel/module.c:1472
1472		kobj = kset_find_obj(module_kset, mod->name);
(gdb) where
#0  kset_find_obj_hinted (kset=0xd6138040, name=0xbf0001d8 "ts_drv",
hint=0x0)
    at lib/kobject.c:765
#1  0xc0221288 in kset_find_obj (kset=0xd6138040, name=0xbf0001d8
"ts_drv")
    at lib/kobject.c:749
#2  0xc00cef98 in mod_sysfs_init (mod=<optimized out>) at
kernel/module.c:1472
#3  mod_sysfs_setup (num_params=<optimized out>, kparam=<optimized out>, 
    info=<optimized out>, mod=<optimized out>) at kernel/module.c:1501
#4  load_module (umod=<optimized out>, len=<optimized out>,
uargs=<optimized out>)
    at kernel/module.c:2843
#5  0xc00cf694 in sys_init_module (umod=0xef040, len=64680,
uargs=0xe3358 "")
    at kernel/module.c:2904
#6  0xc0060340 in ?? ()
Backtrace stopped: frame did not save the PC
(gdb) c  
Continuing.

Breakpoint 2, kset_find_obj_hinted (kset=0xd612fc80, name=0x0, hint=0x0)
at lib/kobject.c:765
765	{
(gdb) where
#0  kset_find_obj_hinted (kset=0xd612fc80, name=0x0, hint=0x0) at
lib/kobject.c:765
#1  0xc0221288 in kset_find_obj (kset=0xd612fc80, name=0x0) at
lib/kobject.c:749
#2  0xc0268f28 in driver_find (name=0x0, bus=<optimized out>) at
drivers/base/driver.c:282
#3  0xc0269060 in driver_register (drv=0xbf000138) at
drivers/base/driver.c:235
#4  0xc02e4f5c in i2c_register_driver (owner=<optimized out>,
driver=0xbf000110)
    at drivers/i2c/i2c-core.c:1139
#5  0xbf002028 in ?? ()
Backtrace stopped: frame did not save the PC
(gdb)    


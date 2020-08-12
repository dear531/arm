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



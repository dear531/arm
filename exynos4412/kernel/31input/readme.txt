cat /proc/bus/input/devices |grep Handlers 
H: Handlers=kbd event0 
H: Handlers=sysrq kbd event1 
H: Handlers=mouse0 event2 
H: Handlers=mouse1 event3 
sudo ./a.out /dev/input/event3

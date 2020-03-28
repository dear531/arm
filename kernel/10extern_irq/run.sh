#! /bin/bash
mkdir /dev/test
mknod /dev/test/mywdt c 246 2
./a.out
# rm -rf /dev/test/


#! /bin/bash
mkdir /dev/test
mknod /dev/test/mydev c 248 2
./a.out
# rm -rf /dev/test/


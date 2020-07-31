#! /bin/bash
mkdir /dev/test
mknod /dev/test/adc c 248 2
./a.out
# rm -rf /dev/test/


testmyadd:libsysmyadd.so testmyadd.c
	arm-none-linux-gnueabi-gcc -L. -lsysmyadd -o testmyadd -I. testmyadd.c 
libsysmyadd.so:sysmyadd.o
	arm-none-linux-gnueabi-gcc -shared -o $@ $^
sysmyadd.o:sysmyadd.c
	arm-none-linux-gnueabi-gcc -fPIC -c -o $@ $<
clean:
	-rm -rf *.o *.so testmyadd
.PHONY:
	clean

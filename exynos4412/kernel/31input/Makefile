target	:=read_evnt remove_mouse
all:$(target)
read_evnt:app.c
	arm-none-linux-gnueabi-gcc $^ -o $@
remove_mouse:remove_mouse.c
	arm-none-linux-gnueabi-gcc $^ -o $@
clean:
	-rm -rf $(target)
.PHONY:
	all clean

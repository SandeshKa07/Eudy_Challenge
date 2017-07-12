CFLAGS_helloworld.o = -DDEBUG

obj-m := my_usb_driver.o 

all:
	make -C /usr/src/linux-headers-`uname -r`/  M=`pwd` modules
clean:
	rm -rf Module.symvers *.mod.c *.o *.mod.o *.order *.ko

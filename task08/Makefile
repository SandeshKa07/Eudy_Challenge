obj-m := helloworld.o

all:
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` modules
clean:
	rm -rf *.o *.mod.o *.mod.c *.ko modules.order *sym*

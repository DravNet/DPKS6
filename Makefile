
ifneq ($(KERNELRELEASE),)
# debug info
ccfalgs-y += -g
# kbuild part of makefile
obj-m   := hello.o
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello.ko hello.ko.unstripped
	$(CROSS_COMPILE)strip -g hello.ko
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif

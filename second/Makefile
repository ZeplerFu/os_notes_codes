ifneq	($(KERNELRELEASE),)
	obj-m := lab2_1.o
    
else
	KERNELDIR=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
    
default:
	$(MAKE)	-C	$(KERNELDIR)	M=$(PWD) modules

endif

ifeq ($(KERNELRELEASE),)

KERNEL_SOURCE := /home/arun/Github/Tutorial_BeagleBoneBlack/Embitude/linux-4.19.103
PWD := $(shell pwd)

ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-

default:
	$(MAKE) -C $(KERNEL_SOURCE)	M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNEL_SOURCE)	M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean

else

obj-m := my_ldd_driver.o

endif
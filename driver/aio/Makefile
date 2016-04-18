obj-m := cpsaio.o

#KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

#cross build 
CROSS_COMPILE=arm-linux-gnueabi-
ARCH=arm
KERNELDIR ?= /home/contec/workspace/sdk/CPS_SDK/kernel #target kernel source path

# double and float calc option (soft-float version)
KBUILD_CFLAGS += -mfloat-abi=softfp
KBUILD_AFLAGS += -mfloat-abi=softfp

EXTRA_CFLAGS	:= -DDEBUG=0

#cps-system extra symbol path
EXTRA_SYMBOL	:= $(PWD)/../system/Module.symvers

all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) KBUILD_EXTRA_SYMBOLS=$(EXTRA_SYMBOL) M=$(PWD) modules
#	$(MAKE) -C $(KERNELDIR) KBUILD_EXTRA_SYMBOLS=$(EXTRA_SYMBOL) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

release_copy:
	cp ./cpsaio.ko ../release/

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions vtty Module.symvers modules.order 

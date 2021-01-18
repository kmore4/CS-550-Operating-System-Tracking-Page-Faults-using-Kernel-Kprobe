
EXTRA_CFLAGS = -Wall -Werror -Wframe-larger-than=4000

ifeq ($(KERNELRELEASE),)

	KERNELDIR ?= /lib/modules/$(shell uname -r)/build

	PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.order *.symvers

.PHONY: modules modules_install clean

else
	obj-m := pf_probe_A1.o
	obj-m += pf_probe_B1.o

endif

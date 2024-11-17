
obj-m = linux_module.o
obj-m += mod_kmalloc.o
obj-m += mod_tasklet_timing.o
obj-m += mod_delayed_workqueue.o

KVERSION = $(shell uname -r)

modules all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

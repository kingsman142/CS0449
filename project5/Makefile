obj-m := e_driver.o 

KDIR  := /u/SysLab/shared/linux-2.6.23.1
PWD   := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -m32 -o e_digits e_digits.c

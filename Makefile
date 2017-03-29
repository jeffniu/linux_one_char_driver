# makefile: makefile of our first driver
# if KERNELRELEASE is defined, we've been invoked from the 
# kernel build system and can use its language.

ifneq (${KERNELRELEASE},)
    obj-m := hello_module.o one_char_driver.o
# Otherwise we were called directly from the command line. 
# Invoke the kernel build system.
else
	KERNEL_SOURCE := /home/niuguangyuan/Downloads/linux-4.6.3/
	PWD := $(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules
clean:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
endif

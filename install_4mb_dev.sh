rm /dev/scull
make clean
make
mknod /dev/scull c 62 0
insmod four_mb_driver.ko


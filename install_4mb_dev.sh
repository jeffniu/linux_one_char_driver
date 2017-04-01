rm /dev/lcd
make clean
make
mknod /dev/lcd c 62 0
insmod four_mb_driver.ko


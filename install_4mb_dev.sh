rm /dev/four
make clean
make
mknod /dev/four c 62 0
insmod four_mb_driver.ko


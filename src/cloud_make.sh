#! /bin/bash

cd "/home/ardunet/ESP8266_SDK_RTOS/src"

PATH=/home/ardunet/crosstool-NG/builds/xtensa-lx106-elf/bin:$PATH
PATH=/home/ardunet/esptool/:$PATH

export PATH

make
code=$?
if [ $code -ne 0 ] ; then
    exit $code
fi
./gen_misc.sh
exit $?
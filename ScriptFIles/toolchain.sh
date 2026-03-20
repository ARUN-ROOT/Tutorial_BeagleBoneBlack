#!/bin/bash

sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
arm-linux-gnueabihf-gcc --version


echo "========================================================="
echo " BeagleBone Black Toolchain installed Successfull"
echo "========================================================="
echo "arm-linux-gnueabihf-gcc your_file.c -o output"
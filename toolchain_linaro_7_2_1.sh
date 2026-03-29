#!/bin/bash

# Toolchain setup for BeagleBone Black

export PATH=$PATH:/home/arun/Github/Arun_Porting/Testing/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabihf/bin
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

echo "Toolchain configured:"
which ${CROSS_COMPILE}gcc



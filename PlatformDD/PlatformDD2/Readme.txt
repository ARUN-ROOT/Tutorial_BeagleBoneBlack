      
      Platform Device Driver + Character Device Driver DTS + GPIO Skeleton Model
------------------------------------------------------------------------------------

1: write the Device Driver code .
2: Makefile 
ifeq ($(KERNELRELEASE),)

KERNEL_SOURCE := /home/arun/Github/Tutorial_BeagleBoneBlack/Embitude/linux-4.19.103
PWD := $(shell pwd)

ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-

default:
	$(MAKE) -C $(KERNEL_SOURCE)	M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNEL_SOURCE)	M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean

else

obj-m := my_ldd_driver.o

endif

3: Add device node inside Main node of am335x-boneblack.dts
gedit arch/arm/boot/dts/am335x-boneblack.dts

or 

open with text editor arch/arm/boot/dts/am335x-boneblack.dts

/dts-v1/;

#include "am33xx.dtsi"
#include "am335x-bone-common.dtsi"
#include "am335x-boneblack-common.dtsi"

/ {
    model = "TI AM335x BeagleBone Black";
    compatible = "ti,am335x-bone-black", "ti,am335x-bone", "ti,am33xx";

    /* Custom Platform Device Node */
    my_ldd_device {
        compatible = "my-ldd";
        status = "okay";
        led-gpio = <&gpio2 3 GPIO_ACTIVE_HIGH>;
    };
};

&cpu0_opp_table {
    /*
     * All PG 2.0 silicon may not support 1GHz but some of the early
     * BeagleBone Blacks have PG 2.0 silicon which is guaranteed
     * to support 1GHz OPP so enable it for PG 2.0 on this board.
     */
    oppnitro-1000000000 {
        opp-supported-hw = <0x06 0x0100>;
    };
};

&usb0 {
    interrupt-names = "mc";
};

4: make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs
5: Connect BBB to Host 
   Try ping 192.168.7.2 
   Not Happen
      sudo ifconfig enxe415f6f91296 192.168.7.1 netmask 255.255.255.0 up
      ping 192.168.7.2
      PING 192.168.7.2 (192.168.7.2) 56(84) bytes of data.
      64 bytes from 192.168.7.2: icmp_seq=1 ttl=64 time=1.00 ms
      64 bytes from 192.168.7.2: icmp_seq=2 ttl=64 time=0.386 ms
   
   scp arch/arm/boot/dts/am335x-boneblack.dtb root@192.168.7.2:/boot/
6: reboot

7: scp my_ldd_driver.ko root@192.168.7.2:/root/
8: sync
9: insmod my_ldd_driver.ko
10: dmesg | grep Probe
11: ls /dev/my_ldd
12: ls /proc/device-tree/
13: ls /proc/device-tree/my_ldd_device
14: echo 1 > /dev/my_ldd
15: echo 0 > /dev/my_ldd

13: rmmod my_ldd_driver

Output :
---------
# ls
my_ldd_driver.ko
#  insmod my_ldd_driver.ko
[  550.646439] my_ldd_driver: loading out-of-tree module taints kernel.
[  550.655014] Probe called
[  550.657866] GPIO number = 67
[  550.661861] Character device created
# 
# ls /dev/my_ldd
/dev/my_ldd
# dmesg | grep Probe
[  550.655014] Probe called
#  ls /proc/device-tree/
#address-cells    compatible        model             serial-number
#size-cells       cpus              my_ldd_device     soc
aliases           fixedregulator0   name              sound
chosen            interrupt-parent  ocp
clk_mcasp0        leds              opp-table
clk_mcasp0_fixed  memory@80000000   pmu@4b000000
# ls /proc/device-tree/my_ldd_device
compatible  led-gpio    name        status
# echo 1 > /dev/my_ldd
[  674.249743] Device opened
[  674.252523] LED ON
[  674.255423] Device closed
# echo 0 > /dev/my_ldd
[  686.969541] Device opened
[  686.972238] LED OFF
[  686.975121] Device closed
# rmmod  my_ldd_driver
[ 1225.132729] Driver removed

======================

  dmesg :
-------------
[    4.380667] 8021q: adding VLAN 0 to HW filter on device eth0
[    4.536872] configfs-gadget gadget: high-speed config #1: c
[    4.542791] IPv6: ADDRCONF(NETDEV_CHANGE): usb0: link becomes ready
[  550.646439] my_ldd_driver: loading out-of-tree module taints kernel.
[  550.655014] Probe called
[  550.657866] GPIO number = 67
[  550.661861] Character device created
[  674.249743] Device opened
[  674.252523] LED ON
[  674.255423] Device closed
[  686.969541] Device opened
[  686.972238] LED OFF
[  686.975121] Device closed
[ 1225.132729] Driver removed
# 


Now,After rm module then only we can use that gpio67 pin otherwise it will show busy...
# ls /sys/class/gpio
export      gpiochip0   gpiochip32  gpiochip64  gpiochip96  unexport
# 
echo 67 > /sys/class/gpio/export

echo out > /sys/class/gpio/gpio67/direction

echo 1 > /sys/class/gpio/gpio67/value

sleep 1

echo 0 > /sys/class/gpio/gpio67/value

echo 67 > /sys/class/gpio/unexport


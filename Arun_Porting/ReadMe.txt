All are Upload in Google Drive :
-----------------------------------------
am335x-evm-linux-sdk-bin-05.02.00.10.tar.xz
am335x-evm-linux-sdk-src-05.02.00.10.tar.xz
gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabihf.tar.xz
tisdk-rootfs-image-am335x-evm.tar.xz
 =============================================
 Packages:
 ------------
 	sudo sh elinux_pkg.sh
 	
 Install ARM Toolchain
 ------------------------
 	Extract Toolchain
 	click go to bin folder ,copy the path(click ctrl+L)
 	sudo gedit .bashrc 
 	export PATH=$PATH:__________Paste the Path___________
 	save it .
 	In Terminal , check arm-linux-gnueabihf-gcc -v
 	
 Build Bootloader : U-Boot
 ---------------------------
 	cd u-boot-2013.10-ti2013.12.01/
 	make clean
 	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- clean
 	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x_boneblack_config
 	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
 	ls --> you will see MLO , u-boot.img
 	
 Build Kernal and DTB and Modules
 --------------------------------------
 	cd linux-3.12.10-ti2013.12.01/
 	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- clean
 	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- omap2plus_defconfig
 	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
 	make -j8 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage dtbs
 	  if you are facing any error : 
 	   sudo gedit scripts/dtc/dtc-lexer.lex.c
 	   extern  yyloc
 	make -j8 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules
 	mkdir ../tmp
 	make -j4 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_STRIP=1 INSTALL_MOD_PATH=../tmp modules_install
 	
 Building RFS
 --------------
        sudo mkdir /nfsroot
        $cd Arun_Porting/Testing/am335x-evm-linux-sdk-bin-05.02.00.10/ti-processor-sdk-linux-am335x-evm-bin-05.02.00.10/filesystem
        arun@arun-ubuntu:~/am335x-evm-linux-sdk-bin-05.02.00.10/ti-processor-sdk-linux-am335x-evm-bin-05.02.00.10/filesystem$ ls
	arago-base-tisdk-image-am335x-evm.tar.xz
	arago-base-tisdk-image-am335x-evm.ubi
	arago-tiny-image-am335x-evm.tar.xz
	arago-tiny-image-am335x-evm.ubi
	tisdk-docker-rootfs-image-am335x-evm.tar.xz
	tisdk-docker-rootfs-image-am335x-evm.ubi
	tisdk-rootfs-image-am335x-evm.tar.xz
	tisdk-rootfs-image-am335x-evm.ubi

        sudo tar xfv tisdk-rootfs-image-am335x-evm.tar.xz -C /nfsroot/
        
 Your Modules:
 --------------
  
  Arun_Porting/Testing/am335x-evm-linux-sdk-src-05.02.00.10/board-support$ Give below command in this path
        sudo cp -r tmp/lib/modules/4.14.79-gbde58ab01e/ /nfsroot/lib/modules/
        
        
        
        
==============================================================================================
        #nfs settings on server side
#---------------------------------------------------------

sudo gedit /etc/exports 
copy below line and add it that file
/nfsroot *(rw,sync,no_subtree_check,no_root_squash)

sudo exportfs -ra
sudo /etc/init.d/nfs-kernel-server restart

---------------------------------------------------------

Create boot Partition in SD Card, boot size = 100 MB , Format = FAT32 use Disk utility tool
In SDK : copy this MLO , uboot.img , uEnv.txt
--------------------------------------------------------
/home/arun/Github/Arun_Porting/Testing/am335x-evm-linux-sdk-src-05.02.00.10/board-support/u-boot-2018.01+gitAUTOINC+313dcd69c2-g313dcd69c2

sudo cp MLO /media/arun/boot/
sudo cp u-boot.img /media/arun/boot/
sudo cp uEnv.txt /media/arun/boot/

In /srv/tftp : copy this zImage , am335x-boneblack.dtb
-----------------------------------------------------------------
chmod 777 /srv/tftp

Go to cd am335x-evm-linux-sdk-src-05.02.00.10/board-support/linux-4.14.79+gitAUTOINC+bde58ab01e-gbde58ab01e/arch/arm/boot/
sudo cp zImage /srv/tftp/

Go to cd am335x-evm-linux-sdk-src-05.02.00.10/board-support/linux-4.14.79+gitAUTOINC+bde58ab01e-gbde58ab01e/arch/arm/boot/dts/
sudo cp am335x-boneblack.dtb /srv/tftp/


==============================================================================================

        
 
 Full path : 
 /home/arun/Github/Arun_Porting/Testing/am335x-evm-linux-sdk-src-05.02.00.10/board-support/linux-4.14.79+gitAUTOINC+bde58ab01e-gbde58ab01e
a
 
===========================================
In you Laptop , eth0 is not showing , then you need to change your ethernet connection enps0 to eth0

ifconfig
ip link show enp2s0

  run@arun-ubuntu:~$ ip link show enp2s0
2: enp2s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 30:65:ec:c0:f7:c5 brd ff:ff:ff:ff:ff:ff
    
sudo nano /etc/udev/rules.d/10-network.rules

Add below line

SUBSYSTEM=="net", ACTION=="add", ATTR{address}=="30:65:ec:c0:f7:c5", NAME="eth0"
sudo udevadm control --reload
sudo reboot


===================================================
sudo ifconfig eth0 192.168.1.10

In Laptop , Settings -> Network -> IPV4 
Set Manual, 192.168.1.10 , 255.255.255.254 ,  192.168.1.1 
and Apply


==================================================================
 For SPI 
 
Copy below things Linux kernel , Patches , Configs
For Kernel : linux-4.19.103

arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work$ cd linux-4.19.103
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work/linux-4.19.103$ ls
arch   certs    CREDITS  Documentation  firmware  include  ipc     Kconfig  lib       MAINTAINERS  mm   README   scripts   sound  usr
block  COPYING  crypto   drivers        fs        init     Kbuild  kernel   LICENSES  Makefile     net  samples  security  tools  virt
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work/linux-4.19.103$ cd ..
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work$ ls
hello.c  linux-4.19.103  linux-4.19.103.tar.gz  Makefile  P01_spichar_framework  Patches  SPI_Template
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work$ cd linux-4.19.103/
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work/linux-4.19.103$ ls
arch   certs    CREDITS  Documentation  firmware  include  ipc     Kconfig  lib       MAINTAINERS  mm   README   scripts   sound  usr
block  COPYING  crypto   drivers        fs        init     Kbuild  kernel   LICENSES  Makefile     net  samples  security  tools  virt
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work/linux-4.19.103$ patch -p1 < ../Patches/sud_embisys.patch 
patching file arch/arm/mach-omap2/omap_hwmod_33xx_43xx_ipblock_data.c
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work/linux-4.19.103$ ls
arch   certs    CREDITS  Documentation  firmware  include  ipc     Kconfig  lib       MAINTAINERS  mm   README   scripts   sound  usr
block  COPYING  crypto   drivers        fs        init     Kbuild  kernel   LICENSES  Makefile     net  samples  security  tools  virt
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work/linux-4.19.103$ cp ../Configs/config.4.19.103.sud .config
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work/linux-4.19.103$ ^C
arun@arun-ubuntu:~/Github/Embisys_Labs/LinuxDeviceDriver/00_SPI_Work/linux-4.19.103$ 


make -j8 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage dtbs
==================================================================



For running :
Makefile :
=================
obj-m += spi_pdev.o
obj-m += spi_pdri.o
KVERSION	:= /home/arun/Github/Arun_Porting/Testing/am335x-evm-linux-sdk-src-05.02.00.10/board-support/linux-4.14.79+gitAUTOINC+bde58ab01e-gbde58ab01e/
CROSS_COMPILE	:= arm-linux-gnueabihf-
ARCH	:=	arm
all:
	$(MAKE)	-C $(KVERSION) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
clean:
	$(MAKE)	-C $(KVERSION) M=$(PWD) clean



========

make
sudo cp spi_pdev.ko /nfsroot/
sudo cp spi_pdri.ko /nfsroot/

cd /
sudo insmod spi_pdri.ko 
sudo insmod spi_pdev.ko

rmmod spi_pdri
rmmod spi_pdev
=====
make -j8 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- KCFLAGS="-march=armv7-a -mtune=cortex-a8" zImage


===========================
serverip=192.168.1.10
ipaddr=192.168.1.11
setenv rootpath=/nfsroot
nfsargs= setenv bootargs mpurate=${mpurate} console=${console} vram=${vram} root=/dev/nfs tcp rw init=/sbin/init nfsroot=192.168.1.10:/nfsroot,nfsvers=3,tcp rw ip=192.168.1.11:::::eth0
uenvcmd=run nfsargs;mmc rescan; tftpboot 0x81000000 am335x-boneblack.dtb; tftpboot 0x80200000 zImage; bootz 0x80200000 - 0x81000000;





Cross Compilation your C code 
===================================== 
In your HOST 

sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
arm-linux-gnueabihf-gcc --version



arm-linux-gnueabihf-gcc hello.c -o output 
Using SCP command Transfer Binary and check it ..

scp output debian@192.168.6.2:/home/debian/
chmod +x output
./output


If Your facing any Glib error : 
===============================
Solution 1: (Success)
-------------
 arm-linux-gnueabihf-gcc main.c -o output -static

Solution 2:
--------------
ldd --version
cat /etc/os-release

check toolchain
glib is NOT Maching (Host GLIB higher version than BBB)
  
mkdir -p bbb-sysroot/
rsync -avz debian@192.168.6.2:/lib /home/arun/Github/Tutorial_BeagleBoneBlack/bbb-sysroot/
rsync -avz debian@192.168.6.2:/usr /home/arun/Github/Tutorial_BeagleBoneBlack/bbb-sysroot/
arm-linux-gnueabihf-gcc hello.c -o output --sysroot=/home/arun/Github/Tutorial_BeagleBoneBlack/bbb-sysroot
Transfer and check it 


or
Solution 3:
---------------
Compile inside the BBB

or
Solution 4:
-------------
Use Docker Setup as same Debian 10 setup 

or
Solution 5:
-------------
Upgrade BBB OS 


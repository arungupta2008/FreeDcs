#!/bin/bash

if [ "$(id -u)" != "0" ]; then
	echo “This script must be run as root” 2>&1
	exit 1
fi

######################################
#				Variables
######################################
arm='arm'
i386='i386'
BASE_DIR=`pwd`
common='common'
arm_patch='adeos-ipipe-2.6.37.6-arm-1.18-03.patch'
i386_patch='adeos-ipipe-2.6.37.6-x86-2.9-02.patch'
liblog='liblog.so'
logd='logd'
######################################

mkdir $common
mkdir build
mkdir vm_controller
cd build
mkdir $arm
mkdir $i386
#chmod 777 -R *
cd ../$common/
mkdir $arm
mkdir $i386

cd $BASE_DIR

#have to see this again
chmod 777 -R *

cp $BASE_DIR/Packages/buildroot-2011.11.tar.bz $BASE_DIR/build/$arm
cd $BASE_DIR/build/$arm
tar xvf buildroot-2011.11.tar.bz
rm buildroot-2011.11.tar.bz

cp $BASE_DIR/Packages/buildroot-2011.11.tar.bz $BASE_DIR/build/$i386
cd $BASE_DIR/build/$i386
tar xvf buildroot-2011.11.tar.bz
rm buildroot-2011.11.tar.bz


####################################################
# Old one xenomai Use 
####################################################
#cp $BASE_DIR/Packages/xenomai-2.6.0.tar.bz2 $BASE_DIR/$common
#cd $BASE_DIR/$common
#tar xvf xenomai-2.6.0.tar.bz2
#rm xenomai-2.6.0.tar.bz2
####################################################

cp $BASE_DIR/Packages/$arm_patch $BASE_DIR/$common/$arm/$arm_patch
cp $BASE_DIR/Packages/$i386_patch $BASE_DIR/$common/$i386/$i386_patch

cp $BASE_DIR/scripts/vmwarecontroller.vmx $BASE_DIR/vm_controller/vmwarecontroller.vmx
cp $BASE_DIR/scripts/vmwarecontroller.vmx $BASE_DIR/vm_controller/vmwarecontroller_arm.vmx
cp $BASE_DIR/scripts/vmwarecontroller.vmx $BASE_DIR/vm_controller/vmwarecontroller_i386.vmx
cp $BASE_DIR/scripts/ARM_buildroot_config $BASE_DIR/build/$arm/buildroot-2011.11/.config  
cp $BASE_DIR/scripts/mini2440_defconfig $BASE_DIR/build/$arm/buildroot-2011.11/mini2440_defconfig
cp $BASE_DIR/scripts/mini2440_defconfig $BASE_DIR/common/mini2440_defconfig
cp $BASE_DIR/scripts/i386_buildroot_config $BASE_DIR/build/$i386/buildroot-2011.11/.config
cp $BASE_DIR/scripts/kernel_config $BASE_DIR/$common/.kernel_config
cp $BASE_DIR/scripts/install $BASE_DIR/
cp $BASE_DIR/scripts/test_package.sh $BASE_DIR/
cp $BASE_DIR/scripts/image2vmdk $BASE_DIR/
cp $BASE_DIR/scripts/configure.conf $BASE_DIR/
cp $BASE_DIR/scripts/$liblog $BASE_DIR/
cp $BASE_DIR/scripts/$logd $BASE_DIR/
cd $BASE_DIR

##Don't turn it on !!!
#rm -R Packages
#
chmod 777 -R *
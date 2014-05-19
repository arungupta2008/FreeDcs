#!/usr/bin/env bash
#!/bin/bash

#dependenci
# vmwarecontroller.vmx shoulv be in  vm_controller folder.

	if [ "$(id -u)" != "0" ]; then
		echo “This script must be run as root” 2>&1
		exit 1
	fi
#Testing the Give package present or not
###################################
chmod 777 test_package.sh
./test_package.sh
###################################

#including Configuration file
if [ ! -f  configure.conf ]
then
	echo "Can not Find the configuration file";
	exit;
fi

source configure.conf

###################################
#parameters

vmdisk='n'
arc_i386='n'
arc_arm='n'
iface_type='2'  # 1 --> Nated 2 --> Birdged
version='1.0';
ip='192.168.1.1'
netmask='255.255.0.0'
dhcp='n'
ip_type='2'    # 1--> Dhcp 2--> Static
##Commands Line Input

#		(1)-vm For Wanna Vmware or not ?
#		(2.a)-B for Bridged
#		(2.b)-N for Nated mode(host only)
#		(3.a,1.a)-ip if user wants to insert ip
#		(5)-x86
#		(6)-arm
#		(3.b)-dhcp
#		(3.a,1.b)-netmask

# So minimum input size is (1,2,3,(5 or 6)) total input size is 4 here.
input=$*
echo "Input : $*"
## Case can non have a If condition

########################################################################################
#								Functions Started
########################################################################################

function print_usage(){
	#wall <<EOF
cat <<EOF

	usage : $0  [-vm] [either -B or -N] [-arm] [-i386] [either -dhcp or -ip][ipaddress] [-netmask][netmask]
	-vm      --> want to generate the VM ware disk or not? by default it will not generate vmware disk
	-B       --> Interface will be in Bridged mode, at a time either use -N or -B
	-N       --> Interface will be in NAT mode(host_only), at a time either use -N or -B
	-arm     --> Architecture of os  ARM architecture.
	-i386    --> Architecture of os  i386 or x86 architecture.
	-dhcp    --> interface would be in DHCP mode either use -ip for Static or -dhcp for dynamic IP.
	-ip      --> Static IP of interface. please add -netmask in the argument.
	-netmask --> for adding netmask for the entered IP.
	
		For Ex.  $0 -vm -B -arm -i386 -ip 10.202.2.89 -netmask 255.255.255.0"

EOF
		#echo -e " usage : $0  [-vm] [either -B or -N] [-arm] [-i386] [either -dhcp or -ip][ipaddress] [-netmask][netmask]\n\t-vm --> want to generate the VM ware disk or not? by default it will not generate vmware disk\n\t-B --> Interface will be in Bridged mode, at a time either use -N or -B\n\t-N --> Interface will be in NAT mode(host_only), at a time either use -N or -B\n\t-arm --> Architecture of os  ARM architecture \n\t-i386 --> Architecture of os  i386 or x86 architecture \n\t-dhcp --> interface would be in DHCP mode either use -ip for Static or -dhcp for dynamic IP \n\t-ip --> Static IP of interface. please add -netmask in the argument.\n\t-netmask --> for adding netmask for the entered IP\n\n\tFor Ex.\n\t$0 -vm -B -arm -i386 -ip 10.202.2.89 -netmask 255.255.255.0"
		exit;
	}
function command_line_input(){
	ip_type_conflict=0;
	iface_type_conflict=0;
	error=0;
	ip_with_netmask=0;
	#for i in $@; do
	while [ $# -gt 0 ]; do
		case "$1" in
			 -vm ) vmdisk='y';;
			 -B  ) iface_type='2';iface_type_conflict+=1;;
			 -N  ) iface_type='1';iface_type_conflict+=1;;
			 -ip ) shift; ip_type='2';ip_with_netmask+=1;ip=$1;ip_type_conflict+=1;validate_ip_CLI $1;;
			 -netmask ) shift;netmask=$1; ip_with_netmask+=1;validate_netmask_CLI $1;;
			 -arm ) arc_arm='y';;
			 -i386 ) arc_i386='y';;
			 -dhcp ) dhcp='y';ip_type='1';ip_type_conflict+=1;;
		esac
		shift;
	done;
	if [[ iface_type_conflict -gt 1 || ip_type_conflict -gt  1 || error -gt 0 || ip_with_netmask -lt 2 ]]; then
		echo -e "Some Error !!! in CLI\n"
		print_usage $input
		exit;
	fi
	#echo "Vmware $vmdisk"
	}

function build_i386(){
	arc='i386'
	#make sure those packages are installed. Although anyway the program that needs the package will trhough an error
	#saying wich package is missing and then must be installed and tried again.
	#subversion
	#GCC compiler
	#ncurses-dev
	#gettext
	#texinfo
	#patch
	#bison
	#flex
	#openssl
	#libssl-dev
	#sed

	#compile everything with buildroot
	cd build/$arc/$Buildroot/
	make
	cd output/images/
	if [ ! -f rootfs.tar ]
	then
		echo "There is some error in creating the Root.fs file please check the configuration files";
	exit;
	fi
	#build and install xenomai libraries
	cd $BASE_DIR/build/$arc/$Buildroot/output/build/$Xenomai/
	pwd
	#build and install xenomai libraries
	#cd output/build/$Xenomai/
	make DESTDIR=$BASE_DIR/build/$arc/$Buildroot/output/target/usr/xenomai install
	cd $BASE_DIR/

	if [ $vmdisk = 'y' ]
	then
		#create vmware image
		chmod 777 image2vmdk
		./image2vmdk $dhcp $ip $netmask $arc
		chmod 777 ./vm_controller/ -R
	fi

	#Download the controller code
	#This is used in an if sentence. Because it has happened to overwrite the code with a new checkout when trying to build the
	#vmware image.
	if [ ! -d ./freedcs-code ]
	then
		svn checkout https://svn.code.sf.net/p/freedcs/code/branches/ControllerWithUnity freedcs-code
		chmod 777 ./freedcs-code/ -R
	fi

	#compile controller code, remember to go to the src dir and then use from there

	#give an idea of what to do next.
	#set ipaddress, change root password, add to ssh known host your development machine, install certificates,
}
function build_arm(){
	#compile everything with buildroot
	arc='arm'
	cd build/$arc/$Buildroot/
	make

	cd output/images/
	if [ ! -f rootfs.tar ]
	then
		echo "There is some error in creating the Root.fs file please check the configuration files";
	exit;
	fi
	#build and install xenomai libraries
	cd $BASE_DIR/build/$arc/$Buildroot/output/build/$Xenomai/
	pwd
	#build and install xenomai libraries
	cd output/build/$Xenomai/
	make DESTDIR=$BASE_DIR/build/$arc/$Buildroot/output/target/usr/xenomai install
	cd $BASE_DIR/

	if [ $vmdisk = 'y' ]
	then
		#create vmware image
		chmod 777 image2vmdk
		./image2vmdk $dhcp $ip $netmask $arc
		chmod 777 ./vm_controller/ -R
	fi	

	#Download the controller code
	#This is used in an if sentence. Because it has happened to overwrite the code with a new checkout when trying to build the
	#vmware image.
	if [ ! -d ./freedcs-code ]
	then
		svn checkout https://svn.code.sf.net/p/freedcs/code/branches/ControllerWithUnity freedcs-code
		chmod 777 ./freedcs-code/ -R
	fi

	#compile controller code, remember to go to the src dir and then use from there

	#give an idea of what to do next.
	#set ipaddress, change root password, add to ssh known host your development machine, install certificates,
	}
function creating_NIC_TPYE(){
		if [[ $1 -eq '1' ]]; then   # Nated mode :)
			sed -i 's/ethernet0.connectionType.*/ethernet0.connectionType = "hostonly"/' vm_controller/vmwarecontroller.vmx;
		else
			sed -i 's/ethernet0.connectionType.*/ethernet0.connectionType = "bridged"/' vm_controller/vmwarecontroller.vmx
		fi
	}


function valid_ip(){
    local  ip=$1
    local  stat=1

    if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
        OIFS=$IFS
        IFS='.'
        ip=($ip)
        IFS=$OIFS
        [[ ${ip[0]} -le 255 && ${ip[1]} -le 255 \
            && ${ip[2]} -le 255 && ${ip[3]} -le 255 ]]
        stat=$?
    fi
    return $stat
}

function validate_netmask(){
    local netmask=$1
    local netmask_binary
    local octet
    local stat

    if [[ $netmask =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
        stat=0
        for ((i=0; i<4; i++))
        do
            octet=${netmask%%.*}
            netmask=${netmask#*.}
            [[ $octet -gt 255 ]] && { stat=1; break; }
            netmask_binary=$netmask_binary$( echo "obase=2; $octet" | bc )
            [[ $netmask_binary =~ 01 ]] && { stat=1; break; }
        done
    else
        stat=1
    fi
    return $stat
}


function validate_ip_CLI(){
		#echo "Here $1"
		if valid_ip $1; then
			echo "IP is correct"
			else
			let error=$error+1 ;
		fi
		#if [[ ! `valid_ip $1` ]]; then  let error=$error+1 ; fi
	}
function validate_netmask_CLI(){
		#echo "Here $1"
		if validate_netmask $1; then
			echo "Netmask is correct"
			else
			let error=$error+1 ;
		fi
		#if [[ ! `validate_netmask $1` ]]; then let error=$error+1 ;fi
	}
function default_setup(){
	echo "You are supposed to do default setup of FreeCDS"
	echo "Here would be some information what we are going to make in default setup"
	build;
	}
function custom_setup(){
	echo "Custom Setup"

	while true; do
		read -p "Do you want to make vmware disk file (y/n) :" vmdisk
		case $vmdisk in
			[YyNn]* ) break;;
			* ) echo "Please answer yes or no.";;
		esac
	done
	while true; do
		read  -p "For what type of architecture you want to make build? `echo $'\n1. '`ARM`echo $'\n2. '`i386`echo $'\n3. '`Both`echo $'\n '`Please enter you choice. (1/2/3) :" choice
		if [[ $choice -eq '1' ]]; then
			arc_arm='y';
			arc_i386='n';
			break;
		elif [[ $choice -eq '2' ]]; then
			arc_i386='y';
			arc_arm='n';
			break;
		elif [[ $choice -eq '3' ]]; then
			arc_i386='y';
			arc_arm='y';
			break;
		else
			echo "Please Enter correct input";
		fi
	done
	while true; do
		read  -p "Do you want to have DHCP or Static IP? `echo $'\n1. '`DHCP`echo $'\n2. '`Static IP`echo $'\n '`Please enter you choice. (1/2) :" ip_type
		if [[ $ip_type -eq '1' ]]; then
			break;
		elif [[ $ip_type -eq '2' ]]; then
			while true; do
				read  -p "Please Enter your ipaddress :" ip
				if valid_ip $ip; then
					break;
				else
					echo "Please Enter correct choice";
				fi
			done
			while true; do
				read  -p "Please Enter your netmask :" netmask
				if validate_netmask $netmask; then
					break;
				else
					echo "Please Enter correct choice";
				fi
			done
			break;
		else
			echo "Please Enter correct choice";
		fi
	done
	while true; do
		read  -p "What type of interface you want? `echo $'\n1. '`Nated`echo $'\n2. '`Bridged`echo $'\n '`Please enter you choice. (1/2) :" iface_type
		if [[ $iface_type -eq '1' || $iface_type -eq '2' ]]; then
			break;
		else
			echo "Please Enter correct choice";
		fi
	done
	build;
	}

function build(){
	echo "***************************************"
	echo "Vmware $vmdisk"
	echo "i386 Architecture $arc_i386"
	echo "ARM Architecture $arc_arm"
	if [[ $iface_type -eq '1' ]]; then
		echo "Interface type hostonly"
	else
		echo "Interface type bridged"
	fi
	if [[ $iface_type -eq '1' ]]; then
		echo "Interface type dhcp"
	else
		echo "Interface type static"
	fi
	echo "Ipaddress $ip"
	echo "Netmask $netmask"
	echo "***************************************"
	#echo " $"
	#echo " $"
	#yet to see this
	if [ $arc_i386  -eq "n"  ] &&  [ $arc_arm -eq "n"  ]; then
		echo -e "No architecture Selected \n Please select an Architecture least";
		exit;
	fi
	echo -e "Creating Build"
	BASE_DIR=`pwd`

	#verify that the we have root privileges, if not stop with a message
	if [ "$(id -u)" != "0" ]; then
		echo “This script must be run as root” 2>&1
		exit 1
	fi
	if [[ $arc_i386 = 'y' ]]
	then
		figlet -ctk  ".................."
		figlet -ctk  "Building for i386"
		figlet -ctk  ".................."
		echo "Going to create arc_i386 $arc_i386"
		build_i386
	fi
	if [[ $arc_arm = 'y' ]]
	then
		figlet -ctk  ".................."
		figlet -ctk  "Building for ARM"
		figlet -ctk  ".................."
		echo "Going to create arc_ARM"
		build_arm;
	fi
	}
########################################################################################
#								Functions Ended
########################################################################################
if [[ $# -eq '0' ]]; then
#Welcome
figlet -ctk  ".................."
figlet -ctk  "Welcome to FreeDCS"
figlet -ctk  ".................."
# Version
echo "Version : $version"

#Question and Answeres ?
	while true; do
		read -p "Do you want to run Default setup? (y/n) :" yn
		case $yn in
			[Yy]* ) default_setup; break;;
			[Nn]* )  custom_setup; break;;
			* ) echo "Please answer yes or no.";;
		esac

	done
	else
		if [ "$1" -eq "--help" ] || [ "$1" -eq  "-h" ] || [ "$1" -eq "-help" ]; then
			echo " have to print --help"
			print_usage $input
		else
			command_line_input $input
			build;
		fi
fi

###################################

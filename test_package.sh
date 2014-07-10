#!/usr/bin/env bash
#!/bin/bash
# What dependencies are missing?
DEPENDENCIES="sed  gettext  libncurses5-dev  texinfo patch bison flex openssl libssl-dev gcc sysvbanner figlet toilet grub tar bc kpartx grub qemu-img make svn bzip2 g++ unzip subversion rsync lsof fakeroot parted kpartx"
#DEPENDENCIES=(sed svn)
PKGSTOINSTALL=""
PKGFINALNOTINS=""
case_=0

function warning(){
cat <<EOF
**************************************************************
Note : Please this package tester comes with no guarantee may 
be some packages goes undetected but actually these packages 
are present in your machine.
Bacause this was implemented for every linux box
**************************************************************
EOF
sleep 2
}
test_packages(){
##Calling warning function.	
warning;
#for ((i=0; i /dev/null; do
for i in $@; do
	if command -v dpkg > /dev/null; then
		if [[ ! `dpkg -l | grep -w "ii  ${i} "` ]]; then
			case_=1;
			#echo "1Checking for ...$i"
			PKGSTOINSTALL=$PKGSTOINSTALL" "${i};
		fi	
	# OpenSuse, Mandriva, Fedora, CentOs, ecc. (with rpm)
	elif command -v rpm > /dev/null; then
		if [[ ! `rpm -qa | grep "${i}"` ]]; then
			#echo "2Checking for ...$i"
			case_=1;
			PKGSTOINSTALL=$PKGSTOINSTALL" "${i};
		fi;
	# ArchLinux (with pacman)
	elif command -v pacman > /dev/null; then
		if [[ ! `pacman -Qqe | grep "${i}"` ]]; then
			#echo "3Checking for ...$i"
			case_=1;
			PKGSTOINSTALL=$PKGSTOINSTALL" "${i};
		fi;
	# If it's impossible to determine if there are missing dependencies, mark all as missing
	else
		case_=1;
		PKGSTOINSTALL=$PKGSTOINSTALL" "${i};
	fi;
	if [ "$case_" -eq 1 ]; then
		echo "Checking for $i.... NO"
		case_=0;
		else 
		echo "Checking for $i.... YES"
	fi;
done;
}
test_packages ${DEPENDENCIES}
update_package(){
	for i in $@; do
		# Debian, Ubuntu and derivatives (with apt-get)
		if command -v apt-get &> /dev/null; then
			apt-get install ${i}
			#if [ "$eval apt-get install ${i}" -ne 0 ]; then
			#	PKGFINALNOTINS=$PKGFINALNOTINS" "${i};
			#fi;
		# OpenSuse (with zypper)
		elif command -v zypper &> /dev/null; then
			zypper install ${i}
		# Mandriva (with urpmi)
		elif command -v urpmi &> /dev/null; then
			urpmi ${i}
		# Fedora and CentOS (with yum)
		elif command -v yum &> /dev/null; then
			yum install ${i}
		# ArchLinux (with pacman)
		elif command -v pacman &> /dev/null; then
			pacman -Sy ${i}
		# Else, if no package manager has been founded
		else
			# Set $NOPKGMANAGER
			NOPKGMANAGER=TRUE
			echo "ERROR: impossible to found a package manager in your sistem. Please, install manually ${i}."
		fi
	done;
		# Check if installation is successful
		if [[ $? -eq 0 && ! -z $NOPKGMANAGER ]] ; then
			echo "All dependencies are satisfied."
		# Else, if installation isn't successful
		else
			echo "ERROR: impossible to install some missing dependencies. Please, install manually $PKGFINALNOTINS."
		fi
}


# If some dependencies arsse missing, asks if user wants to install
if [ "$PKGSTOINSTALL" != "" ]; then
	echo -n "Some dependencies are missing. Want to install them? (Y/n): "
	read SURE
	# If user want to install missing dependencies
	if [[ $SURE = "Y" || $SURE = "y" || $SURE = "" ]]; then
	update_package${PKGSTOINSTALL}
	# Else, if user don't want to install missing dependencies
	else
		echo "WARNING: Some dependencies may be missing. So, please, install manually $PKGSTOINSTALL."
	fi
fi;	

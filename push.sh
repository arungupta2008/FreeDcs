BASE_DIR=`pwd`


cp $BASE_DIR/freedcs-code1/Controller/src/Logger/liblog.so $BASE_DIR/
cp $BASE_DIR/freedcs-code1/Controller/src/Logger/logd $BASE_DIR/

## For Base FreeDcs
echo -e "Setting up for minimal Freedcs(/freedcs/freedcs/)\n"
cp $BASE_DIR/install $BASE_DIR/freedcs/scripts/
cp $BASE_DIR/image2vmdk $BASE_DIR/freedcs/scripts/
cp $BASE_DIR/test_package.sh $BASE_DIR/freedcs/scripts/
cp $BASE_DIR/README $BASE_DIR/freedcs/scripts/
cp $BASE_DIR/Version_Info_build $BASE_DIR/freedcs/scripts/
cp $BASE_DIR/Developers $BASE_DIR/freedcs/scripts/
cp $BASE_DIR/liblog.so $BASE_DIR/freedcs/scripts/
cp $BASE_DIR/logd $BASE_DIR/freedcs/scripts/
cp $BASE_DIR/configure.conf $BASE_DIR/freedcs/scripts/


## For Base FreeDcsARM
echo -e "Setting up for ARM Freedcs(/freedcs/freedcs_arm/)\n"

cp $BASE_DIR/test_package.sh $BASE_DIR/freedcs_arm/
cp $BASE_DIR/README $BASE_DIR/freedcs_arm/
cp $BASE_DIR/Version_Info_build $BASE_DIR/freedcs_arm/
cp $BASE_DIR/Developers $BASE_DIR/freedcs_arm/
cp $BASE_DIR/liblog.so $BASE_DIR/freedcs_arm/
cp $BASE_DIR/logd $BASE_DIR/freedcs_arm/
cp $BASE_DIR/configure.conf $BASE_DIR/freedcs_arm/

## For Base FreeDcsi386
echo -e "Setting up for i386 Freedcs(/freedcs/freedcs_i386/)\n"
cp $BASE_DIR/test_package.sh $BASE_DIR/freedcs_i386/
cp $BASE_DIR/README $BASE_DIR/freedcs_i386/
cp $BASE_DIR/Version_Info_build $BASE_DIR/freedcs_i386/
cp $BASE_DIR/Developers $BASE_DIR/freedcs_i386/
cp $BASE_DIR/liblog.so $BASE_DIR/freedcs_i386/
cp $BASE_DIR/logd $BASE_DIR/freedcs_i386/
cp $BASE_DIR/configure.conf $BASE_DIR/freedcs_i386/


##For Git

echo -e "Putting everything in git"

cp $BASE_DIR/test_package.sh $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/push.sh $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/README $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/Version_Info_build $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/Developers $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/liblog.so $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/logd $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/configure.conf $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/install $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/image2vmdk $BASE_DIR/git/FreeDcs/
cp $BASE_DIR/issue/* $BASE_DIR/git/FreeDcs/issue/

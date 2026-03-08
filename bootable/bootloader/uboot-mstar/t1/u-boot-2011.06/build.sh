HOST_PROCESSOR_NUMBER=`cat /proc/cpuinfo | grep processor | wc -l`
HOST_PROCESSOR_NUMBER=$(($HOST_PROCESSOR_NUMBER+0))
if [ $HOST_PROCESSOR_NUMBER -gt 2 ] ; then
	HOST_PROCESSOR_NUMBER=2
fi
echo "HOST_PROCESSOR_NUMBER: " $HOST_PROCESSOR_NUMBER
START_TIME=$(date +%s)

#Build utopia libs
export MBOOT_ROOT="`pwd`"
export PROJECT_ROOT="$MBOOT_ROOT/../../../../../"
echo "## MAKE MBOOT UTOPIA"
cd $MBOOT_ROOT/utopia/build/
cp maxim/.config_maxim_nos_arm32_softfloat_static_mboot .config || exit 1
rm -rf bsp
rm -rf obj
make defconfig
make clean
make
make bsp
cp -a ./bsp/lib/ $MBOOT_ROOT/MstarCore/bsp/maxim/
cp -a ./bsp/include/ $MBOOT_ROOT/MstarCore/bsp/maxim/
cd $MBOOT_ROOT
echo "## MBOOT UTOPIA preparation done."

END_TIME=$(date +%s)
DIFF_TIME=$(( $END_TIME - $START_TIME ))
echo -e ""
echo -e "#############################################"
echo -e "    Done!!!! spent $DIFF_TIME seconds"
echo -e "    `date`${NONE}"
echo -e "#############################################"

#Build uboot
make -s -j$HOST_PROCESSOR_NUMBER

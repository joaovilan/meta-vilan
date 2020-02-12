#! /bin/sh
# Prepare files needed for flashing a Colibri iMX7 module and
# copy them to a convenient location for using from a running U-Boot

set -e

Flash()
{
	echo "To flash the Colibri iMX7 module a running U-Boot is required. Boot the"
	echo "module to the U-Boot prompt and"
	echo ""
	echo "insert the SD card, USB flash drive or when using TFTP connect Ethernet only"
	echo "and enter:"
	echo "'run setupdate'"
	echo ""
	echo "then to update all components enter:"
	echo "'run update'"
	echo ""
	echo "Alternatively, to update U-Boot enter:"
	echo "'run update_uboot'"
	echo "to update a component stored in UBI enter:"
	echo "'run prepare_ubi'"
	echo "followed by one of:"
	echo "'run update_kernel'"
	echo "'run update_fdt'"
	echo "'run update_rootfs'"
	echo ""
	echo ""
	echo "If you don't have a working U-Boot anymore, connect your PC to the module's"
	echo "USB client port, bring the module into the serial download mode and start the update.sh"
	echo "script with the -d option. This will copy U-Boot into the module's RAM and"
	echo "execute it."
	echo ""
	echo "Then use the following command to get U-Boot running:"
	echo "'./update.sh -d'"
	echo ""
	echo "Next, recreate the Boot Configuration Block and the Toradex Config Block:"
	echo "'run setupdate'"
	echo "'run update_uboot'"
	echo "'run create_bcb'"
	echo "'cfgblock create'"
}

Usage()
{
	echo ""
	echo "Prepares and copies files for flashing internal NAND of Colibri iMX7"
	echo ""
	echo "The recommended way is to copy the files on a SD card or USB flash drive."
	echo "The script format_sd.sh may be used to format the SD card."
	echo ""
	echo "The flash step requires a running U-Boot on the target. Either one already"
	echo "flashed on the NAND or downloaded using serial downloader (argument -d)."
	echo ""
	echo "-d           : use a USB connection to copy/execute U-Boot to/from module's RAM"
	echo "-f           : flash instructions"
	echo "-h           : prints this message"
	echo "-m           : module type: 0: autodetect from ./rootfs/etc/issues (default)"
	echo "                            1: Colibri iMX7"
	echo "-o directory : output directory"
	echo ""
	echo "Example \"./update.sh -o /media/KERNEL/\" copies the required files to SD card"
	echo ""
	echo "*** For detailed recovery/update procedures, refer to the following website: ***"
	echo "http://developer.toradex.com/knowledge-base/flashing-embedded-linux-to-imx7-modules"
	echo ""
}

# initialise options
KERNEL_DEVICETREE="imx7s-colibri-eval-v3.dtb imx7d-colibri-eval-v3.dtb imx7s-colibri-aster.dtb imx7d-colibri-aster.dtb"
KERNEL_IMAGETYPE="zImage"
MODTYPE_DETECT=0

# NAND parameters
BLOCK=124KiB
MAXLEB=8112
PAGE=2KiB

OUT_DIR=""
UBOOT_RECOVERY=0

while getopts "dfhm:o:" Option ; do
	case $Option in
		d)	UBOOT_RECOVERY=1
			;;
		f)	Flash
			exit 0
			;;
		h)	Usage
			exit 0
			;;
		m)	MODTYPE_DETECT=$OPTARG
			;;
		o)	OUT_DIR=$OPTARG
			;;
	esac
done

if [ "$OUT_DIR" = "" ] && [ "$UBOOT_RECOVERY" = "0" ] ; then
	Usage
	exit 1
fi

# is OUT_DIR an existing directory?
if [ ! -d "$OUT_DIR" ] && [ "$UBOOT_RECOVERY" = "0" ] ; then
	echo "$OUT_DIR" "does not exist, exiting"
	exit 1
fi

case $MODTYPE_DETECT in
	0)	# auto detect MODTYPE from rootfs directory
		if [ -f rootfs/etc/issue ] ; then
			CNT=`grep -c "MX7" rootfs/etc/issue || true`
			if [ "$CNT" -ge 1 ] ; then
				echo "Colibri iMX7 rootfs detected"
				MODTYPE=colibri-imx7
			fi
		fi
		if [ -e $MODTYPE ] ; then
			echo "can not detect module type from ./rootfs/etc/issue"
			echo "please specify the module type with the -m parameter"
			echo "see help: '$ ./update.sh -h'"
			echo "exiting"
			exit 1
		fi
		;;
	1)	MODTYPE=colibri-imx7
		echo "Colibri iMX7 rootfs specified"
		;;
	*)	echo "-m paramter specifies an unknown value"
		exit 1
		;;
esac

case "$MODTYPE" in
	"colibri-imx7")
		IMAGEFILE=ubifs.img
		LOCPATH="imx_flash"
		OUT_DIR="$OUT_DIR/colibri_imx7"
		;;
	*)	echo "script internal error, unknown module type set"
		exit 1
		;;
esac

BINARIES=${MODTYPE}_bin

#is only U-Boot to be copied to RAM?
if [ "$UBOOT_RECOVERY" -eq 1 ] ; then
	echo "Put the module in recovery mode and press [ENTER]..."
	read RESULT
	sudo ${LOCPATH}/imx_usb ${BINARIES}/u-boot-nand.imx
	exit
fi

#sanity check for correct untared rootfs
DEV_OWNER=`ls -ld rootfs/dev | awk '{print $3}'`
if [ "${DEV_OWNER}x" != "rootx" ]
then
	printf "rootfs/dev is not owned by root, but it should!\n"
	printf "\033[1mPlease unpack the tarball with root rights.\033[0m\n"
	printf "e.g. sudo tar xjvf Colibri_iMX7_LinuxImageV2.6_20160331.tar.bz2\n"
	exit 1
fi

#sanity check, can we execute mkfs.ubifs, e.g. see the help text?
CNT=`sudo $LOCPATH/mkfs.ubifs -h | grep -c space-fixup || true`
if [ "$CNT" -eq 0 ] ; then
	echo "The program mkfs.ubifs can not be executed or does not provide --space-fixup"
	echo "option."
	echo "Are you on a 64bit Linux host without installed 32bit execution environment?"
	printf "\033[1mPlease install e.g. ia32-libs on 64-bit Ubuntu\033[0m"
	printf "\033[1mMaybe others are needed e.g. liblzo2:i386 on 64-bit Ubuntu\033[0m"
	exit 1
fi

#Install trap to write a sensible message in case any of the commands below
#exit premature...
trap '{ printf "\033[31mScript aborted unexpectedly...\033[0m\n"; }' EXIT

# Prepare full flashing
sudo $LOCPATH/mkfs.ubifs --space-fixup -c ${MAXLEB} -e ${BLOCK} -m ${PAGE} -o ${BINARIES}/${IMAGEFILE} -r rootfs/ -v

echo ""
echo "UBI image of root file system generated, copying data to target folder..."

#make a file with the used versions for U-Boot, kernel and rootfs
sudo touch ${BINARIES}/versions.txt
sudo chmod ugo+w ${BINARIES}/versions.txt
echo "Component Versions" > ${BINARIES}/versions.txt
basename "`readlink -e ${BINARIES}/u-boot-nand.imx`" >> ${BINARIES}/versions.txt
basename "`readlink -e ${BINARIES}/${KERNEL_IMAGETYPE}`" >> ${BINARIES}/versions.txt
ROOTFSVERSION=`grep MX7 rootfs/etc/issue || echo "Version Unknown"`
echo "Rootfs ${ROOTFSVERSION}" >> ${BINARIES}/versions.txt

#create subdirectory for this module type
sudo mkdir -p "$OUT_DIR"

# Copy device tree file
COPIED=false
if test -n "${KERNEL_DEVICETREE}"; then
	for DTB_FILE in ${KERNEL_DEVICETREE}; do
		if [ -e "${BINARIES}/${KERNEL_IMAGETYPE}-${DTB_FILE}" ]; then
			sudo cp ${BINARIES}/${DEPLOY_DIR_IMAGE}/${KERNEL_IMAGETYPE}-${DTB_FILE} "$OUT_DIR/${DTB_FILE}"
			COPIED=true
		fi
	done
	[ $COPIED = true ] || { echo "Did not find the devicetrees from KERNEL_DEVICETREE, ${KERNEL_DEVICETREE}.  Aborting."; exit 1; }
fi

#copy to $OUT_DIR
sudo cp ${BINARIES}/u-boot-nand.imx ${BINARIES}/zImage ${BINARIES}/ubifs.img ${BINARIES}/flash*.img ${BINARIES}/versions.txt "$OUT_DIR"
sudo cp ${BINARIES}/fwd_blk.img "$OUT_DIR/../flash_blk.img"
sudo cp ${BINARIES}/fwd_eth.img "$OUT_DIR/../flash_eth.img"
#cleanup intermediate files
sudo rm ${BINARIES}/ubifs.img ${BINARIES}/versions.txt
sync

#Remove trap and report success!
trap - EXIT
printf "\033[32mSuccessfully copied data to target folder.\033[0m\n\n"

Flash

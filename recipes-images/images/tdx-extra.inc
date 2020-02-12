#additional packages to include in our Toradex images

# Copy Licenses to image /usr/share/common-licenses, on vybrid for flash size reasons only the manifest
COPY_LIC_MANIFEST ?= "1"
#Bug with several MACHINES build, license files for all arch packages stored wrongly
COPY_LIC_DIRS ?= "0"
COPY_LIC_DIRS_colibri-vf = "0"

add_rootfs_version () {
    echo -n "${IMAGE_NAME} ${PV}" >> ${IMAGE_ROOTFS}/etc/issue
    echo " ${TDX_VERDATE}" | sed 's/-//' >>${IMAGE_ROOTFS}/etc/issue
    echo "" >> ${IMAGE_ROOTFS}/etc/issue
}

# add the rootfs version to the welcome banner
ROOTFS_POSTPROCESS_COMMAND += " add_rootfs_version;"

IMAGE_INSTALL_append_tegra = " \
    u-boot-toradex-fw-utils \
    tegrastats \
    tdx-nv-binaries \
    tdx-nv-binaries-nv-gstapps \
    tdx-nv-binaries-restricted-codecs \
    usb-rndis-systemd \
    hdmi-hotplug \
    \
    file \
    aspell \
    kernel-modules \
"

IMAGE_INSTALL_append_tegra3 = " \
    fs-init \
    e2fsprogs \
    e2fsprogs-mke2fs \
    e2fsprogs-resize2fs \
    e2fsprogs-tune2fs \
    nv-conf-systemd \
    perf \
"

IMAGE_INSTALL_append_tegra124 = " \
    aspell \
    e2fsprogs \
    e2fsprogs-mke2fs \
    e2fsprogs-tune2fs \
    file \
    fs-init \
    hdmi-hotplug \
    kernel-modules \
    linuxptp \
    lvds-tegra124 \
    pciutils \
    ptpd \
    tegrastats \
    u-boot-toradex-fw-utils \
    usb-rndis-systemd \
    perf \
"
IMAGE_INSTALL_append_tegra124m = " \
    aspell \
    e2fsprogs \
    e2fsprogs-mke2fs \
    e2fsprogs-tune2fs \
    file \
    fs-init \
    hdmi-hotplug \
    kernel-modules \
    linuxptp \
    lvds-tegra124 \
    pciutils \
    ptpd \
    u-boot-toradex-fw-utils \
    perf \
"

IMAGE_INSTALL_append_apalis-t30 = " \
    linuxptp \
    pciutils \
    ptpd \
    lvds-ctrl \
"

IMAGE_INSTALL_append_colibri-t20 = " \
    mtd-utils-ubifs \
"

#note that the kernel-modules meta package gets installed from
#meta-fsl-arm/conf/machine/include/imx-base.inc
IMAGE_INSTALL-MX6QDL = " \
    bmode-usb \
    libusbgx \
    libusbgx-examples \
    linuxptp \
    gpio-export \
    file \
    aspell \
    ptpd \
    fs-init \
    e2fsprogs \
    e2fsprogs-mke2fs \
    e2fsprogs-resize2fs \
    e2fsprogs-tune2fs \
    mimetypes \
"
IMAGE_INSTALL_append_mx6q = "${IMAGE_INSTALL-MX6QDL}"
IMAGE_INSTALL_append_mx6dl = "${IMAGE_INSTALL-MX6QDL}"

IMAGE_INSTALL_append_apalis-imx6 = " \
    u-boot-toradex-fsl-fw-utils \
    perf \
"
IMAGE_INSTALL_append_colibri-imx6 = " \
    u-boot-toradex-fsl-fw-utils \
    perf \
"

IMAGE_INSTALL_append_mx6ull = " \
    libusbgx \
    libusbgx-examples \
    linuxptp \
    ptpd \
    mtd-utils-ubifs \
"

#note that the kernel-modules meta package gets installed from
#meta-fsl-arm/conf/machine/include/imx-base.inc
IMAGE_INSTALL_append_apalis-imx6 = " \
    pciutils \
"

IMAGE_INSTALL_append_vf = " \
    linuxptp \
    libusbgx \
    libusbgx-examples \
    ptpd \
    mtd-utils-ubifs \
    kernel-modules \
    usb-suspend-resume \
"

IMAGE_INSTALL_append_colibri-vf = " \
    u-boot-toradex-fsl-fw-utils \
"

IMAGE_INSTALL_append_mx7 = " \
    linuxptp \
    libusbgx \
    libusbgx-examples \
    file \
    aspell \
    ptpd \
"

IMAGE_INSTALL_append_colibri-imx7-emmc = " \
    u-boot-toradex-fsl-fw-utils \
    perf \
"

IMAGE_INSTALL_append_colibri-imx7 = " \
    u-boot-toradex-fsl-fw-utils \
    mtd-utils-ubifs \
"

IMAGE_INSTALL_append_colibri-imx6ull = " \
    u-boot-toradex-fsl-fw-utils \
    mtd-utils-ubifs \
    bluez-alsa \
    mwifiexap \
"

IMAGE_INSTALL += " \
    packagegroup-dotnet-deps \
    \
    wireless-regdb-static \
    \
    alsa-utils \
    alsa-utils-aplay \
    alsa-utils-amixer \
    \
    sqlite3 \
    \
    ${BACKPORTS_WIFI} \
    curl \
    canutils \
    nfs-utils-client \
    openssh-scp \
    openssh-sftp \
    \
    bzip2 \
    gdbserver \
    grep \
    joe \
    minicom \
    ldd \
    lsof \
    mtd-utils \
    dosfstools \
    util-linux-fstrim \
    \
    devmem2 \
    ethtool \
    evtest \
    hdparm \
    iperf3 \
    i2c-tools \
    libsoc \
    lmbench \
    memtester \
    mmc-utils-cos \
    nbench-byte \
    rt-tests \
    stress \
    tinymembench \
    util-linux-lsblk \
    \
    tdx-oak-sensors \
"

#Backports

BACKPORTS_WIFI = " \
    backports \
    linux-firmware-ath10k    \
    linux-firmware-sd8686    \
    linux-firmware-sd8688    \
    linux-firmware-sd8787    \
    linux-firmware-sd8797    \
    linux-firmware-sd8887    \
    linux-firmware-sd8997    \
    linux-firmware-ralink    \
    linux-firmware-rtl8192cu \
    linux-firmware-rtl8188eu \
    hostapd \
"
BACKPORTS_WIFI_EXTRA = " \
    linux-firmware-ath9k     \
    linux-firmware-iwlwifi   \
"

IMAGE_INSTALL_append_apalis-tk1 = " \
     ${BACKPORTS_WIFI_EXTRA} \
"

IMAGE_INSTALL_append_apalis-tk1-mainline = " \
     ${BACKPORTS_WIFI_EXTRA} \
"

IMAGE_INSTALL_remove_apalis-tk1-mainline = " \
    backports \
"

IMAGE_INSTALL_remove_colibri-t20 = " \
    linux-firmware-ath10k \
"

IMAGE_INSTALL_append_apalis-t30 = " \
    ${BACKPORTS_WIFI_EXTRA} \
"

IMAGE_INSTALL_append_apalis-imx6 = " \
    ${BACKPORTS_WIFI_EXTRA} \
"

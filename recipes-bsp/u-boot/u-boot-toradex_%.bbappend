FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += " \
    file://colibri_imx7.patch \
"

LOCALVERSION = "-${TDX_VER_ITEM}"

PV_append = "+git${SRCPV}"

do_configure_prepend() {
	rm ${WORKDIR}/git/tools/logos/toradex.bmp
	cp ${PWD}/../layers/meta-vilan/recipes-bsp/u-boot/files/toradex.bmp ${WORKDIR}/git/tools/logos/
}

SRCREV = "83a53c1c0c6fd813bd655b4f88fd07bf798e11d7"
SRCBRANCH = "2016.11-toradex"
SRCREV_use-head-next = "${AUTOREV}"
SRCBRANCH_use-head-next = "2016.11-toradex-next"

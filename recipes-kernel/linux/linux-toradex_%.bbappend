FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += " \
    file://imx7-colibri-eval-v3.patch \
    file://logo_custom_clut224.patch \
    file://mxc_lcdif.patch \
    file://Kconfig.patch \
    file://vt.patch \
"

LOCALVERSION = "-${TDX_VER_ITEM}"
PV_append = "+git${SRCPV}"

SRCREV = "d899927728beca8357a5b4120b690cb3c1d80844"
SRCBRANCH = "toradex_4.9-2.3.x-imx"
SRCREV_use-head-next = "${AUTOREV}"
SRCBRANCH_use-head-next = "toradex_4.9-2.3.x-imx-next"


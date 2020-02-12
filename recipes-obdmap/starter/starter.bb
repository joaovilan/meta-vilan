DESCRIPTION = "Starter obdmap"
HOMEPAGE = "http://chiptronic.com.br"
SECTION = "base"
DEPENDS = ""
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

ALTERNATIVE_PRIORITY = "10"

PROVIDES = "virtual/starter"
RPROVIDES_${PN} = "virtual-starter"
RCONFLICTS_${PN} = "exquisite"

PV = "0.2"
PR = "r1"

SRC_URI = "file://starter-0.2.tar.gz"

do_compile() {
         ${CC} main.c -o o
}

do_install_prepend() {
	install -d ${D}${sysconfdir}/init.d/
	install -m 0755 ${WORKDIR}/starter-0.2/starter.sh ${D}${sysconfdir}/init.d/starter.sh
	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/starter-0.2/o ${D}${bindir}/starter
	install -d ${D}${sysconfdir}/rc2.d/
	ln -s ${D}${sysconfdir}/init.d/starter.sh ${D}${sysconfdir}/rc2.d/S21starter.sh
	install -d ${D}${sysconfdir}/udev/rules.d/
	install -m 0755 ${WORKDIR}/starter-0.2/serial.rules ${D}${sysconfdir}/udev/rules.d/serial.rules
	install -d ${D}data/
	install -d ${D}data/soft-starter/
	install -d ${D}data/interface_logs/
	install -m 0755 ${WORKDIR}/starter-0.2/populate ${D}data/.populate
	install -m 0755 ${WORKDIR}/starter-0.2/populate ${D}data/soft-starter/.populate
	install -m 0755 ${WORKDIR}/starter-0.2/populate ${D}data/interface_logs/.populate
}

FILES_${PN}-dbg += "${sysconfdir}/init.d/.debug"

PACKAGES =+ "${PN}-support"

RDEPENDS_${PN} += "${PN}-support"
FILES_${PN}-support += "${sysconfdir}/init.d/starter.sh ${bindir}/starter ${sysconfdir}/rc2.d/S21starter.sh"

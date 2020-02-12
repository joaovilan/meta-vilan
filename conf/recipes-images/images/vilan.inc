IMAGE_INSTALL_BLUETOOTH = " \
    bluez5 \
    bluez-hcidump \
    obex-data-server \
    openobex \
    python \
    python-dbus \
    python-pygobject \
    dbus-glib \
"

IMAGE_INSTALL_JAVA = " \
    openjdk-7-jre \
"

IMAGE_INSTALL_X11VNC = " \
    x11vnc \
"

IMAGE_INSTALL_MPLAYER = " \
    mplayer2 \
"

IMAGE_INSTALL_BINUTILS = " \
    binutils \
"

IMAGE_INSTALL_PSPLASH = " \
    psplash \
"

IMAGE_INSTALL_STARTER = " \
    starter \
"

IMAGE_INSTALL += " \
    ${IMAGE_INSTALL_X11VNC} \
    ${IMAGE_INSTALL_BLUETOOTH} \
    ${IMAGE_INSTALL_BINUTILS} \
    ${IMAGE_INSTALL_JAVA} \
    ${IMAGE_INSTALL_PSPLASH} \
    ${IMAGE_INSTALL_STARTER} \
"


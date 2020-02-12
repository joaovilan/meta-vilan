IMAGE_INSTALL_NODE = " \
    packagegroup-core-buildessential \
    nodejs \
    git \
"

IMAGE_INSTALL_PV = " \
    pv \
"

IMAGE_INSTALL += " \
    ${IMAGE_INSTALL_NODE} \
    ${IMAGE_INSTALL_PV} \
"


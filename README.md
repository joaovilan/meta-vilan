# meta-vilan
Station: Ubuntu 16.04 64-bit 

Toradex Version: 2.7 

Distribution: Ångström v2016.12 

OpenEmbedded/Yocto Codename: morty 

Yocto Project Release: 2.2

Instalando os pacotes necessários:

$ sudo dpkg --add-architecture i386

$ sudo apt-get update

$ sudo apt-get install g++-5-multilib

$ sudo apt-get install curl dosfstools gawk g++-multilib gcc-multilib lib32z1-dev libcrypto++9v5:i386 libcrypto++-dev:i386 liblzo2-dev:i386 lzop libsdl1.2-dev libstdc++-5-dev:i386 libusb-1.0-0:i386 libusb-1.0-0-dev:i386 uuid-dev:i386 texinfo chrpath

$ cd /usr/lib; sudo ln -s libcrypto++.so.9.0.0 libcryptopp.so.6

Instalando a ferramenta repo:

$ mkdir ~/bin

$ export PATH=~/bin:$PATH

$ curl http://commondatastorage.googleapis.com/git-repo-downloads/repo > ~/bin/repo

$ chmod a+x ~/bin/repo

Configurando o Git:

$ sudo apt install git

$ git config --global user.name "João Vilan Júnior"

$ git config --global user.email joaovilan@chiptronic.com.br

Baixando o repositório:

$ mkdir oe-core

$ cd oe-core

$ repo init -u http://git.toradex.com/toradex-bsp-platform.git -b LinuxImageV2.7

$ repo sync

Iniciando a compilação
$ . export

Abra o arquivo build/conf/local.conf e escolha a máquina:
MACHINE ?= "colibri-imx7"

Comente a linha:
'INHERIT += "rm_work"'

Acrescente no final do arquivo a seguinte linha:
ACCEPT_FSL_EULA = "1"


Colocar a seguinte entrada no arquivo bblayers.conf:
${TOPDIR}/../layers/meta-vilan \

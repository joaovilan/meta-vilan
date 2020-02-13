# meta-vilan
|         Station       |Toradex Version|   Distribution  |OpenEmbedded/Yocto Codename|Yocto Project Release|
|-----------------------|---------------|-----------------|---------------------------|---------------------|
|  Ubuntu 16.04 64-bit  |      2.7      |Ångström v2016.12|          morty            |         2.2         |



## Instalando os pacotes necessários:

$ sudo dpkg --add-architecture i386

$ sudo apt-get update

$ sudo apt-get install g++-5-multilib

$ sudo apt-get install curl dosfstools gawk g++-multilib gcc-multilib lib32z1-dev libcrypto++9v5:i386 libcrypto++-dev:i386 liblzo2-dev:i386 lzop libsdl1.2-dev libstdc++-5-dev:i386 libusb-1.0-0:i386 libusb-1.0-0-dev:i386 uuid-dev:i386 texinfo chrpath

$ cd /usr/lib; sudo ln -s libcrypto++.so.9.0.0 libcryptopp.so.6



## Instalando a ferramenta repo:

$ mkdir ~/bin

$ export PATH=~/bin:$PATH

$ curl http://commondatastorage.googleapis.com/git-repo-downloads/repo > ~/bin/repo

$ chmod a+x ~/bin/repo



## Configurando o Git:

$ sudo apt install git

$ git config --global user.name "João Vilan Júnior"

$ git config --global user.email joaovilan@chiptronic.com.br



## Baixando o repositório:

$ mkdir oe-core

$ cd oe-core

$ repo init -u http://git.toradex.com/toradex-bsp-platform.git -b LinuxImageV2.7

$ repo sync



## Iniciando o ambiente
$ . export



Abra o arquivo **build/conf/local.conf** e escolha a máquina:

MACHINE ?= "colibri-imx7"



Comente a linha:

'INHERIT += "rm_work"'



Acrescente no final do arquivo a seguinte linha:

ACCEPT_FSL_EULA = "1"



## Adicionando a layer Java

$ cd

$ cd oe-core/layers

$ META_JAVA=65bd3d6eaa6ae4e06f1c04efb493f448cf6d619b 

$ git clone --no-checkout http://git.yoctoproject.org/git/meta-java

$ cd meta-java

$ git checkout -b mywork $META_JAVA

$ cd ..



Acrescentar no arquivo build/conf/bblayers.conf
${TOPDIR}/../layers/meta-java \



## Baixando o repositorio meta-vilan com as especificações da imagem para OBDMAP-CONNECT

$ cd oe-core/layers

$ git clone https://github.com/joaovilan/meta-vilan.git



Colocar a seguinte entrada no arquivo bblayers.conf:
${TOPDIR}/../layers/meta-vilan \



## Compilar tudo

$ bitbake -k obdmap-image

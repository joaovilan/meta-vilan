bin_PROGRAMS=o
CC=$(GCC)
LN_S='ln -s'
hellomake: main.c
o_SOURCES: main.c
install:
	cp -f o /usr/local/bin/starter
	cp -f starter.sh /etc/init.d/starter.sh
	chmod 755 /etc/init.d/starter.sh
	cd /etc/rc2.d && $(LN_S) ../init.d/starter.sh S20starter.sh
	cd /etc/rc3.d && $(LN_S) ../init.d/starter.sh S20starter.sh
	cd /etc/rc4.d && $(LN_S) ../init.d/starter.sh S20starter.sh

clean:
	rm -f o



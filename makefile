#  -*- c-basic-offset : 4 -*-

all: test staticlibimage doc

test:
	make -C ./src/image/ test

staticlibimage:
	make -C ./src/image/ staticlib

doc:
	doygen

distclean:
	make -C ./src/tinyimage/ clean
	rm -rf doc/html/

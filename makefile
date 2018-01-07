#  -*- c-basic-offset : 4 -*-

all: segmentate test staticlibimage doc

segmentate:
	make -C ./src/main/ Segmentate

test:
	make -C ./src/image/ test
	make -C ./src/main/ test

staticlibimage:
	make -C ./src/image/ staticlib

doc:
	doxygen

clean:
	make -C ./src/main/ clean
	make -C ./src/image/ clean
	rm -rf doc/

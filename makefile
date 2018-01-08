#  -*- c-basic-offset : 4 -*-

all: staticlibimage segmentate test doc

segmentate:
	make -C ./src/main/ Segmentate

test: segmentate
	make -C ./src/image/ test
	make -C ./src/main/ test

staticlibimage:
	make -C ./src/image/ staticlib

doc: segmentate
	doxygen

clean:
	make -C ./src/main/ clean
	make -C ./src/image/ clean
	rm -rf doc/*

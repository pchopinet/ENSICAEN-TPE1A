#  -*- c-basic-offset : 4 -*-

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -I "../../include/" -fpic
LDFLAGS=
PATHLIB=../../lib/
PATHOBJ=../../obj/main/
PATHBIN=../../bin/main/

all: test

test: testMoments testRAG

testRAG:
	true

testMoments: $(PATHOBJ)testMoments.o
	$(CC) -o $(PATHBIN)$@ $^ $(LDFLAGS)

$(PATHOBJ)%.o: %.c ../../include/*.h
	$(CC) $< $(CFLAGS) -c -o $@
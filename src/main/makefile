#  -*- c-basic-offset : 4 -*-

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -I "../../include/" -fpic
LDFLAGS=
PATHLIB=../../lib/
PATHOBJ=../../obj/main/
PATHBIN=../../bin/main/

all: test

test: testMoments testRAG testMerge

main: $(PATHOBJ)merge.o
	$(CC) -o $(PATHBIN)$@ $^ $(LDFLAGS) $(PATHLIB)libimage.a

testRAG: $(PATHOBJ)RAG.o $(PATHOBJ)testRAG.o $(PATHOBJ)moments.o
	$(CC) -o $(PATHBIN)$@ $^ $(LDFLAGS) $(PATHLIB)libimage.a

testMoments: $(PATHOBJ)testMoments.o $(PATHOBJ)moments.o
	$(CC) -o $(PATHBIN)$@ $^ $(LDFLAGS) $(PATHLIB)libimage.a

testMerge: $(PATHOBJ)merge.o $(PATHOBJ)testMerge.o $(PATHOBJ)RAG.o $(PATHOBJ)moments.o
	$(CC) -o $(PATHBIN)$@ $^ $(LDFLAGS) $(PATHLIB)libimage.a

$(PATHOBJ)merge.o: $(PATHOBJ)RAG.o

$(PATHOBJ)%.o: %.c ../../include/*.h
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -rf $(PATHOBJ)*.o

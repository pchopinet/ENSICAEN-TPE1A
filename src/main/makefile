#  -*- c-basic-offset : 4 -*-

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -I "../../include/" -fpic
LDFLAGS=
PATHLIB=../../lib/
PATHOBJ=../../obj/main/
PATHBIN=../../bin/main/
OBJ=$(PATHOBJ)RAG.o $(PATHOBJ)merge.o $(PATHOBJ)moments.o
all: test main

test: testMoments testRAG testMerge

main: $(OBJ)
	$(CC) -o $(PATHBIN)Segmentation $^ $(LDFLAGS) $(PATHLIB)libimage.a

testRAG: $(OBJ) $(PATHOBJ)testRAG.o $(OBJ)
	$(CC) -o $(PATHBIN)$@ $^ $(LDFLAGS) $(PATHLIB)libimage.a

testMoments: $(PATHOBJ)testMoments.o $(PATHOBJ)moments.o
	$(CC) -o $(PATHBIN)$@ $^ $(LDFLAGS) $(PATHLIB)libimage.a

testMerge:$(OBJ) $(PATHOBJ)testMerge.o
	$(CC) -o $(PATHBIN)$@ $^ $(LDFLAGS) $(PATHLIB)libimage.a

$(PATHOBJ)merge.o: $(PATHOBJ)RAG.o

$(PATHOBJ)%.o: %.c ../../include/*.h
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -rf $(PATHOBJ)*.o

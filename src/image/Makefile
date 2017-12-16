CC=gcc
CFLAGS=-Wall -Wextra -pedantic -I "../../include/" -fpic
LDFLAGS=
PATHLIB=../../lib/
PATHOBJ=../../obj/image/
PATHBIN=../../bin/image/
OBJ=$(PATHOBJ)main.o $(PATHOBJ)image.o

all: staticlib test

test: $(OBJ) staticlib
	$(CC) $(OBJ) -o $(PATHBIN)$@ $(PATHLIB)libimage.a

staticlib: $(OBJ)
	ar rcs $(PATHLIB)libimage.a $^

$(PATHOBJ)%.o: %.c ../../include/*.h
	$(CC) $< $(CFLAGS) -c -o $@
	
clean:
	rm -f $(PATHOBJ)*.o

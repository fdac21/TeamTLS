CC=gcc
CFLAGS=-g
DEPS = memhier.h
OBJ = memhier.o

all: memhier

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

memhier: memhier.o 
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o 
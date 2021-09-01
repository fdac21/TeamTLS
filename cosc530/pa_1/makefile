CC=g++
CFLAGS=-g
DEPS = memhier.h 
OBJ = memhier.o

all: memhier

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

memhier: memhier.o 
	$(CC) -o $@ $^ $(CFLAGS) -lm

.PHONY: clean

clean:
	rm -f *.o 
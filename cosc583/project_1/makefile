CC=gcc
CFLAGS=-g
DEPS = aes.h
OBJ = aes.o 

all: aes

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

aes: aes.o 
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o 
CC=g++
CFLAGS=-g
DEPS = dynamsched.h 
OBJ = dynamsched.o

all: dynamsched

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

memhier: dynamsched.o 
	$(CC) -o $@ $^ $(CFLAGS) 

.PHONY: clean

clean:
	rm -f *.o 
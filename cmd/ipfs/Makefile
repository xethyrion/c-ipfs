CC = gcc
CFLAGS = -O0
LFLAGS =
DEPS = init.h
OBJS = init.o main.o ../../commands/argument.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ipfs: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)
	
all: ipfs

clean:
	rm -f *.o
CC = gcc
CFLAGS = -O0
LFLAGS = 
DEPS = builder.h ipfs_node.h
OBJS = builder.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

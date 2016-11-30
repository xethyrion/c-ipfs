CC = gcc
CFLAGS = -O0 -I../include -I../../c-libp2p/include -I../../c-multihash/include -I../../c-multiaddr/include

ifdef DEBUG
CFLAGS += -g3
endif

LFLAGS = 
DEPS = ../include/blocks/block.h ../include/blocks/blockstore.h
OBJS = block.o blockstore.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

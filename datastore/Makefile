CC = gcc
CFLAGS = -O0 -I../include -I../../c-libp2p/include -I../../c-multihash/include -I../../c-multiaddr/include -I../../lmdb/libraries/liblmdb

ifdef DEBUG
CFLAGS += -g3
endif

LFLAGS = 
DEPS = ../include/ipfsdatastore/ds_helper.h
OBJS = ds_helper.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

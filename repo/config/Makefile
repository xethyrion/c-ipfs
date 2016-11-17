CC = gcc
CFLAGS = -O0 -I../../include -I../../../c-libp2p/include -I../../../c-multihash/include

ifdef DEBUG
CFLAGS += -g3
endif

LFLAGS = 
DEPS = config.h datastore.h identity.h
OBJS = config.o identity.o bootstrap_peers.o datastore.o gateway.o addresses.o swarm.o peer.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

CC = gcc
CFLAGS = -O0 -I../../include -I../../../c-libp2p/include

ifdef DEBUG
CFLAGS += -g3
endif

LFLAGS = 
DEPS = config.h datastore.h identity.h
OBJS = config.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

CC = gcc
CFLAGS = -O0 -I../../include -I../../../c-libp2p/include -I../../../lmdb/libraries/liblmdb

ifdef DEBUG
CFLAGS += -g3
endif

LFLAGS = 
DEPS = 
OBJS = fs_repo.o jsmn.o lmdb_datastore.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

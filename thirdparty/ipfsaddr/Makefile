CC = gcc
CFLAGS = -O0 -I../../include -I../../../c-libp2p/include
LFLAGS = 
DEPS = 
OBJS = ipfsaddr.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

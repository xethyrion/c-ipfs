CC = gcc
CFLAGS = -O0 -I../../include -I../../../c-libp2p/include
LFLAGS = 
DEPS = parse.h
OBJS = parse.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

CC = gcc
CFLAGS = -O0 -I../include -I../../c-libp2p/include

ifdef DEBUG
CFLAGS += -g3
endif

DEPS = 
OBJS =  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	cd config; make all;
	cd fsrepo; make all;
clean:
	rm -f *.o
	cd config; make clean;
	cd fsrepo; make clean;
CC = gcc
CFLAGS = -O0 -I../include -I../../c-libp2p/include
LFLAGS = 
DEPS = 
OBJS = argument.o command.o command_option.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	cd cli; make all;

clean:
	rm -f *.o
	cd cli; make clean;

CC = gcc
CFLAGS = -O0 -I../include -I../../c-libp2p/include
LFLAGS = 
DEPS = ../include/ipfs/commands/argument.h ../include/ipfs/commands/command_option.h \
	../include/ipfs/commands/command.h ../include/ipfs/commands/context.h \
	../include/ipfs/commands/req_log.h ../include/ipfs/commands/request.h
OBJS = argument.o command.o command_option.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	cd cli; make all;

clean:
	rm -f *.o
	cd cli; make clean;

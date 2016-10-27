CC = gcc
CFLAGS = -O0
LFLAGS = 
DEPS = argument.h command.h request.h command_option.h
OBJS = argument.o command.o command_option.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	cd cli; make all;

clean:
	rm -f *.o
	cd cli; make clean;

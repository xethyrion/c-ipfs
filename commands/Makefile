CC = gcc
CFLAGS = -O0
LFLAGS = 
DEPS = argument.h command.h
OBJS = argument.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o
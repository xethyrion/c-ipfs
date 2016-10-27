CC = gcc
CFLAGS = -O0
LFLAGS = 
DEPS = parse.h
OBJS = parse.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

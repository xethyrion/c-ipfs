CC = gcc
CFLAGS = -O0
LFLAGS = 
DEPS = utils.h
OBJS = utils.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

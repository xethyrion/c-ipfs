CC = gcc
CFLAGS = -O0
DEPS = repo.h
OBJS = repo.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o
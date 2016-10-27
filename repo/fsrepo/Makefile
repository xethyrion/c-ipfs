CC = gcc
CFLAGS = -O0
LFLAGS = 
DEPS = fs_repo.h
OBJS = fs_repo.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)

clean:
	rm -f *.o

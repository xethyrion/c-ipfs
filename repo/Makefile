CC = gcc
CFLAGS = -O0
DEPS = repo.h
OBJS = repo.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	cd config; make all;
	cd fsrepo; make all;
clean:
	rm -f *.o
	cd config; make clean;
	cd fsrepo; make clean;
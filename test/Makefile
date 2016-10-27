CC = gcc
CFLAGS = -O0
LFLAGS = 
DEPS = testit.h repo/test_repo.h cmd/ipfs/test_init.h
OBJS = testit.o ../cmd/ipfs/init.o ../commands/argument.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

testit: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

all: testit

clean:
	rm -f *.o
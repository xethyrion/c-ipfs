CC = gcc
CFLAGS = -O0
LFLAGS = 
DEPS = testit.h repo/test_repo.h cmd/ipfs/test_init.h
OBJS = testit.o ../cmd/ipfs/init.o ../commands/argument.o ../commands/command_option.o \
	../commands/command.o ../commands/cli/parse.o ../core/builder.o ../repo/fsrepo/fs_repo.o \
	../repo/fsrepo/fs_repo.o ../repo//config/config.o ../os/utils.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

testit: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

all: testit

clean:
	rm -f *.o
	rm -f testit
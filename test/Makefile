CC = gcc
CFLAGS = -O0 -I../include -I../../c-libp2p/include -g3
LFLAGS = -L../../c-libp2p -lp2p
DEPS = 
OBJS = testit.o ../cmd/ipfs/init.o ../commands/argument.o ../commands/command_option.o \
	../commands/command.o ../commands/cli/parse.o ../core/builder.o ../repo/fsrepo/fs_repo.o \
	../repo/fsrepo/fs_repo.o ../repo//config/config.o ../os/utils.o ../repo/config/identity.o \
	../repo/config/bootstrap_peers.o ../repo/config/datastore.o ../repo/config/gateway.o \
	../repo/config/addresses.o ../repo/config/swarm.o ../repo/config/peer.o \
	../thirdparty/ipfsaddr/ipfs_addr.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

testit: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

all: testit

clean:
	rm -f *.o
	rm -f testit
CC = gcc
CFLAGS = -O0 -I../include -I../../c-libp2p/include -I../../c-multihash/include -I../../c-multiaddr/ -g3 
LFLAGS = -L../../c-libp2p -L../../c-multihash -L../../c-multiaddr -lp2p -lm -lmultihash -lmultiaddr -lpthread
DEPS = cmd/ipfs/test_init.h repo/test_repo_bootstrap_peers.h repo/test_repo_config.h repo/test_repo_identity.h cid/test_cid.h
OBJS = testit.o ../cmd/ipfs/init.o ../commands/argument.o ../commands/command_option.o \
	../commands/command.o ../commands/cli/parse.o ../core/builder.o ../repo/fsrepo/fs_repo.o \
	../repo/fsrepo/jsmn.o ../repo/fsrepo/lmdb_datastore.o ../repo/config/config.o ../os/utils.o ../repo/config/identity.o \
	../repo/config/bootstrap_peers.o ../repo/config/datastore.o ../repo/config/gateway.o \
	../repo/config/addresses.o ../repo/config/swarm.o ../repo/config/peer.o \
	../thirdparty/ipfsaddr/ipfs_addr.o ../cid/cid.o ../multibase/multibase.o \
	../flatfs/flatfs.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test_ipfs: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS) ../../lmdb/libraries/liblmdb/liblmdb.a

all: test_ipfs

clean:
	rm -f *.o
	rm -f test_ipfs
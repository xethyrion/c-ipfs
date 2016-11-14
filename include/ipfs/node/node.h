/**
 * An implementation of an IPFS node
 * Copying the go-ipfs-node project
 */

#include <stdint.h>



struct Link {
	char* name;
	uint64_t size;
	struct Cid cid; // content id
};

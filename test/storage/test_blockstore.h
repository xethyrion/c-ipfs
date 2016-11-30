#include <string.h>

#include "ipfs/blocks/blockstore.h"
#include "ipfs/blocks/block.h"

int test_blockstore_put() {
	const unsigned char* input_string = "Hello, World!";
	struct Block* input_block;
	int retVal = 0;

	retVal = ipfs_blocks_block_new(input_string, strlen((const char*)input_string) + 1, &input_block);
	if (retVal == 0)
		return 0;

	retVal = ipfs_blockstore_put(input_block);
	if (retVal == 0)
		return 0;

	return 1;
}

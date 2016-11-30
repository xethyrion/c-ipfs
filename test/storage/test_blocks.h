#include "ipfs/blocks/block.h"

int test_blocks_new() {
	const char* input = "Hello, World!";
	int retVal = 0;
	struct Block* block;
	retVal = ipfs_blocks_block_new(input, strlen(input) + 1, &block);
	if (retVal == 0)
		return 0;

	// now examine the block
	if (strcmp(block->data, input) != 0)
		return 0;

	if (block->data_length != strlen(input) + 1)
		return 0;

	if (block->cid->codec != CID_PROTOBUF)
		return 0;

	if (block->cid->version != 0)
		return 0;

	if (block->cid->hash_length != 32)
		return 0;

	unsigned char result_hash[32] = {33, 153, 66, 187, 124, 250, 87, 12, 12, 73, 43, 247, 175, 153, 10, 51, 192, 195, 218, 69, 220, 170, 105, 179, 195, 0, 203, 213, 172, 3, 244, 10 };
	for(int i = 0; i < 32; i++) {
		if (block->cid->hash[i] != result_hash[i])
			return 0;
	}

	retVal = ipfs_blocks_block_free(block);

	return 1;
}

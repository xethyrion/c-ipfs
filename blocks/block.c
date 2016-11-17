/**
 * The implementation of methods around IPFS blocks
 */

#include <stdlib.h>

#include "ipfs/blocks/block.h"
#include "ipfs/cid/cid.h"

/***
 * Create a new block based on the incoming data
 * @param data the data to base the block on
 * @param data_size the length of the data array
 * @param block a pointer to the struct Block that will be created
 * @returns true(1) on success
 */
int ipfs_blocks_block_new(unsigned char* data, size_t data_size, struct Block* block) {
	char hash[32];
	ipfs_crypto_hashing_sha256(data, hash, 32);
	ipfs_cid_new(0, hash, 32, CID_PROTOBUF, block->cid);

	block->data = malloc(sizeof(unsigned char) * data_size);
	if (block->data == NULL)
		return 0;

	memcpy(block->data, data, data_size);
	return 1;
}

/***
 * Free resources used by the creation of a block
 * @param block the block to free
 * @returns true(1) on success
 */
int ipfs_blocks_block_free(struct Block* block) {
	ipfs_cid_free(block->cid);
	if (block->data != NULL)
		free(block->data);
	return 1;
}

/***
 * IPFS has the notion of storage blocks.
 */

#ifndef __IPFS_BLOCKS_BLOCK_H__
#define __IPFS_BLOCKS_BLOCK_H__

#include "ipfs/cid/cid.h"

struct Block {
	struct Cid* cid;
	unsigned char* data;
	size_t data_length;
};

/***
 * Create a new block based on the incoming data.
 * @param data the data to base the block on
 * @param data_size the length of the data array
 * @param block a pointer to the struct Block that will be created
 * @returns true(1) on success
 */
int ipfs_blocks_block_new(const unsigned char* data, size_t data_size, struct Block** block);

/***
 * Free resources used by the creation of a block
 * @param block the block to free
 * @returns true(1) on success
 */
int ipfs_blocks_block_free(struct Block* block);

#endif

/**
 * The implementation of methods around IPFS blocks
 */

#include <stdlib.h>
#include <string.h>

#include "libp2p/crypto/sha256.h"
#include "ipfs/blocks/block.h"
#include "ipfs/cid/cid.h"

/***
 * Create a new block based on the incoming data
 * @param data the data to base the block on
 * @param data_size the length of the data array
 * @param block a pointer to the struct Block that will be created
 * @returns true(1) on success
 */
int ipfs_blocks_block_new(const unsigned char* data, size_t data_size, struct Block** block) {

	// allocate memory for structure
	(*block) = (struct Block*)malloc(sizeof(struct Block));
	if ((*block) == NULL)
		return 0;

	// cid
	unsigned char hash[32];
	if (libp2p_crypto_hashing_sha256(data, data_size, &hash[0]) == 0) {
		free(*block);
		return 0;
	}

	if (ipfs_cid_new(0, hash, 32, CID_PROTOBUF, &((*block)->cid)) == 0) {
		free(*block);
		return 0;
	}

	(*block)->data_length = data_size;

	(*block)->data = malloc(sizeof(unsigned char) * data_size);
	if ( (*block)->data == NULL) {
		ipfs_cid_free((*block)->cid);
		free(*block);
		return 0;
	}

	memcpy( (*block)->data, data, data_size);
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
	free(block);
	return 1;
}

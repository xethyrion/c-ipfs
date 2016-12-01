/***
 * a thin wrapper over a datastore for getting and putting block objects
 */
#include "libp2p/crypto/encoding/base32.h"
#include "ipfs/cid/cid.h"
#include "ipfs/blocks/block.h"
#include "ipfs/datastore/ds_helper.h"
#include "ipfs/repo/fsrepo/fs_repo.h"

/**
 * Delete a block based on its Cid
 * @param cid the Cid to look for
 * @param returns true(1) on success
 */
int ipfs_blockstore_delete(struct Cid* cid, struct FSRepo* fs_repo) {
	return 0;
}

/***
 * Determine if the Cid can be found
 * @param cid the Cid to look for
 * @returns true(1) if found
 */
int ipfs_blockstore_has(struct Cid* cid, struct FSRepo* fs_repo) {
	return 0;
}

/***
 * Find a block based on its Cid
 * @param cid the Cid to look for
 * @param block where to put the data to be returned
 * @returns true(1) on success
 */
int ipfs_blockstore_get(struct Cid* cid, struct Block* block, struct FSRepo* fs_repo) {
	return 0;
}

/***
 * Put a block in the blockstore
 * @param block the block to store
 * @returns true(1) on success
 */
int ipfs_blockstore_put(struct Block* block, struct FSRepo* fs_repo) {
	// from blockstore.go line 118
	// Get Datastore key, which is a base32 key of the binary,
	size_t key_length = libp2p_crypto_encoding_base32_encode_size(block->data_length);
	unsigned char key[key_length];
	int retVal = ipfs_datastore_helper_ds_key_from_binary(block->data, block->data_length, &key[0], key_length, &key_length);
	if (retVal == 0)
		return 0;

	// send to Put with key
	fs_repo->config->datastore->datastore_put(key, key_length, block, fs_repo->config->datastore);
	return 0;
}

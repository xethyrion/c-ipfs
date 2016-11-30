/***
 * a thin wrapper over a datastore for getting and putting block objects
 */

#ifndef __IPFS_BLOCKS_BLOCKSTORE_H__
#ifndef __IPFS_BLOCKS_BLOCKSTORE_H__

/**
 * Delete a block based on its Cid
 * @param cid the Cid to look for
 * @param returns true(1) on success
 */
int ipfs_blockstore_delete(struct Cid* cid, struct FSRepo* fs_repo);

/***
 * Determine if the Cid can be found
 * @param cid the Cid to look for
 * @returns true(1) if found
 */
int ipfs_blockstore_has(struct Cid* cid, struct FSRepo* fs_repo);

/***
 * Find a block based on its Cid
 * @param cid the Cid to look for
 * @param block where to put the data to be returned
 * @returns true(1) on success
 */
int ipfs_blockstore_get(struct Cid* cid, struct Block* block, struct FSRepo* fs_repo);

/***
 * Put a block in the blockstore
 * @param block the block to store
 * @returns true(1) on success
 */
int ipfs_blockstore_put(struct Block* block, struct FSRepo* fs_repo);


#endif

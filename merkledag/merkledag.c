/**
 * A basic storage building block of the IPFS system
 */

/***
 * Adds a node to the dagService and blockService
 * @param node the node to add
 * @returns true(1) on success
 */
int ipfs_merkledag_add(struct Node* node) {
	// taken from merkledag.go line 59
	// TODO: put in blockstore
	// TODO: call HasBlock (unsure why as yet)

}

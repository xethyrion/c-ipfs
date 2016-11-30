/***
 * A content id
 */

#ifndef __IPFS_CID_CID_H
#define __IPFS_CID_CID_H

#include <stddef.h>

#define CID_PROTOBUF 0x70
#define CID_CBOR 0x71
#define CID_RAW 0x72
#define CID_JSON 0x73
#define CID_ETHEREUM_BLOCK 0x90
#define CID_ETHEREUM_TX 0x91
#define CID_BITCOIN_BLOCK 0xb0
#define CID_BITCOIN_TX 0xb1
#define CID_ZCASH_BLOCK 0xc0
#define CID_ZCASH_TX 0xc1

struct Cid {
	int version;
	char codec;
	unsigned char* hash; // a multihash
	size_t hash_length;
};


/**
 * Create a new CID based on the given hash
 * @param version the version
 * @param hash the multihash
 * @param hash_length the length of the multihash in bytes
 * @param codec the codec to be used (NOTE: For version 0, this should be CID_PROTOBUF)
 * @param cid where to put the results
 * @returns true(1) on success
 */
int ipfs_cid_new(int version, unsigned char* hash, size_t hash_length, const char codec, struct Cid** cid);

/***
 * Free the resources from a Cid
 * @param cid the struct
 * @returns 1
 */
int ipfs_cid_free(struct Cid* cid);

/***
 * Fill a Cid struct based on a base 58 encoded string
 * @param incoming the string
 * @param incoming_size the size of the string
 * @cid the Cid struct to fill
 * @return true(1) on success
 */
int ipfs_cid_decode_from_string(const unsigned char* incoming, size_t incoming_length, struct Cid** cid);

/***
 * Turn a multibase decoded string of bytes into a Cid struct
 * @param incoming the multibase decoded array
 * @param incoming_size the size of the array
 * @param cid the Cid structure to fill
 */
int ipfs_cid_cast(unsigned char* incoming, size_t incoming_size, struct Cid* cid);

#endif

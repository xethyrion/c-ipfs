#include <string.h>

#include "mh/hashes.h"
#include "mh/multihash.h"

#include "ipfs/cid/cid.h"
#include "ipfs/multibase/multibase.h"

#include "libp2p/crypto/hashing/sha256.h"

int test_cid_new_free() {

	struct Cid cid;
	const unsigned char* hash = "ABC123";
	int retVal = cid_new(0, (unsigned char*)hash, strlen((char*)hash), CID_PROTOBUF, &cid);
	if (retVal == 0)
		return 0;

	if (cid.version != 0)
		return 0;

	if (cid.codec != CID_PROTOBUF)
		return 0;

	if (cid.hash_length != strlen((char*)hash))
		return 0;

	if (strncmp((char*)cid.hash, (char*)hash, 6) != 0)
		return 0;

	return cid_free(&cid);
}

/***
 * Test sending a multibase encoded multihash into cid_cast method
 * that should return a Cid struct
 */
int test_cid_cast_multihash() {
	// first, build a multihash
	char* string_to_hash = "Hello, World!";
	unsigned char hashed[32];
	memset(hashed, 0, 32);
	// hash the string
	libp2p_crypto_hashing_sha256(string_to_hash, strlen(string_to_hash), hashed);
	size_t multihash_size = mh_new_length(MH_H_SHA2_256, 32);
	unsigned char multihash[multihash_size];
	memset(multihash, 0, multihash_size);
	unsigned char* ptr = multihash;

	int retVal = mh_new(ptr, MH_H_SHA2_256, hashed, 32);
	if (retVal < 0)
		return 0;

	// now call cast
	struct Cid cid;
	retVal = cid_cast(multihash, multihash_size, &cid);
	if (retVal == 0)
		return 0;
	// check results
	if (cid.version != 0)
		return 0;
	if (cid.hash_length != 32)
		return 0;
	if (cid.codec != CID_PROTOBUF)
		return 0;
	if (strncmp(hashed, cid.hash, 32) != 0)
		return 0;

	return 1;
}

int test_cid_cast_non_multihash() {
	// this should turn a multibase encoded string into a cid struct
	// first, build a multibase encoded string
	// now call cast
	// check results
	return 0;
}


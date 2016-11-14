/**
 * Content ID
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipfs/cid/cid.h"

#include "libp2p/crypto/encoding/base58.h"
#include "ipfs/multibase/multibase.h"
#include "mh/multihash.h"
#include "multiaddr/varint.h"


/**
 * Create a new CID based on the given hash
 * @param version the version
 * @param hash the multihash
 * @param hash_length the length of the multihash in bytes
 * @param codec the codec to be used (NOTE: For version 0, this should be CID_PROTOBUF)
 * @param cid where to put the results
 * @returns true(1) on success
 */
int cid_new(int version, unsigned char* hash, size_t hash_length, const char codec, struct Cid* cid) {
	// allocate memory
	cid->hash = malloc(sizeof(unsigned char) * hash_length);
	if (cid->hash == NULL)
		return 0;
	// assign values
	cid->version = version;
	cid->codec = codec;
	memcpy(cid->hash, hash, hash_length);
	cid->hash_length = hash_length;

	return 1;

}

/***
 * Free the resources from a Cid
 * @param cid the struct
 * @returns 1
 */
int cid_free(struct Cid* cid) {
	if (cid->hash != NULL)
		free(cid->hash);
	return 1;
}

/***
 * Fill a Cid struct based on a base 58 encoded string
 * @param incoming the string
 * @param incoming_size the size of the string
 * @cid the Cid struct to fill
 * @return true(1) on success
 */
int cid_decode_from_string(const unsigned char* incoming, size_t incoming_length, struct Cid* cid) {
	int retVal = 0;

	if (incoming_length < 2)
		return 0;

	// is this a sha_256 multihash?
	if (incoming_length == 46 && incoming[0] == 'Q' && incoming[1] == 'm') {
		size_t hash_length = libp2p_crypto_encoding_base58_decode_size(incoming_length);
		unsigned char hash[hash_length];
		unsigned char* ptr = hash;
		retVal = libp2p_crypto_encoding_base58_decode(incoming, incoming_length, &ptr, &hash_length);
		if (retVal == 0)
			return 0;
		// now we have the hash, build the object
		return cid_new(0, hash, hash_length, CID_PROTOBUF, cid);
	}

	// TODO: finish this
	/*
	// it wasn't a sha_256 multihash, try to decode it using multibase
	size_t buffer_size = multibase_decode_size(incoming_length);
	if (buffer_size == 0)
		return 0;
	unsigned char buffer[buffer_size];

	memset(buffer, 0, buffer_size);

	retVal = multibase_decode(incoming, incoming_length, buffer, buffer_size, &buffer_size);
	if (retVal == 0)
		return 0;

	return cid_cast(buffer, buffer_size, cid);
	*/
	return 0;
}

/***
 * Turn a multibase decoded string of bytes into a Cid struct
 * @param incoming the multibase decoded array
 * @param incoming_size the size of the array
 * @param cid the Cid structure to fill
 */
int cid_cast(unsigned char* incoming, size_t incoming_size, struct Cid* cid) {
	// this is a multihash
	if (incoming_size == 34 && incoming[0] == 18 && incoming[1] == 32) {
		cid->hash_length = mh_multihash_length(incoming, incoming_size);
		cid->codec = CID_PROTOBUF;
		cid->version = 0;

		mh_multihash_digest(incoming, incoming_size, &cid->hash, &cid->hash_length);
		return 1;
	}

	// This is not a multihash. Perhaps it is using varints. Try to peel the information out of the bytes.
	// first the version
	int pos = 0, retVal = 0;
	size_t num_bytes = 0;
	num_bytes = uvarint_decode32(&incoming[pos], incoming_size - pos, &cid->version);
	if (num_bytes < 0 || cid->version > 1 || cid->version < 0)
		return 0;
	pos = num_bytes;
	// now the codec
	uint32_t codec = 0;
	num_bytes = uvarint_decode32(&incoming[pos], incoming_size - pos, &codec);
	if (num_bytes < 0)
		return 0;
	cid->codec = codec;
	pos += num_bytes;
	// now what is left
	cid->hash_length = incoming_size - pos;
	cid->hash = &incoming[pos];

	return 1;
}

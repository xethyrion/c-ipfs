/**
 * an "Identity" 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipfs/repo/config/identity.h"
#include "libp2p/crypto/rsa.h"
#include "libp2p/peerutils.h"
#include "libp2p/crypto/encoding/base64.h"
#include "libp2p/crypto/encoding/x509.h"

/**
 * Builds the Peer ID using the private key, and places it in the identity->peer_id
 * @param identity Where to get the DER of the private key
 * @returns true(1) on success
 */
int repo_config_identity_build_peer_id(struct Identity* identity) {
	// ic key and PeerID
	char hash[32];
	ID_FromPK_non_null_terminated(hash, identity->private_key.der, identity->private_key.der_length);

	// peer id is multihashed
	size_t sz = 255;
	char results[sz];
	if (PrettyID(results, &sz, hash, 32) == 0)
		return 0;

	// copy it into the structure
	identity->peer_id = (char*)malloc(sz + 1);
	if (identity->peer_id == NULL)
		return 0;

	strncpy(identity->peer_id, results, sz);
	identity->peer_id[sz] = 0;
	return 1;
}

/***
 * public methods
 */

/***
 * Initializes a new Identity. NOTE: This builds a new private/public key pair
 * @param identity the identity to fill
 * @param num_bits_for_keypair the number of bits for the keypair
 * @returns true(1) on success, false(0) otherwise
 */
int repo_config_identity_init(struct Identity* identity, unsigned long num_bits_for_keypair) {
	if (num_bits_for_keypair < 1024)
		return 0;
	// generate the private key (& public)
	if (!libp2p_crypto_rsa_generate_keypair( &(identity->private_key), num_bits_for_keypair))
		return 0;

	repo_config_identity_build_peer_id(identity);
	
	return 1;
}

int repo_config_identity_new(struct Identity** identity) {
	*identity = (struct Identity*)malloc(sizeof(struct Identity));
	if (*identity == NULL)
		return 0;

	memset(*identity, 0, sizeof(struct Identity));
	return 1;
}

int repo_config_identity_free(struct Identity* identity) {
	if (identity != NULL) {
		if (identity->private_key.public_key_der != NULL)
			free(identity->private_key.public_key_der);
		if (identity->private_key.der != NULL)
			free(identity->private_key.der);
		free(identity);
	}
	return 1;
}

/***
 * Build a RsaPrivateKey struct from a base64 string of the private key
 * @param identity where to put the new struct
 * @param base64 the null terminated base 64 encoded private key in DER format
 * @returns true(1) on success
 */
int repo_config_identity_build_private_key(struct Identity* identity, const char* base64) {
	size_t decoded_size = libp2p_crypto_encoding_base64_decode_size(strlen(base64));
	unsigned char decoded[decoded_size];

	int retVal = libp2p_crypto_encoding_base64_decode(base64, strlen(base64), decoded, decoded_size, &decoded_size);
	if (retVal == 0)
		return 0;

	// now convert DER to RsaPrivateKey
	retVal = libp2p_crypto_encoding_x509_der_to_private_key(decoded, decoded_size, &identity->private_key);
	if (retVal == 0)
		return 0;

	// now build the private key DER
	retVal = libp2p_crypto_rsa_private_key_fill_public_key(&identity->private_key);
	if (retVal == 0)
		return 0;

	// now build PeerID
	retVal = repo_config_identity_build_peer_id(identity);

	return retVal;
}

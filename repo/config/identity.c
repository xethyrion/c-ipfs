/**
 * an "Identity" 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipfs/repo/config/identity.h"
#include "libp2p/crypto/rsa.h"

/***
 * public methods
 */

/***
 * initializes a new Identity
 * @param identity the identity to fill
 * @param num_bits_for_keypair the number of bits for the keypair
 * @returns true(1) on success, false(0) otherwise
 */
int repo_config_identity_new(struct Identity* identity, unsigned long num_bits_for_keypair) {
	if (num_bits_for_keypair < 1024)
		return 0;
	// generate the private key (& public)
	if (!crypto_rsa_generate_keypair( &(identity->private_key), num_bits_for_keypair))
		return 0;
	
	// TODO: Get ID from public key
	// TODO: Store peer id in identity struct
	return 1;
}

int repo_config_identity_free(struct Identity* identity) {
	if (identity->private_key.der != NULL)
		free(identity->private_key.der);
	return 0;
}

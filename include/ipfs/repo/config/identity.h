//
//  identity.h
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef __REPO_CONFIG_IDENTITY_H__
#define __REPO_CONFIG_IDENTITY_H__

#include "libp2p/crypto/rsa.h"

struct Identity {
	char* peer_id; // a pretty-printed hash of the public key
	struct RsaPrivateKey private_key; // a private key
};

/***
 * initializes a new keypair, and puts it in the Identity struct
 */
int repo_config_identity_init(struct Identity* identity, unsigned long num_bits_for_keypair);

/***
 * Build a RsaPrivateKey struct from a base64 string of the private key
 * @param identity where to put the new struct
 * @param base64 the base 64 encoded private key in DER format
 * @returns true(1) on success
 */
int repo_config_identity_build_private_key(struct Identity* identity, const char* base64);

/***
 * Frees resources held by Identity
 * @param identity the identity that we're cleaning up
 * @returns true(0)
 */
int repo_config_identity_free(struct Identity* identity);

/***
 * Initializes a new identity struct that will need to be identity_free'd
 * @param identity the identity to allocate memory for
 * @returns true(1) on success
 */
int repo_config_identity_new(struct Identity** identity);

#endif /* identity_h */

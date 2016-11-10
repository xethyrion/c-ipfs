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
int repo_config_identity_new(struct Identity* identity, unsigned long num_bits_for_keypair);

/***
 * Frees resources held by Identity
 * @param identity the identity that we're cleaning up
 * @returns true(0)
 */
int repo_config_identity_free(struct Identity* identity);

#endif /* identity_h */

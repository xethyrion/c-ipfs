//
//  identity.h
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef __REPO_CONFIG_IDENTITY_H__
#define __REPO_CONFIG_IDENTITY_H__

struct PublicKey {
	unsigned long long modulus;
	unsigned long long exponent;
};

struct CRTValue {
	unsigned long long exponent;
	unsigned long long coefficient;
	unsigned long long r;
};

struct PrecomputedValues {
	unsigned long long dp;
	unsigned long long dq;
	unsigned long long q_inv;
	struct CRTValue** crt_values;
};

struct PrivateKey {
	struct PublicKey public_key;
	unsigned long long private_exponent;
	unsigned long long prime1;
	unsigned long long prime2;
	struct PrecomputedValues precomputed_values;
};

struct Identity {
	char* peer_id; // a pretty-printed hash of the public key
	struct PrivateKey private_key; // a private key
};

/***
 * initializes a new keypair, and puts it in the Identity struct
 */
int repo_config_identity_new(struct Identity* identity, unsigned long num_bits_for_keypair);

#endif /* identity_h */

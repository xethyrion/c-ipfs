//
//  identity.h
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef __REPO_CONFIG_IDENTITY_H__
#define __REPO_CONFIG_IDENTITY_H__

struct Identity {
	char* peer_id; // a pretty-printed public key
	char* private_key; // a private key
};

/***
 * initializes a new keypair, and puts it in the Identity struct
 */
int repo_config_identity_new(struct Identity* identity, unsigned long num_bits_for_keypair);

#endif /* identity_h */

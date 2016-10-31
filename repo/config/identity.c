//
//  identity.c
//  c-ipfs
//
//  Created by John Jones on 10/31/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "ipfs/repo/config/identity.h"

int repo_config_identity_generate_keypair(char* private_key, char* public_key) {
	return 0;
}

/***
 * public methods
 */

/***
 * initializes a new Identity (this should be moved to identity.h)
 * @param identity the identity to fill
 * @param num_bits_for_keypair the number of bits for the keypair
 * @returns true(1) on success, false(0) otherwise
 */
int repo_config_identity_new(struct Identity* identity, unsigned long num_bits_for_keypair) {
	identity = malloc(sizeof(struct Identity));
	if (num_bits_for_keypair < 1024)
		return 0;
	char* private_key;
	char* public_key;
	if (!repo_config_identity_generate_keypair(private_key, public_key))
		return 0;
	return 0;
}

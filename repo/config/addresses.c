//
//  addresses.c
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipfs/repo/config/addresses.h"

char* alloc_and_copy(char* source) {
	unsigned long strLen = strlen(source);
	char* result = malloc(sizeof(char) * (strLen + 1));
	strncpy(result, source, strLen);
	result[strLen] = 0;
	return result;
}

int repo_config_addresses_init(struct Addresses* addresses, char* api, char* gateway) {
	// allocate memory to store api and gateway
	addresses->api = alloc_and_copy(api);
	addresses->gateway = alloc_and_copy(gateway);
	if (addresses->api == NULL || addresses->gateway == NULL)
		return 0;
	return 1;
}

int repo_config_addresses_free(struct Addresses* addresses) {
	free(addresses->api);
	free(addresses->gateway);
	repo_config_swarm_address_free(&(addresses->swarm));
	return 1;
}

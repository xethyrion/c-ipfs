//
//  swarm.c
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipfs/repo/config/swarm.h"

int repo_config_swarm_address_init(struct SwarmAddresses* swarm_addresses, char** addresses, int array_length) {
	// allocate memory for the addresses array
	swarm_addresses->addresses = malloc(sizeof(char*) * array_length);
	if (swarm_addresses->addresses == NULL)
		return 0;
	
	// copy in all the strings
	for(int i = 0; i < array_length; i++) {
		char* newString = malloc(sizeof(char) * (strlen(addresses[i]) + 1));
		if (newString == NULL)
			return 0;
		strncpy(newString, addresses[i], strlen(addresses[i]));
		newString[strlen(addresses[i])] = 0;
		swarm_addresses->addresses[i] = newString;
	}
	
	swarm_addresses->num_addresses = array_length;
	
	return 1;
}

int repo_config_swarm_address_free(struct SwarmAddresses* swarm_addresses) {
	for (int i = 0; i < swarm_addresses->num_addresses; i++) {
		free(swarm_addresses->addresses[i]);
	}
	free(swarm_addresses->addresses);
	return 1;
}

//
//  swarm.h
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef swarm_h
#define swarm_h

struct SwarmAddresses {
	int num_addresses;
	char** addresses;
};

/**
 * add addresses to the SwarmAddresses struct
 * @param swarm_addresses the structure
 * @param addresses the array of addresses to store
 * @param array_length the number of elements in addresses array
 * @returns true(1) on success
 */
int repo_config_swarm_address_init(struct SwarmAddresses* swarm_addresses, char** addresses, int array_length);

/***
 * free up memory from repo_config_swarm_address_init
 * @param swarm_addresses the structure
 * @returns true(1)
 */
int repo_config_swarm_address_free(struct SwarmAddresses* swarm_addresses);

#endif /* swarm_h */

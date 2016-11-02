//
//  bootstrap_peer.h
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef bootstrap_peer_h
#define bootstrap_peer_h

#include "ipfs/thirdparty/ipfsaddr/ipfs_addr.h"

struct BootstrapPeers {
	int num_peers;
	struct IPFSAddr** peers;
};

/***
 * get a list of peers to use to bootstrap the instance
 * @param bootstrap_peers an array of IPFSAddr structs, will be allocated by this function
 * @returns true(1) on success, otherwise false(0)
 */
int repo_config_bootstrap_peers_retrieve(struct BootstrapPeers* bootstrap_peers);

/***
 * frees up memory caused by call to repo_config_bootstrap_peers_retrieve
 * @param list the list to free
 * @returns true(1)
 */
int repo_config_bootstrap_peers_free(struct BootstrapPeers* list);

#endif /* bootstrap_peer_h */

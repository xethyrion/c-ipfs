//
//  bootstrap_peers.c
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "ipfs/thirdparty/ipfsaddr/ipfs_addr.h"
#include "ipfs/repo/config/bootstrap_peers.h"

int repo_config_bootstrap_peers_retrieve(struct BootstrapPeers* list) {

	char* default_bootstrap_addresses[] = {
		"/ip4/104.131.131.82/tcp/4001/ipfs/QmaCpDMGvV2BGHeYERUEnRQAwe3N8SzbUtfsmvsqQLuvuJ",  // mars.i.ipfs.io
		"/ip4/104.236.176.52/tcp/4001/ipfs/QmSoLnSGccFuZQJzRadHn95W2CrSFmZuTdDWP8HXaHca9z",  // neptune.i.ipfs.io
		"/ip4/104.236.179.241/tcp/4001/ipfs/QmSoLPppuBtQSGwKDZT2M73ULpjvfd3aZ6ha4oFGL1KrGM", // pluto.i.ipfs.io
		"/ip4/162.243.248.213/tcp/4001/ipfs/QmSoLueR4xBeUbY9WZ9xGUUxunbKWcrNFTDAadQJmocnWm", // uranus.i.ipfs.io
		"/ip4/128.199.219.111/tcp/4001/ipfs/QmSoLSafTMBsPKadTEgaXctDQVcqN88CNLHXMkTNwMKPnu", // saturn.i.ipfs.io
		"/ip4/104.236.76.40/tcp/4001/ipfs/QmSoLV4Bbm51jM9C4gDYZQ9Cy3U6aXMJDAbzgu2fzaDs64",   // venus.i.ipfs.io
		"/ip4/178.62.158.247/tcp/4001/ipfs/QmSoLer265NRgSp2LA3dPaeykiS1J6DifTC88f5uVQKNAd",  // earth.i.ipfs.io
		"/ip4/178.62.61.185/tcp/4001/ipfs/QmSoLMeWqB7YGVLJN3pNLQpmmEk35v6wYtsMGLzSr5QBU3",   // mercury.i.ipfs.io
		"/ip4/104.236.151.122/tcp/4001/ipfs/QmSoLju6m7xTh3DuokvT3886QRYqxAzb1kShaanJgW36yx", // jupiter.i.ipfs.io
	};
	
	list->num_peers = 9;
	// allocate memory for list
	list->peers = malloc(sizeof(struct IPFSAddr*) * list->num_peers);
	if (list->peers == NULL)
		return 0;
	
	for(int i = 0; i < list->num_peers; i++) {
		// allocate memory for struct
		struct IPFSAddr* currAddr = (struct IPFSAddr*)malloc(sizeof(struct IPFSAddr));
		if (currAddr == NULL)
			return 0;
		// allocate memory for string
		if (ipfsaddr_init_new(&currAddr, default_bootstrap_addresses[i]) == 0)
			return 0;
		list->peers[i] = currAddr;
	}
	return 1;
}

int repo_config_bootstrap_peers_free(struct BootstrapPeers* list) {
	
	for(int i = 0; i < list->num_peers; i++) {
		if (list->peers[i] != NULL) {
			ipfsaddr_free(list->peers[i]);
			free(list->peers[i]);
		}
	}
	return 1;
}

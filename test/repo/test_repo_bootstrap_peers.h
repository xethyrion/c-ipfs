//
//  test_repo_bootstrap_peers.h
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef test_repo_bootstrap_peers_h
#define test_repo_bootstrap_peers_h

//#include <string.h>

#include "ipfs/repo/config/bootstrap_peers.h"

int test_repo_bootstrap_peers_init() {

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

	struct BootstrapPeers list;
	int retVal = 1;
	repo_config_bootstrap_peers_retrieve(&list);
	if ( list.num_peers != 9) {
		printf("Size does not equal 9 in test_repo_bootstrap_peers_init");
		retVal = 0;
	}
	for(int i = 0; i < list.num_peers; i++) {
		unsigned long strLen = strlen(default_bootstrap_addresses[i]);
		if (strncmp(list.peers[i]->entire_string, default_bootstrap_addresses[i], strLen) != 0)
			printf("The value of element %d is: %s\n", i, list.peers[i]->entire_string);
	}
	repo_config_bootstrap_peers_free(&list);
	return retVal;
}

#endif /* test_repo_bootstrap_peers_h */

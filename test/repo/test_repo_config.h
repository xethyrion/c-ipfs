//
//  test_repo_config.h
//  c-ipfs
//
//  Created by John Jones on 10/31/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef test_repo_config_h
#define test_repo_config_h

#include "ipfs/repo/config/config.h"

int test_repo_config_init() {
	struct RepoConfig repoConfig;
	int retVal = repo_config_init(&repoConfig, 2048, "/Users/JohnJones/.ipfs");
	if (retVal == 0)
		return 0;
	
	// now tear it apart to check for anything broken

	// addresses
	retVal = strncmp(repoConfig.addresses.api, "/ip4/127.0.0.1/tcp/5001", 23);
	if (retVal != 0)
		return 0;
	retVal = strncmp(repoConfig.addresses.gateway, "/ip4/127.0.0.1/tcp/8080", 23);
	if (retVal != 0)
		return 0;
	
	// datastore
	retVal = strncmp(repoConfig.datastore.path, "/Users/JohnJones/.ipfs/datastore", 32);
	if (retVal != 0)
		return 0;
	
	repo_config_free(&repoConfig);
	
	return 1;
}

#endif /* test_repo_config_h */

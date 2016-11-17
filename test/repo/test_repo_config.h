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
#include "ipfs/repo/fsrepo/fs_repo.h"
#include "ipfs/os/utils.h"

int test_repo_config_init() {
	struct RepoConfig* repoConfig;
	int retVal = ipfs_repo_config_new(&repoConfig);
	if (retVal == 0)
		return 0;

	retVal = repo_config_init(repoConfig, 2048, "/Users/JohnJones/.ipfs");
	if (retVal == 0)
		return 0;
	
	// now tear it apart to check for anything broken

	// addresses
	retVal = strncmp(repoConfig->addresses.api, "/ip4/127.0.0.1/tcp/5001", 23);
	if (retVal != 0)
		return 0;
	retVal = strncmp(repoConfig->addresses.gateway, "/ip4/127.0.0.1/tcp/8080", 23);
	if (retVal != 0)
		return 0;
	
	if (repoConfig->addresses.swarm.num_addresses != 2)
		return 0;
	
	retVal = strncmp(repoConfig->addresses.swarm.addresses[0], "/ip4/0.0.0.0/tcp/4001", 21);
	if (retVal != 0)
		return 0;
	
	retVal = strncmp(repoConfig->addresses.swarm.addresses[1], "/ip6/::/tcp/4001", 16);
	if (retVal != 0)
		return 0;
	
	// datastore
	retVal = strncmp(repoConfig->datastore.path, "/Users/JohnJones/.ipfs/datastore", 32);
	if (retVal != 0)
		return 0;
	
	ipfs_repo_config_free(repoConfig);
	
	return 1;
}

/***
 * test the writing of the config file
 */
int test_repo_config_write() {
	// first delete the existing one
	unlink("/tmp/.ipfs/config");
	
	// now build a new one
	struct RepoConfig* repoConfig;
	ipfs_repo_config_new(&repoConfig);
	if (!repo_config_init(repoConfig, 2048, "/tmp/.ipfs")) {
		ipfs_repo_config_free(repoConfig);
		return 0;
	}
	
	if (!fs_repo_write_config_file("/tmp/.ipfs", repoConfig)) {
		ipfs_repo_config_free(repoConfig);
		return 0;
	}

	ipfs_repo_config_free(repoConfig);
	
	// check to see if the file exists
	return os_utils_file_exists("/tmp/.ipfs/config");
}

#endif /* test_repo_config_h */

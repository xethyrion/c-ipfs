//
//  config.c
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "ipfs/os/utils.h"
#include "ipfs/repo/config/bootstrap_peers.h"
#include "ipfs/repo/config/swarm.h"

/***
 * public
 */

/***
 * gets the default path from the environment variable and/or the homedir struct
 * NOTE: this allocates memory for result. Clean up after yourself.
 * @param result where the result string will reside.
 * @returns true(1) on success, or false(0)
 */
int config_get_default_path_root(char* result) {
	char* root = os_utils_getenv("IPFS_PATH");
	if (root == NULL) {
		root = os_utils_getenv("HOME");
		result = malloc( strlen(root) + 7);
		if (result == NULL)
			return 0;
		strncpy(result, root, strlen(root)+1);
		strncat(result, "/.ipfs", 7);
	} else {
		char* result = malloc(strlen(root)+1);
		if (result == NULL)
			return 0;
		strncpy(result, root, strlen(root)+1);
	}
	return 1;
}

/***
 * Returns the path "extension" relative to the configuration root.
 * If an empty string is provided for config_root, the default root
 * is used. NOTE: be sure to dispose of the memory allocated for result.
 * @param config_root the path to the root of the configuration
 * @param extension the extension to add to the path
 * @param result the result of config_root with extension appended
 * @returns true(1) if everything went okay, false(0) otherwise
 */
int config_path(char* config_root, char* extension, char* result, int max_len) {
	if (strlen(config_root) == 0) {
		char* default_path;
		int retVal = config_get_default_path_root(default_path);
		if (!retVal)
			return retVal;
		retVal = os_utils_filepath_join(default_path, extension, result, max_len);
		free(default_path);
		return retVal;
	}
	return os_utils_filepath_join(config_root, extension, result, max_len);
}

/**
 * provide the full path of the config file, given the directory.
 * NOTE: This allocates memory for result. Make sure to clean up after yourself.
 * @param path the path to the config file (without the actual file name)
 * @param result the full filename including the path
 * @returns true(1) on success, false(0) otherwise
 */
int config_get_file_name(char* path, char* result) {
	return 0;
}

/***
 * create a configuration based on the passed in parameters
 * @param config the configuration struct to be filled in
 * @param num_bits_for_keypair number of bits for the key pair
 * @returns true(1) on success, otherwise 0
 */
int repo_config_init(struct RepoConfig* config, unsigned int num_bits_for_keypair, char* repo_path) {
	
	// identity
	int retVal = repo_config_identity_new(&(config->identity), num_bits_for_keypair);
	if (retVal == 0)
		return 0;
	
	// bootstrap peers
	retVal = repo_config_bootstrap_peers_retrieve(&(config->peer_addresses));
	if (retVal == 0)
		return 0;
	
	// datastore
	retVal = repo_config_datastore_init(&(config->datastore), repo_path);
	if (retVal == 0)
		return 0;

	retVal = repo_config_addresses_init(&(config->addresses), "/ip4/127.0.0.1/tcp/5001", "/ip4/127.0.0.1/tcp/8080");
	if (retVal == 0)
		return 0;
	
	// swarm addresses
	char** address_array = (char * []){ "/ip4/0.0.0.0/tcp/4001", "/ip6/::/tcp/4001" };
	retVal = repo_config_swarm_address_init(&(config->addresses.swarm), address_array, 2);
	if (retVal == 0)
		return 0;
	
	config->discovery.mdns.enabled = 1;
	config->discovery.mdns.interval = 10;
	
	config->mounts.ipfs = "/ipfs";
	config->mounts.ipns = "/ipns";
	
	config->ipns.resolve_cache_size = 128;
	
	config->reprovider.interval = "12h";
	
	config->gateway.root_redirect = "";
	config->gateway.writable = 0;
	
	config->gateway.path_prefixes.num_elements = 0;

	// gateway http headers
	char** header_array = (char * []) { "Access-Control-Allow-Origin", "Access-Control-Allow-Methods", "Access-Control-Allow-Headers" };
	char** header_values = (char*[])  { "*", "GET", "X-Requested-With" };
	retVal = repo_config_gateway_http_header_init(&(config->gateway.http_headers), header_array, header_values, 3);
	if (retVal == 0)
		return 0;
	
	return 1;
}


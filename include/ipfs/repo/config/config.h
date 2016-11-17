#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "datastore.h"
#include "identity.h"
#include "swarm.h"
#include "bootstrap_peers.h"
#include "addresses.h"
#include "gateway.h"

struct MDNS {
	int enabled;
	int interval;
};

struct Discovery {
	struct MDNS mdns;
};

struct Mounts {
	char* ipfs;
	char* ipns;
};

struct Ipns {
	int resolve_cache_size;
};

struct Reprovider {
	char* interval;
};

struct RepoConfig {
	struct Identity identity;
	struct Datastore datastore;
	struct Addresses addresses;
	struct Mounts mounts;
	struct Discovery discovery;
	struct Ipns ipns;
	struct BootstrapPeers peer_addresses;
	//struct tour tour;
	struct Gateway gateway;
	//struct supernode_routing supernode_client_config;
	//struct api api;
	struct Reprovider reprovider;
};

/**
 * provide the full path of the config file, given the directory. 
 * NOTE: This allocates memory for result. Make sure to clean up after yourself.
 * @param path the path to the config file (without the actual file name)
 * @param result the full filename including the path
 * @returns true(1) on success, false(0) otherwise
 */
int repo_config_get_file_name(char* path, char** result);

/***
 * Returns the path "extension" relative to the configuration root.
 * If an empty string is provided for config_root, the default root
 * is used.
 * @param config_root the path to the root of the configuration
 * @param extension the extension to add to the path
 * @param result the result of config_root with extension appended
 * @param max_len the max length of the result
 * @returns true(1) if everything went okay, false(0) otherwise
 */
int config_path(char* config_root, char* extension, char* result, int max_len);

/***
 * create a configuration based on the passed in parameters
 * @param config the configuration struct
 * @param num_bits_for_keypair number of bits for the key pair
 * @returns true(1) on success, otherwise 0
 */
int repo_config_init(struct RepoConfig* config, unsigned int num_bits_for_keypair, char* repo_path);

/***
 * Initialize memory for a RepoConfig struct
 * @param config the structure to initialize
 * @returns true(1) on success
 */
int ipfs_repo_config_new(struct RepoConfig** config);

/***
 * free all resources that were allocated to store config information
 * @param config the config
 * @returns true(1)
 */
int ipfs_repo_config_free(struct RepoConfig* config);

#endif

//
//  fs_repo.c
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include "ipfs/repo/fsrepo/fs_repo.h"
#include "ipfs/os/utils.h"
/** 
 * private methods
 */

/**
 * writes the config file
 * @param full_filename the full filename of the config file in the OS
 * @param config the details to put into the file
 * @returns true(1) on success, else false(0)
 */
int repo_config_write_config_file(char* full_filename, struct RepoConfig* config) {
	FILE* out_file = fopen(full_filename, "w");
	if (out_file == NULL)
		return 0;
	
	fprintf(out_file, "{\n");
	fprintf(out_file, " \"Identity\": {\n");
	fprintf(out_file, "  \"PeerID\": \"%s\",\n", config->identity.peer_id);
	// TODO: print correct format of private key
	//fprintf(out_file, "  \"PrivKey\": \"%s\"\n", config->identity.private_key.base64);
	fprintf(out_file, " },\n");
	fprintf(out_file, " \"Datastore\": {\n");
	fprintf(out_file, "  \"Type\": \"%s\",\n", config->datastore.type);
	fprintf(out_file, "  \"Path\": \"%s\",\n", config->datastore.path);
	fprintf(out_file, "  \"StorageMax\": \"%s\",\n", config->datastore.storage_max);
	fprintf(out_file, "  \"StorageGCWatermark\": %d,\n", config->datastore.storage_gc_watermark);
	fprintf(out_file, "  \"GCPeriod\": \"%s\",\n", config->datastore.gc_period);
	fprintf(out_file, "  \"Params\": null,\n");
	fprintf(out_file, "  \"NoSync\": %s,\n", config->datastore.no_sync ? "true" : "false");
	fprintf(out_file, "  \"HashOnRead\": %s,\n", config->datastore.hash_on_read ? "true" : "false");
	fprintf(out_file, "  \"BloomFilterSize\": %d\n", config->datastore.bloom_filter_size);
	fprintf(out_file, " },\n \"Addresses\": {\n");
	fprintf(out_file, "  \"Swarm\": [\n");
	for(int i = 0; i < config->addresses.swarm.num_addresses; i++) {
		fprintf(out_file, "   \"%s\"", config->addresses.swarm.addresses[i]);
		if (i != (config->addresses.swarm.num_addresses - 1))
			fprintf(out_file, ",\n");
		else
			fprintf(out_file, "\n");
	}
	fprintf(out_file, "  ],\n");
	fprintf(out_file, "  \"API\": \"%s\",\n", config->addresses.api);
	fprintf(out_file, "  \"Gateway\": \"%s\"\n", config->addresses.gateway);
	fprintf(out_file, " }\n  \"Mounts\": {\n");
	fprintf(out_file, "  \"IPFS\": \"%s\",\n", config->mounts.ipfs);
	fprintf(out_file, "  \"IPNS\": \"%s\",\n", config->mounts.ipns);
	fprintf(out_file, "  \"FuseAllowOther\": %s\n", "false");
	fprintf(out_file, " },\n  \"Discovery\": {\n   \"MDNS\": {\n");
	fprintf(out_file, "   \"Enabled\": %s,\n", config->discovery.mdns.enabled ? "true" : "false");
	fprintf(out_file, "   \"Interval\": %d\n  }\n },\n", config->discovery.mdns.interval);
	fprintf(out_file, " \"Ipns\": {\n");
	fprintf(out_file, "  \"RepublishedPeriod\": \"\",\n");
	fprintf(out_file, "  \"RecordLifetime\": \"\",\n");
	fprintf(out_file, "  \"ResolveCacheSize\": %d\n", config->ipns.resolve_cache_size);
	fprintf(out_file, " },\n \"Bootstrap\": [\n");
	for(int i = 0; i < config->peer_addresses.num_peers; i++) {
		fprintf(out_file, "  \"%s\"", config->peer_addresses.peers[i]->entire_string);
		if (i < config->peer_addresses.num_peers - 1)
			fprintf(out_file, ",\n");
		else
			fprintf(out_file, "\n");
	}
	fprintf(out_file, " ],\n \"Tour\": {\n  \"Last\": \"\"\n },\n");
	fprintf(out_file, " \"Gateway\": {\n");
	fprintf(out_file, "  \"HTTPHeaders\": {\n");
	for (int i = 0; i < config->gateway.http_headers.num_elements; i++) {
		fprintf(out_file, "   \"%s\": [\n    \"%s\"\n  ]", config->gateway.http_headers.headers[i]->header, config->gateway.http_headers.headers[i]->value);
		if (i < config->gateway.http_headers.num_elements - 1)
			fprintf(out_file, ",\n");
		else
			fprintf(out_file, "\n");
	}
	fprintf(out_file, "  \"RootRedirect\": \"%s\"\n", config->gateway.root_redirect);
	fprintf(out_file, "  \"Writable\": %s\n", config->gateway.writable ? "true" : "false");
	fprintf(out_file, "  \"PathPrefixes\": []\n");
	fprintf(out_file, " },\n \"SupernodeRouting\": {\n");
	fprintf(out_file, "  \"Servers\": null\n },");
	fprintf(out_file, " \"API\": {\n  \"HTTPHeaders\": null\n },\n");
	fprintf(out_file, " \"Swarm\": {\n  \"AddrFilters\": null\n }\n}");
	fclose(out_file);
	return 1;
}

/**
 * constructs the FSRepo struct. Basically fills in the FSRepo.path
 * Remember: the path must be freed
 * @param repo_path the path to the repo
 * @param repo the struct to fill in
 * @returns false(0) if something bad happened, otherwise true(1)
 */
int repo_new_fs_repo(char* repo_path, struct FSRepo* repo) {
	// get the user's home directory
	char* home_dir = os_utils_get_homedir();
	unsigned long newPathLen = strlen(home_dir) + strlen(repo_path) + 2;  // 1 for slash and 1 for end
	char* newPath = malloc(sizeof(char) * newPathLen);
	os_utils_filepath_join(os_utils_get_homedir(), repo_path, newPath, newPathLen);
	repo->path = newPath;
	return 1;
}

/**
 * checks to see if the repo is initialized at the given path
 * @param full_path the path to the repo
 * @returns true(1) if the config file is there, false(0) otherwise
 */
int repo_config_is_initialized(char* full_path) {
	char* config_file_full_path;
	int retVal = repo_config_get_file_name(full_path, &config_file_full_path);
	if (!retVal)
		return 0;
	
	if (os_utils_file_exists(config_file_full_path))
		retVal = 1;
	else
		retVal = 0;
	
	free(config_file_full_path);
	return retVal;
}

/***
 * Check to see if the repo is initialized
 * @param full_path the path to the repo
 * @returns true(1) if it is initialized, false(0) otherwise.
 */
int fs_repo_is_initialized_unsynced(char* full_path) {
	return repo_config_is_initialized(full_path);
}

/**
 * checks to see if the repo is initialized
 * @param full_path the full path to the repo
 * @returns true(1) if it is initialized, otherwise false(0)
 */
int repo_check_initialized(char* full_path) {
	// note the old version of this reported an error if the repo was a .go-ipfs repo (by looking at the path)
	// this version skips that step
	return fs_repo_is_initialized_unsynced(full_path);
}

/***
 * opens the datastore and puts it in the FSRepo struct
 * @param repo the FSRepo struct
 * @returns 0 on failure, otherwise 1
 */
int fs_repo_open_config(struct FSRepo* repo) {
	//TODO: open config file
	//TODO: read into the FSRepo struct
	return 0;
}

/***
 * opens the repo's datastore, and puts a reference to it in the FSRepo struct
 * @param repo the FSRepo struct
 * @returns 0 on failure, otherwise 1
 */
int fs_repo_open_datastore(struct FSRepo* repo) {
	return 0;
}

/**
 * public methods
 */

/**
 * opens a fsrepo
 * @param repo_path the path to the repo
 * @param repo where to store the repo info
 * @return 0 if there was a problem, otherwise 1
 */
int fs_repo_open(char* repo_path, struct FSRepo* repo) {
	//TODO: lock
	// get the path set in the repo struct
	int retVal = repo_new_fs_repo(repo_path, repo);
	if (retVal == 0)
		return 0;
	
	// check if initialized
	if (!repo_check_initialized(repo->path))
		return 0;
	//TODO: lock the file (remember to unlock)
	//TODO: check the version, and make sure it is correct
	//TODO: make sure the directory is writable
	//TODO: open the config
	fs_repo_open_config(repo);
	//TODO: open the datastore. Note: the config file has the datastore type
	fs_repo_open_datastore(repo);
	
	return 0;
}

/***
 * checks to see if the repo is initialized
 * @param repo_path the path to the repo
 * @returns true(1) if it is initialized, otherwise false(0)
 */
int fs_repo_is_initialized(char* repo_path) {
	//TODO: lock things up so that someone doesn't try an init or remove while this call is in progress
	// don't forget to unlock
	return fs_repo_is_initialized_unsynced(repo_path);
}

int fs_repo_init(char* path, struct RepoConfig* config) {
	// TODO: Do a lock so 2 don't do this at the same time
	
	// return error if this has already been done
	if (fs_repo_is_initialized_unsynced(path))
		return 0;
	
	int retVal = fs_repo_write_config_file(path, config);
	if (retVal == 0)
		return 0;
	
	// TODO: Implement this method
	//retVal = fs_repo_defaultds_init(path, config);
	if (retVal == 0)
		return 0;
	
	// write the version to a file for migrations (see repo/fsrepo/migrations/mfsr.go)
	//TODO: mfsr.RepoPath(repo_path).WriteVersion(RepoVersion)
	return 1;
}

/**
 * write the config file to disk
 * @param path the path to the file
 * @param config the config structure
 * @returns true(1) on success
 */
int fs_repo_write_config_file(char* path, struct RepoConfig* config) {
	if (fs_repo_is_initialized(path))
		return 0;
	
	char* buff = NULL;
	if (!repo_config_get_file_name(path, &buff))
		return 0;
	
	int retVal = repo_config_write_config_file(buff, config);
	
	free(buff);
	
	return retVal;
}


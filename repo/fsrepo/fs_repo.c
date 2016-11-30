//
//  fs_repo.c
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include "libp2p/crypto/encoding/base64.h"

#include "ipfs/repo/config/datastore.h"
#include "ipfs/repo/fsrepo/fs_repo.h"
#include "ipfs/os/utils.h"
#include "ipfs/repo/fsrepo/lmdb_datastore.h"
#include "jsmn.h"

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
	fprintf(out_file, "  \"PeerID\": \"%s\",\n", config->identity->peer_id);
	// print correct format of private key
	// first base 64 it
	size_t encoded_size = libp2p_crypto_encoding_base64_encode_size(config->identity->private_key.der_length);
	unsigned char encoded_buffer[encoded_size + 1];
	int retVal = libp2p_crypto_encoding_base64_encode(config->identity->private_key.der, config->identity->private_key.der_length, encoded_buffer, encoded_size, &encoded_size);
	if (retVal == 0)
		return 0;
	encoded_buffer[encoded_size] = 0;
	fprintf(out_file, "  \"PrivKey\": \"%s\"\n", encoded_buffer);
	fprintf(out_file, " },\n");
	fprintf(out_file, " \"Datastore\": {\n");
	fprintf(out_file, "  \"Type\": \"%s\",\n", config->datastore->type);
	fprintf(out_file, "  \"Path\": \"%s\",\n", config->datastore->path);
	fprintf(out_file, "  \"StorageMax\": \"%s\",\n", config->datastore->storage_max);
	fprintf(out_file, "  \"StorageGCWatermark\": %d,\n", config->datastore->storage_gc_watermark);
	fprintf(out_file, "  \"GCPeriod\": \"%s\",\n", config->datastore->gc_period);
	fprintf(out_file, "  \"Params\": null,\n");
	fprintf(out_file, "  \"NoSync\": %s,\n", config->datastore->no_sync ? "true" : "false");
	fprintf(out_file, "  \"HashOnRead\": %s,\n", config->datastore->hash_on_read ? "true" : "false");
	fprintf(out_file, "  \"BloomFilterSize\": %d\n", config->datastore->bloom_filter_size);
	fprintf(out_file, " },\n \"Addresses\": {\n");
	fprintf(out_file, "  \"Swarm\": [\n");
	for(int i = 0; i < config->addresses->swarm->num_addresses; i++) {
		fprintf(out_file, "   \"%s\"", config->addresses->swarm->addresses[i]);
		if (i != (config->addresses->swarm->num_addresses - 1))
			fprintf(out_file, ",\n");
		else
			fprintf(out_file, "\n");
	}
	fprintf(out_file, "  ],\n");
	fprintf(out_file, "  \"API\": \"%s\",\n", config->addresses->api);
	fprintf(out_file, "  \"Gateway\": \"%s\"\n", config->addresses->gateway);
	fprintf(out_file, " },\n  \"Mounts\": {\n");
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
	for (int i = 0; i < config->gateway->http_headers->num_elements; i++) {
		fprintf(out_file, "   \"%s\": [\n    \"%s\"\n  ]", config->gateway->http_headers->headers[i]->header, config->gateway->http_headers->headers[i]->value);
		if (i < config->gateway->http_headers->num_elements - 1)
			fprintf(out_file, ",\n");
		else
			fprintf(out_file, "\n },\n");
	}
	fprintf(out_file, "  \"RootRedirect\": \"%s\"\n", config->gateway->root_redirect);
	fprintf(out_file, "  \"Writable\": %s\n", config->gateway->writable ? "true" : "false");
	fprintf(out_file, "  \"PathPrefixes\": []\n");
	fprintf(out_file, " },\n \"SupernodeRouting\": {\n");
	fprintf(out_file, "  \"Servers\": null\n },");
	fprintf(out_file, " \"API\": {\n  \"HTTPHeaders\": null\n },\n");
	fprintf(out_file, " \"Swarm\": {\n  \"AddrFilters\": null\n }\n}");
	fclose(out_file);
	return 1;
}

/**
 * constructs the FSRepo struct.
 * Remember: ipfs_repo_fsrepo_free must be called
 * @param repo_path the path to the repo
 * @param config the optional config file. NOTE: if passed, fsrepo_free will free resources of the RepoConfig.
 * @param repo the struct to allocate memory for
 * @returns false(0) if something bad happened, otherwise true(1)
 */
int ipfs_repo_fsrepo_new(char* repo_path, struct RepoConfig* config, struct FSRepo** repo) {
	*repo = (struct FSRepo*)malloc(sizeof(struct FSRepo));

	if (repo_path == NULL) {
		// get the user's home directory
		char* home_dir = os_utils_get_homedir();
		char* default_subdir = "/.ipfs";
		unsigned long newPathLen = strlen(home_dir) + strlen(default_subdir) + 2;  // 1 for slash and 1 for end
		(*repo)->path = malloc(sizeof(char) * newPathLen);
		if ((*repo)->path == NULL) {
			free( (*repo));
			return 0;
		}
		os_utils_filepath_join(os_utils_get_homedir(), default_subdir, (*repo)->path, newPathLen);
	} else {
		int len = strlen(repo_path) + 1;
		(*repo)->path = (char*)malloc(len);
		strncpy((*repo)->path, repo_path, len);
	}
	// allocate other structures
	if (config != NULL)
		(*repo)->config = config;
	else {
		if (ipfs_repo_config_new(&((*repo)->config)) == 0) {
			free(repo_path);
			return 0;
		}
	}
	return 1;
}

/**
 * Cleans up memory
 * @param repo the struct to clean up
 * @returns true(1) on success
 */
int ipfs_repo_fsrepo_free(struct FSRepo* repo) {
	if (repo != NULL) {
		if (repo->path != NULL)
			free(repo->path);
		if (repo->config != NULL)
			ipfs_repo_config_free(repo->config);
		free(repo);
	}
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
 * Reads the file, placing its contents in buffer
 * NOTE: this allocates memory for buffer, and should be freed
 * @param path the path to the config file
 * @param buffer where to put the contents
 * @returns true(1) on success
 */
int _read_file(const char* path, char** buffer) {
	int file_size = os_utils_file_size(path);
	if (file_size <= 0)
		return 0;
	// allocate memory
	*buffer = malloc(file_size + 1);
	if (*buffer == NULL) {
		return 0;
	}
	memset(*buffer, 0, file_size + 1);

	// open file
	FILE* in_file = fopen(path, "r");
	// read data
	fread(*buffer, file_size, 1, in_file);

	// cleanup
	fclose(in_file);
	return 1;
}

/**
 * Find the position of a key
 * @param data the string that contains the json
 * @param tokens the tokens of the parsed string
 * @param tok_length the number of tokens there are
 * @param tag what we're looking for
 * @returns the position of the requested token in the array, or -1
 */
int _find_token(const char* data, const jsmntok_t* tokens, int tok_length, int start_from, const char* tag) {
	for(int i = start_from; i < tok_length; i++) {
		jsmntok_t curr_token = tokens[i];
		if ( curr_token.type == JSMN_STRING) {
			// convert to string
			int str_len = curr_token.end - curr_token.start;
			char str[str_len + 1];
			strncpy(str, &data[curr_token.start], str_len );
			str[str_len] = 0;
			if (strcmp(str, tag) == 0)
				return i;
		}
	}
	return -1;
}

/**
 * Retrieves the value of a key / value pair from the JSON data
 * @param data the full JSON string
 * @param tokens the array of tokens
 * @param tok_length the number of tokens
 * @param search_from start search from this token onward
 * @param tag what to search for
 * @param result where to put the result. NOTE: allocates memory that must be freed
 * @returns true(1) on success
 */
int _get_json_string_value(char* data, const jsmntok_t* tokens, int tok_length, int search_from, const char* tag, char** result) {
	int pos = _find_token(data, tokens, tok_length, search_from, tag);
	if (pos < 0)
		return 0;
	jsmntok_t curr_token = tokens[pos+1];
	if (curr_token.type == JSMN_PRIMITIVE) {
		// a null
		*result = NULL;
	}
	if (curr_token.type != JSMN_STRING)
		return 0;
	// allocate memory
	int str_len = curr_token.end - curr_token.start;
	*result = malloc(sizeof(char) * str_len + 1);
	// copy in the string
	strncpy(*result, &data[curr_token.start], str_len);
	(*result)[str_len] = 0;
	return 1;
}

/**
 * Retrieves the value of a key / value pair from the JSON data
 * @param data the full JSON string
 * @param tokens the array of tokens
 * @param tok_length the number of tokens
 * @param search_from start search from this token onward
 * @param tag what to search for
 * @param result where to put the result
 * @returns true(1) on success
 */
int _get_json_int_value(char* data, const jsmntok_t* tokens, int tok_length, int search_from, const char* tag, int* result) {
	int pos = _find_token(data, tokens, tok_length, search_from, tag);
	if (pos < 0)
		return 0;
	jsmntok_t curr_token = tokens[pos+1];
	if (curr_token.type != JSMN_PRIMITIVE)
		return 0;
	// allocate memory
	int str_len = curr_token.end - curr_token.start;
	char str[str_len + 1];
	// copy in the string
	strncpy(str, &data[curr_token.start], str_len);
	str[str_len] = 0;
	if (strcmp(str, "true") == 0)
		*result = 1;
	else if (strcmp(str, "false") == 0)
		*result = 0;
	else if (strcmp(str, "null") == 0) // what should we do here?
		*result = 0;
	else // its a real number
		*result = atoi(str);
	return 1;
}

/***
 * Opens the config file and puts the data into the FSRepo struct
 * @param repo the FSRepo struct
 * @returns 0 on failure, otherwise 1
 */
int fs_repo_open_config(struct FSRepo* repo) {
	int retVal;
	char* data;
	size_t full_filename_length = strlen(repo->path) + 8;
	char full_filename[full_filename_length];
	retVal = os_utils_filepath_join(repo->path, "config", full_filename, full_filename_length);
	if (retVal == 0)
		return 0;
	retVal = _read_file(full_filename, &data);
	// parse the data
	jsmn_parser parser;
	jsmn_init(&parser);
	int num_tokens = 256;
	jsmntok_t tokens[num_tokens];
	num_tokens = jsmn_parse(&parser, data, strlen(data), tokens, 256);
	if (num_tokens <= 0) {
		free(data);
		return 0;
	}
	// fill FSRepo struct
	// allocation done by fsrepo_new... repo->config = malloc(sizeof(struct RepoConfig));
	// Identity
	int curr_pos = _find_token(data, tokens, num_tokens, 0, "Identity");
	if (curr_pos < 0) {
		free(data);
		return 0;
	}
	// the next should be the array, then string "PeerID"
	_get_json_string_value(data, tokens, num_tokens, curr_pos, "PeerID", &repo->config->identity->peer_id);
	char* priv_key_base64;
	// then PrivKey
	_get_json_string_value(data, tokens, num_tokens, curr_pos, "PrivKey", &priv_key_base64);
	retVal = repo_config_identity_build_private_key(repo->config->identity, priv_key_base64);
	if (retVal == 0) {
		free(data);
		free(priv_key_base64);
		return 0;
	}
	// now the datastore
	int datastore_position = _find_token(data, tokens, num_tokens, 0, "Datastore");
	_get_json_string_value(data, tokens, num_tokens, curr_pos, "Type", &repo->config->datastore->type);
	_get_json_string_value(data, tokens, num_tokens, curr_pos, "Path", &repo->config->datastore->path);
	_get_json_string_value(data, tokens, num_tokens, curr_pos, "StorageMax", &repo->config->datastore->storage_max);
	_get_json_int_value(data, tokens, num_tokens, curr_pos, "StorageGCWatermark", &repo->config->datastore->storage_gc_watermark);
	_get_json_string_value(data, tokens, num_tokens, curr_pos, "GCPeriod", &repo->config->datastore->gc_period);
	_get_json_string_value(data, tokens, num_tokens, curr_pos, "Params", &repo->config->datastore->params);
	_get_json_int_value(data, tokens, num_tokens, curr_pos, "NoSync", &repo->config->datastore->no_sync);
	_get_json_int_value(data, tokens, num_tokens, curr_pos, "HashOnRead", &repo->config->datastore->hash_on_read);
	_get_json_int_value(data, tokens, num_tokens, curr_pos, "BloomFilterSize", &repo->config->datastore->bloom_filter_size);

	// free the memory used reading the json file
	free(data);
	free(priv_key_base64);
	return 1;
}

/***
 * set function pointers in the datastore struct to lmdb
 * @param repo contains the information
 * @returns true(1) on success
 */
int fs_repo_setup_lmdb_datastore(struct FSRepo* repo) {
	return repo_fsrepo_lmdb_cast(repo->config->datastore);
}

/***
 * opens the repo's datastore, and puts a reference to it in the FSRepo struct
 * @param repo the FSRepo struct
 * @returns 0 on failure, otherwise 1
 */
int fs_repo_open_datastore(struct FSRepo* repo) {
	int argc = 0;
	char** argv = NULL;

	if (strncmp(repo->config->datastore->type, "lmdb", 4) == 0) {
		// this is a LightningDB. Open it.
		int retVal = fs_repo_setup_lmdb_datastore(repo);
		if (retVal == 0)
			return 0;
	} else {
		// add new datastore types here
		return 0;
	}

	int retVal = repo->config->datastore->datastore_open(argc, argv, repo->config->datastore);

	// do specific datastore cleanup here if needed

	return retVal;
}

/**
 * public methods
 */

/**
 * opens a fsrepo
 * @param repo the repo struct. Should contain the path. This method will do the rest
 * @return 0 if there was a problem, otherwise 1
 */
int ipfs_repo_fsrepo_open(struct FSRepo* repo) {
	//TODO: lock
	// check if initialized
	if (!repo_check_initialized(repo->path)) {
		return 0;
	}
	//TODO: lock the file (remember to unlock)
	//TODO: check the version, and make sure it is correct
	//TODO: make sure the directory is writable
	//TODO: open the config
	if (!fs_repo_open_config(repo)) {
		return 0;
	}

	// open the datastore
	if (!fs_repo_open_datastore(repo)) {
		return 0;
	}
	
	return 1;
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

int ipfs_repo_fsrepo_datastore_init(struct FSRepo* fs_repo) {
	// make the directory
	repo_fsrepo_lmdb_create_directory(fs_repo->config->datastore);

	// fill in the function prototypes
	repo_fsrepo_lmdb_cast(fs_repo->config->datastore);
}

/**
 * Initializes a new FSRepo at the given path with the provided config
 * @param path the path to use
 * @param config the information for the config file
 * @returns true(1) on success
 */
int ipfs_repo_fsrepo_init(struct FSRepo* repo) {
	// TODO: Do a lock so 2 don't do this at the same time
	
	// return error if this has already been done
	if (fs_repo_is_initialized_unsynced(repo->path))
		return 0;
	
	int retVal = fs_repo_write_config_file(repo->path, repo->config);
	if (retVal == 0)
		return 0;
	
	// TODO: Implement this method
	retVal = ipfs_repo_fsrepo_datastore_init(repo);
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


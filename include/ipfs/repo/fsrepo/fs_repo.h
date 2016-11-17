/***
 * handles a repo on the file system
 */
#ifndef fs_repo_h
#define fs_repo_h

#include <stdio.h>
#include "ipfs/repo/config/config.h"


/**
 * a structure to hold the repo info
 */
struct FSRepo {
	int closed;
	char* path;
	struct IOCloser* lock_file;
	struct RepoConfig* config;
	struct Datastore* data_store;
};

/**
 * opens a fsrepo
 * @param repo_path the path to the repo
 * @param repo where to store the repo info
 * @return 0 if there was a problem, otherwise 1
 */
int ipfs_repo_fsrepo_open(struct FSRepo* repo);

/***
 * checks to see if the repo is initialized
 * @param repo_path the path to the repo
 * @returns true(1) if it is initialized, otherwise false(0)
 */
int fs_repo_is_initialized(char* repo_path);

/**
 * write the config file to disk
 * @param path the path to the file
 * @param config the config structure
 * @returns true(1) on success
 */
int fs_repo_write_config_file(char* path, struct RepoConfig* config);

/**
 * Initializes a new FSRepo at the given path with the provided config
 * @param repo_path the path to use
 * @param config the information for the config file
 * @returns true(1) on success
 */
int ipfs_repo_fsrepo_new(char* repo_path, struct RepoConfig* config, struct FSRepo** fs_repo);

/***
 * Free all resources used by this struct
 * @param repo the repo to clean up
 * @returns true(1) on success
 */
int ipfs_repo_fsrepo_free(struct FSRepo* config);

/***
 * Initialize a new repo with the specified configuration
 * @param config the information in order to build the repo
 * @returns true(1) on success
 */
int ipfs_repo_fsrepo_init(struct FSRepo* config);

#endif /* fs_repo_h */

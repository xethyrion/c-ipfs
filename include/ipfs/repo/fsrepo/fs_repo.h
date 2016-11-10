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
	struct Config* config;
	struct Datastore* data_store;
};

/**
 * opens a fsrepo
 * @param repo_path the path to the repo
 * @param repo where to store the repo info
 * @return 0 if there was a problem, otherwise 1
 */
int fs_repo_open(char* repo_path, struct FSRepo* repo);

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
int fs_repo_init(char* repo_path, struct RepoConfig* config);


#endif /* fs_repo_h */

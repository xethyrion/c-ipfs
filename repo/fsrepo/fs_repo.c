//
//  fs_repo.c
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include "fs_repo.h"
#include "ipfs/repo/config/config.h"
#include "ipfs/os/utils.h"
/** 
 * private methods
 */

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
	int retVal = config_get_file_name(full_path, config_file_full_path);
	if (!retVal)
		return 0;
	if (os_utils_file_exists(config_file_full_path))
		retVal = 1;
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


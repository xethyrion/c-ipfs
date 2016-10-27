//
//  fs_repo.c
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include "fs_repo.h"
#include "../config/config.h"
#include "../../os/utils.h"
/** 
 * private methods
 */

/**
 * constructs the FSRepo struct. Basically fills in the FSRepo.path
 * @param repo_path the path to the repo
 * @param repo the struct to fill in
 * @returns false(0) if something bad happened, otherwise true(1)
 */
int repo_new_fs_repo(char* repo_path, struct FSRepo* repo) {
	//TODO: add the root to the path, based on environment variable HOMEDIR
	// but for now, just do this
	repo->path = repo_path;
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
int repo_is_initialized_unsynced(char* full_path) {
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
	return repo_is_initialized_unsynced(full_path);
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
	int retVal = repo_new_fs_repo(repo_path, repo);
	// check if initialized
	if (!repo_check_initialized(repo->path))
		return 0;
	return 0;
}


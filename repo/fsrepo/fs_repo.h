/***
 * handles a repo on the file system
 */
#ifndef fs_repo_h
#define fs_repo_h

#include <stdio.h>

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
 * @param repoPath the path to the repo
 * @param repo where to store the repo info
 * @return 0 if there was a problem, otherwise 1
 */
int fs_repo_open(char* repoPath, struct FSRepo* repo);


#endif /* fs_repo_h */

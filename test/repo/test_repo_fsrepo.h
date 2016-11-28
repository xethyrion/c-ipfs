#include "ipfs/repo/fsrepo/fs_repo.h"

int test_repo_fsrepo_open_config() {
	struct FSRepo* fs_repo = NULL;
	struct RepoConfig* repo_config = NULL;

	const char* path = "/tmp/.ipfs";

	// create the struct
	int retVal = ipfs_repo_fsrepo_new((char*)path, repo_config, &fs_repo);
	if (retVal == 0)
		return 0;

	// open the repository and read the file
	retVal = ipfs_repo_fsrepo_open(fs_repo);
	if (retVal == 0) {
		ipfs_repo_fsrepo_free(fs_repo);
		return 0;
	}

	retVal = ipfs_repo_fsrepo_free(fs_repo);
	if (retVal == 0)
		return 0;

	return 1;
}

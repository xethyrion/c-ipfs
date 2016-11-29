#include "repo/test_repo_config.h"
#include "repo/test_repo_identity.h"
#include "repo/test_repo_bootstrap_peers.h"
#include "repo/test_repo_fsrepo.h"
#include "cmd/ipfs/test_init.h"
#include "cid/test_cid.h"
#include "flatfs/test_flatfs.h"
#include "storage/test_ds_helper.h"

int testit(const char* name, int (*func)(void)) {
	printf("Testing %s...\n", name);
	int retVal = func();
	if (retVal)
		printf("%s success!\n", name);
	else
		printf("** Uh oh! %s failed.**\n", name);
	return retVal == 0;
}

int main(int argc, char** argv) {
	int counter = 0;
	counter += testit("test_cid_new_free", test_cid_new_free);
	counter += testit("test_cid_cast_multihash", test_cid_cast_multihash);
	counter += testit("test_cid_cast_non_multihash", test_cid_cast_non_multihash);
	counter += testit("test_init_new_installation", test_init_new_installation);
	counter += testit("test_repo_config_new", test_repo_config_new);
	counter += testit("test_repo_config_init", test_repo_config_init);
	counter += testit("test_repo_config_write", test_repo_config_write);
	counter += testit("test_repo_config_identity_new", test_repo_config_identity_new);
	counter += testit("test_repo_config_identity_private_key", test_repo_config_identity_private_key);
	counter += testit("test_repo_bootstrap_peers_init", test_repo_bootstrap_peers_init);
	counter += testit("get_init_command", test_get_init_command);
	counter += testit("test_fs_repo_open", test_repo_fsrepo_open_config);
	counter += testit("test_flatfs_get_directory", test_flatfs_get_directory);
	counter += testit("test_flatfs_get_filename", test_flatfs_get_filename);
	counter += testit("test_flatfs_get_full_filename", test_flatfs_get_full_filename);
	counter += testit("test_ds_key_from_binary", test_ds_key_from_binary);
	if (counter > 0) {
		printf("***** There were %d failed test(s) *****\n", counter);
	} else {
		printf("All tests passed\n");
	}
	return 1;
}

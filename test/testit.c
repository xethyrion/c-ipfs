#include "repo/test_repo_config.h"
#include "repo/test_repo_identity.h"
#include "repo/test_repo_bootstrap_peers.h"
#include "cmd/ipfs/test_init.h"
#include "cid/test_cid.h"

int testit(const char* name, int (*func)(void)) {
	printf("Testing %s...\n", name);
	int retVal = func();
	if (retVal)
		printf("%s success!\n", name);
	else
		printf("** Uh oh! %s failed.**\n", name);
	return retVal;
}

int main(int argc, char** argv) {
	testit("test_cid_new_free", test_cid_new_free);
	testit("test_cid_cast_multihash", test_cid_cast_multihash);
	testit("test_cid_cast_non_multihash", test_cid_cast_non_multihash);
	testit("test_init_new_installation", test_init_new_installation);
	testit("test_repo_config_init", test_repo_config_init);
	testit("test_repo_config_write", test_repo_config_write);
	testit("test_repo_config_identity_new", test_repo_config_identity_new);
	testit("test_repo_config_identity_private_key", test_repo_config_identity_private_key);
	testit("test_reop_bootstrap_peers_init", test_repo_bootstrap_peers_init);
	testit("get_init_command", test_get_init_command);
	
	return 1;
}

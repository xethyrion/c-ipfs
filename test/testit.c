#include "testit.h"
#include "repo/test_repo.h"
#include "cmd/ipfs/test_init.h"

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
	testit("config_repo", test_config_repo);
	testit("get_init_command", test_get_init_command);
	return 1;
}

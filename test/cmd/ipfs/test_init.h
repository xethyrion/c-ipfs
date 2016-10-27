/**
 * Testing of cmd/ipfs/init
 */
#ifndef __TEST_INIT_H__
#define __TEST_INIT_H__

#include "../../../cmd/ipfs/init.h"
#include "../../../commands/argument.h"

#include <stdio.h>
#include <string.h>

int test_get_init_command() {
	struct Command cmd = { 0 };
	int retVal = 1;
	// make sure its empty
	if (cmd.help_text.tagline != NULL) {
		fprintf(stderr, "short description should be null\n");
		return 0;
	}
	// grab the stuff
	retVal = get_init_command(&cmd);

	if (!retVal) {
		fprintf(stderr, "Function call to get_init_command not successful. Return was %d\n", retVal);
		return retVal;
	}
	// make sure its right
	if (cmd.help_text.tagline == NULL) {
		fprintf(stderr, "short description is null\n");
		retVal = 0;
	} else if (strcmp(cmd.help_text.tagline, "Initializes IPFS config file.") != 0) {
		fprintf(stderr, "short description is not null\n");
		retVal = 0;
	} else if (cmd.argument_count != 1) {
		fprintf(stderr, "argument count should be 1");
		retVal = 0;
	} else {
		struct Argument arg1 = *(cmd.arguments[0]);
		if (strncmp(arg1.name, "default-config", 14) != 0) {
			fprintf(stderr, "arg1 wrong name. Expected %s but got %s\n", "default_config", arg1.name);
			retVal = 0;
		}
	}
	uninit_command(&cmd);
	return retVal;
}

#endif


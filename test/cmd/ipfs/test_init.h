/**
 * Testing of cmd/ipfs/init
 */
#ifndef __TEST_INIT_H__
#define __TEST_INIT_H__

#include "../../../cmd/ipfs/init.h"

#include <stdio.h>
#include <string.h>

int test_get_init_command() {
	struct command cmd = { 0 };

	// make sure its empty
	if (cmd.help_text.tagline != NULL) {
		fprintf(stderr, "short description should be null\n");
		return 0;
	}
	// grab the stuff
	get_init_command(&cmd);

	// make sure its right
	if (cmd.help_text.tagline == NULL) {
		fprintf(stderr, "short description is null\n");
		return 0;
	}
	if (strcmp(cmd.help_text.tagline, "Initializes IPFS config file.") != 0) {
		fprintf(stderr, "short description is not null\n");
		return 0;
	}
	return 1;
}

#endif


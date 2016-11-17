/**
 * Testing of cmd/ipfs/init
 */
#ifndef __TEST_INIT_H__
#define __TEST_INIT_H__

#include "ipfs/cmd/ipfs/init.h"
#include "ipfs/commands/argument.h"
#include "ipfs/commands/request.h"
#include "ipfs/commands/command.h"

#include <stdio.h>
//#include <string.h>

int test_init_new_installation() {
	unlink("/tmp/.ipfs/config");
	// do the minimum to get the .ipfs directory structure and config file built
	struct Request request;
	int retVal = ipfs_cmd_ipfs_init_command_new( &request.cmd );
	if (retVal == 0)
		return 0;

	// build a request so it builds the repository in the /tmp directory
	request.invoc_context = (struct Context*)malloc(sizeof(struct Context));
	request.invoc_context->config_root = "/tmp/.ipfs";

	// run the methods

	retVal = request.cmd.pre_run(&request);
	if (retVal == 0)
		return 0;

	retVal = request.cmd.run(&request);
	if (retVal == 0)
		return 0;

	retVal = request.cmd.post_run(&request);
	if (retVal == 0)
		return 0;

	// clean up
	ipfs_cmd_ipfs_init_command_free( &request.cmd );

	// make sure the repository exists
	retVal = os_utils_file_exists("/tmp/.ipfs/config");

	return retVal;
}

/***
 * This is used for the command line interpreter, which is still in development
 */
int test_get_init_command() {
	struct Command cmd = { 0 };
	int retVal = 1;
	// make sure its empty
	if (cmd.help_text.tagline != NULL) {
		fprintf(stderr, "short description should be null\n");
		return 0;
	}
	// grab the stuff
	retVal = ipfs_cmd_ipfs_init_command_new(&cmd);

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
	ipfs_cmd_ipfs_init_command_free(&cmd);
	return retVal;
}

#endif


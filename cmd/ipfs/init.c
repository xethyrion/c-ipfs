#include <stdlib.h>

#include "init.h"

int get_init_command(struct command* cmd) {
	// help text
	cmd->help_text.tagline = "Initializes IPFS config file.";
	cmd->help_text.short_description = "\nInitializes IPFS configuration files and generates a new keypair.\n\nipfs uses a repository in the local file system. By default, the repo is\nlocated at ~/.ipfs. To change the repo location, set the $IPFS_PATH\nenvironment variable.:\n\n    export IPFS_PATH=/path/to/ipfsrepo";

	// arguments
	// initialize an array of arguments with size of 1
	cmd->argument_count = 1;
	// allocate memory for array of pointers
	cmd->arguments = malloc(cmd->argument_count * sizeof(struct argument*));
	if (cmd->arguments == NULL)
		return 0;
	// allocate memory for argument
	cmd->arguments[0] = malloc(sizeof(struct argument));
	int retVal = init_string_argument(cmd->arguments[0], "default-config", 0, 0, "Initialize with the given configuration");
	cmd->arguments[0]->enable_stdin = 1;
	
	return retVal;
}

int uninit_command(struct command* cmd) {
	for(int i = 0; i < cmd->argument_count; i++)
		uninit_argument(cmd->arguments[i]);
	free(cmd->arguments);
	return 0;
}

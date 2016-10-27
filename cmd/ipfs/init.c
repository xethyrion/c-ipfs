#include "init.h"

int get_init_command(struct command* cmd) {
	// help text
	cmd->help_text.tagline = "Initializes IPFS config file.";
	cmd->help_text.short_description = "\nInitializes IPFS configuration files and generates a new keypair.\n\nipfs uses a repository in the local file system. By default, the repo is\nlocated at ~/.ipfs. To change the repo location, set the $IPFS_PATH\nenvironment variable.:\n\n    export IPFS_PATH=/path/to/ipfsrepo";
	// arguments
	// initialize an array of arguments with size of 1
	struct argument argument;
	int retVal = init_string_argument(&argument, "default-config", 0, 0, "Initialize with the given configuration");
	argument.enable_stdin = 1;
	struct argument* array[] = { &argument };
	cmd->arguments = array;
	cmd->argument_count = 1;
	return retVal;
}

int uninit_command(struct command* cmd) {
	for(int i = 0; i < cmd->argument_count; i++)
		uninit_argument(cmd->arguments[i]);
	return 0;
}

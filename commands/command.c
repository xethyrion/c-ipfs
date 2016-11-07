//
//  command.c
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//
#include <stdlib.h>

#include "ipfs/commands/command.h"

int init_command(struct Command* cmd) {
	// allocate memory for Argument array
	cmd->arguments = malloc(cmd->argument_count * sizeof(struct Argument*));
	if (cmd->arguments == NULL)
		return 0;
	// allocate memory for each argument
	for(int i = 0; i < cmd->argument_count; i++)
		cmd->arguments[i] = malloc(sizeof(struct Argument));
	// allocate memory for CommandOption array
	cmd->options = malloc(cmd->option_count * sizeof(struct CommandOption*));
	if (cmd->options == NULL)
		return 0;
	// allocate memory for each CommandOption
	for(int i = 0; i < cmd->option_count; i++)
		cmd->options[i] = malloc(sizeof(struct CommandOption));
	return 1;
}

int uninit_command(struct Command* cmd) {
	for(int i = 0; i < cmd->argument_count; i++)
		uninit_argument(cmd->arguments[i]);
	free(cmd->arguments);
	for(int i = 0; i < cmd->option_count; i++)
		uninit_option(cmd->options[i]);
	free(cmd->options);
	return 0;
}

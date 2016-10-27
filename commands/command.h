/***
 * The structures to commands
 */

#ifndef __COMMANDS_COMMAND_H__
#define __COMMANDS_COMMAND_H__

#include "argument.h"

struct help_text {
	char* tagline;
	char* short_description;
	char** synopsis_options_values;

	// optional
	char* usage;
	char* long_description;
	char* options;
	char* subcommands;
	char* synopsis;
};

struct command {
	//struct option* options;
	struct argument** arguments;
	//int (*pre_run)(struct request*);
	//int (*run)(struct request*);
	//int (*post_run)(struct request*);
	//struct marshaller** marshallers;
	struct help_text help_text;

	// a boolean to determine if this is an external
	// binary.
	int external;

	//struct type return_type;
	char** subcommands;
};

#endif // command.h

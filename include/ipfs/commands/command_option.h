//
//  option.h
//  c-ipfs
//
//  Created by John Jones on 10/26/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef __COMMANDS_COMMAND_OPTION_H__
#define __COMMANDS_COMMAND_OPTION_H__

#include <stdint.h>

enum Kind { invalid, boolean, integer, unsignedInt, decimal, str };

struct CommandOption {
	char** names;
	int name_count;
	enum Kind kind;
	char* description;
	int default_int_val;
	int default_bool_val;
	uint32_t default_uint_val;
	float default_float_val;
	char* default_string_val;
};

/***
 * Allocate the resources needed for a command option
 * @param option the CommandOption to initialize
 * @param description a description of this CommandOption
 * @returns true(1) on success
 */
int commands_command_option_init(struct CommandOption* option, char* description);

/***
 * Cleans up the resources of a CommandOption
 * @param option the CommandOption to clean up
 * @returns true(1)
 */
int commands_command_option_free(struct CommandOption* option);

#endif /* option_h */

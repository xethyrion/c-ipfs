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

// constructors
int init_command_option(struct CommandOption* option, char* description);
// destructors
int uninit_option(struct CommandOption* option);

#endif /* option_h */

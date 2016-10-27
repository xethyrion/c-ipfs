//
//  option.c
//  c-ipfs
//
//  Created by John Jones on 10/26/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "command_option.h"

int init_command_option(struct CommandOption* option, char* description) {
	option->description = description;
	// allocate memory for names
	option->names = malloc(option->name_count * sizeof(char*));
	if (option->names == NULL)
		return 0;
	return 1;
}

int uninit_option(struct CommandOption* option) {
	free(option->names);
	return 0;
}

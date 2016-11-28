#include <stdlib.h>
#include <string.h>

#include "ipfs/commands/argument.h"

int commands_argument_free(struct Argument* argument) {
	free(argument);
	return 1;
}

int commands_argument_init(struct Argument* argument, char* name, int required, int variadic, char* description) {
	argument->name = name;
	argument->required = required;
	argument->variadic = variadic;
	argument->description = description;
	return 1;
}

int commands_argument_string_init(struct Argument* argument, char* name, int required, int variadic, char* description) {
	int retVal = commands_argument_init(argument, name, required, variadic, description);
	if (retVal)
		argument->type = string;
	return retVal;
}

int commands_argument_file_init(struct Argument* argument, char* name, int required, int variadic, char* description) {
	int retVal = commands_argument_init(argument, name, required, variadic, description);
	if (retVal)
		argument->type = file;
	return retVal;
}

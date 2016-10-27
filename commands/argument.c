#include <stdlib.h>
#include <string.h>

#include "argument.h"

int uninit_argument(struct argument* argument) {
	return 1;
}

int init_argument(struct argument* argument, char* name, int required, int variadic, char* description) {
	argument->name = name;
	argument->required = required;
	argument->variadic = variadic;
	argument->description = description;
	return 1;
}

int init_string_argument(struct argument* argument, char* name, int required, int variadic, char* description) {
	int retVal = init_argument(argument, name, required, variadic, description);
	if (retVal)
		argument->type = string;
	return retVal;
}

int init_file_argument(struct argument* argument, char* name, int required, int variadic, char* description) {
	int retVal = init_argument(argument, name, required, variadic, description);
	if (retVal)
		argument->type = file;
	return retVal;
}

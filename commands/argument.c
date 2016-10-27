#include "argument.h"

int init_argument(struct argument* argument, char* name, int required, int variadic, char* description) {
	argument->name = name;
	argument->required = required;
	argument->variadic = variadic;
	argument->description = description;
	return 0;
}

int init_string_argument(struct argument* argument, char* name, int required, int variadic, char* description) {
	init_argument(argument, name, required, variadic, description);
	argument->type = string;
	return 0;
}

int init_file_argument(struct argument* argument, char* name, int required, int variadic, char* description) {
	init_argument(argument, name, required, variadic, description);
	argument->type = file;
	return 0;
}

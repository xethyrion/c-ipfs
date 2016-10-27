#ifndef __COMMANDS_ARGUMENT_H__
#define __COMMANDS_ARGUMENT_H__

enum ArgumentType { string, file };

struct Argument {
	char* name;
	enum ArgumentType type;
	int required;
	int variadic;
	int supports_stdin;
	int recursive;
	char* description;
	int enable_stdin;
};

int init_argument(struct Argument* argument, char* name, int required, int variadic, char* description);
int uninit_argument(struct Argument* argument);
int init_string_argument(struct Argument* argument, char* name, int required, int variadic, char* description);
int init_file_argument(struct Argument* argument, char* name, int required, int variadic, char* description);

#endif

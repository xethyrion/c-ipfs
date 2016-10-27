#ifndef __COMMANDS_ARGUMENT_H__
#define __COMMANDS_ARGUMENT_H__

enum argument_type { string, file };

struct argument {
	char* name;
	enum argument_type type;
	int required;
	int variadic;
	int supports_stdin;
	int recursive;
	char* description;
	int enable_stdin;
};

int init_argument(struct argument* argument, char* name, int required, int variadic, char* description);
int uninit_argument(struct argument* argument);
int init_string_argument(struct argument* argument, char* name, int required, int variadic, char* description);
int init_file_argument(struct argument* argument, char* name, int required, int variadic, char* description);

#endif

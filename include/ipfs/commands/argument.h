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

/**
 * Initialize an argument structure
 * @param argument the structure to initialize
 * @param name the name of the argument
 * @param required true(1) if the argument is required
 * @param variadic true(1) if the argument is variadic
 * @param description the description of the argument
 * @returns true(1) if all went okay
 */
int commands_argument_init(struct Argument* argument, char* name, int required, int variadic, char* description);
/***
 * Free resources caused by init of argument
 * @param argument the structure to clean up
 * @returns true(1)
 */
int commands_argument_free(struct Argument* argument);
/***
 * initializes a string type argument
 * @param argument the structure to initialize
 * @param name the name of the argument
 * @param required true(1) if the argument is required
 * @param variadic true(1) if the argument is variadic
 * @param description the description of the argument
 * @returns true(1) if all went okay
 */
int commands_argument_string_init(struct Argument* argument, char* name, int required, int variadic, char* description);
/***
 * initializes a file type argument
 * @param argument the structure to initialize
 * @param name the name of the argument
 * @param required true(1) if the argument is required
 * @param variadic true(1) if the argument is variadic
 * @param description the description of the argument
 * @returns true(1) if all went okay
 */
int commands_argument_file_init(struct Argument* argument, char* name, int required, int variadic, char* description);

#endif

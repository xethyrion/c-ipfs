/***
 * methods to parse the command line parameters
 */
#ifndef __COMMANDS_CLI_PARSE_H__
#define __COMMANDS_CLI_PARSE_H__

#include <stdio.h>

#include "ipfs/commands/command.h"

/***
 * turns parameters passed in into a Request struct
 * @param params the command line parameters
 * @param inStream a stream (for piped input)
 * @param cmd the command struct, already initialized
 * @param request the end result, something that can be passed on that actually does something
 * @returns 0 if something bad happens, otherwise 1
 */
int cli_parse(char** params, FILE* inStream, struct Command* cmd, struct Request* request);

int cli_parse_opts(char** params, struct Command* cmd, char* path, char** stringVals);

#endif /* parse_h */

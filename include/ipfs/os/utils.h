/**
 * OS specific stuff. This is going to get ugly, but at least its in one place
 */
#ifndef __OS_UTILS_H__
#define __OS_UTILS_H__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * get an environment varible from the os
 * @param variable the variable to look for
 * @returns the results
 */
char* os_utils_getenv(char* variable);

/**
 * get the user's home directory
 * @returns the user's home directory
 */
char* os_utils_get_homedir();

/**
 * join 2 pieces of a filepath, being careful about the slashes
 * @param root the root part
 * @param extension what should be concatenated
 * @param results where to put the results
 * @param max_len throw an error if the total is longer than max_len
 */
int os_utils_filepath_join(char* root, char* extension, char* results, unsigned long max_len);

int os_utils_file_exists(char* file_name);

int os_utils_directory_writeable(char* path);

#endif /* utils_h */

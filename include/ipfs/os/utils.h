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
char* os_utils_getenv(const char* variable);

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
int os_utils_filepath_join(const char* root, const char* extension, char* results, unsigned long max_len);

int os_utils_file_exists(const char* file_name);

int os_utils_file_size(const char* file_name);

int os_utils_directory_writeable(const char* path);

#endif /* utils_h */

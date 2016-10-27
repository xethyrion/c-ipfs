#include "utils.h"

/**
 * get an environment varible from the os
 * @param variable the variable to look for
 * @returns the results
 */
char* os_utils_getenv(char* variable) {
	return getenv(variable);
}

/**
 * join 2 pieces of a filepath, being careful about the slashes
 * @param root the root part
 * @param extension what should be concatenated
 * @param results where to put the results
 * @param max_len throw an error if the total is longer than max_len
 */
int os_utils_filepath_join(char* root, char* extension, char* results, int max_len) {
	//TODO: implement slash checks
	if (strlen(root) + strlen(extension) + 1 > max_len)
		return 0;
	strncpy(results, root, strlen(root) + 1);
	strncat(results, extension, strlen(extension)+1);
	return 1;
}

int os_utils_file_exists(char* file_name) {
	if (access(file_name, F_OK) != -1)
		return 1;
	return 0;
}

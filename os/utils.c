#include "ipfs/os/utils.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>

/**
 * get an environment varible from the os
 * @param variable the variable to look for
 * @returns the results
 */
char* os_utils_getenv(const char* variable) {
	return getenv(variable);
}

/**
 * returns the user's home directory
 * @returns the home directory
 */
char* os_utils_get_homedir() {
	struct passwd *pw = getpwuid(getuid());
	return pw->pw_dir;
}

/**
 * join 2 pieces of a filepath, being careful about the slashes
 * @param root the root part
 * @param extension what should be concatenated
 * @param results where to put the results
 * @param max_len throw an error if the total is longer than max_len
 */
int os_utils_filepath_join(const char* root, const char* extension, char* results, unsigned long max_len) {
	if (strlen(root) + strlen(extension) + 1 > max_len)
		return 0;
	strncpy(results, root, strlen(root) + 1);
	// one of these should have a slash. If not, add one
	if (root[strlen(root)-1] != '/' && extension[0] != '/') {
		results[strlen(root)] = '/';
		results[strlen(root)+1] = 0;
	}
	strncat(results, extension, strlen(extension)+1);
	return 1;
}

int os_utils_file_exists(const char* file_name) {
	if (access(file_name, F_OK) != -1)
		return 1;
	return 0;
}

int os_utils_directory_writeable(const char* path) {
	int result = access(path, W_OK);
	return result == 0;
}

int os_utils_file_size(const char* path) {
	// open file
	FILE* in_file = fopen(path, "r");
	// determine size
	fseek(in_file, 0L, SEEK_END);
	size_t file_size = ftell(in_file);
	fclose(in_file);
	return file_size;
}

#include "utils.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

/**
 * get an environment varible from the os
 * @param variable the variable to look for
 * @returns the results
 */
char* os_utils_getenv(char* variable) {
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
int os_utils_filepath_join(char* root, char* extension, char* results, unsigned long max_len) {
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

int os_utils_directory_writeable(char* path) {
	int result = access("/root/", W_OK);
	return result == 0;
}

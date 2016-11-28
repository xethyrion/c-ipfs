/**
 * a datastore implementation that stores all
 * objects in a 2 level directory structure in
 * the local file system, regardless of the
 * hierarchy of the keys. Modeled after go-ds-flatfs
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "ipfs/os/utils.h"

#define FLATFS_MAX_PREFIX_LENGTH 16

/**
 * Helper (private) methods
 */

/**
 * remove beginning slash from string
 * @param in the filename to look at
 * @param out a place to store the results
 * @param the size of out
 * @returns true(1) on success
 */
int ipfs_flatfs_remove_preceeding_slash(const char* in, char* out, size_t max_size) {
	// make sure max_size is reasonable
	if (max_size < strlen(in) + 1)
		return 0;
	int pos = 0;
	while (in[pos] == '/')
		pos++;
	strncpy(out, &in[pos], strlen(in) - pos);
	out[strlen(in) - pos] = 0;
	return 1;
}

/**
 * Create a directory if it doesn't already exist
 * @param full_directory the full path
 * @returns true(1) on successful create or if it already exists and is writable. false(0) otherwise.
 */
int ipfs_flatfs_create_directory(const char* full_directory) {
	// shortcut
	if (os_utils_directory_writeable(full_directory))
		return 1;
	// is it there, just not writeable?
	if (os_utils_directory_exists(full_directory)) {
		return 0;
	}
	// it is not there, create it
	if (mkdir(full_directory, S_IRWXU) == -1)
		return 0;

	return 1;
}

/***
 * public methods
 */

/**
 * Given a filename (usually a long hash), derive a subdirectory name
 * @param datastore_path the path to the datastore
 * @param proposed_filename the filename to use
 * @param derived_path the complete pathname to the directory that should contain the proposed_filename
 * @param max_derived_path_length the maximum memory allocated for derived_path
 * @returns true(1) on success
 */
int ipfs_flatfs_get_directory(const char* datastore_path, const char* proposed_filename,
		char* derived_path, size_t max_derived_path_length) {
	// make sure max_derived_path_length is a reasonable number
	if (max_derived_path_length < strlen(datastore_path) + 17)
		return 0;

	// remove slash prefix if there is one
	char buffer[max_derived_path_length];
	int retVal = ipfs_flatfs_remove_preceeding_slash(proposed_filename, buffer, max_derived_path_length);
	if (retVal == 0)
		return 0;

	// make it 16 characters
	if (strlen(buffer) < 16) {
		int pos = strlen(buffer);
		int lacking = 16 - pos; // how many we should add
		memset(&buffer[strlen(buffer)], '_', lacking);
		buffer[pos + lacking] = 0;
	}
	// it may be too long, cut it
	if (strlen(buffer) > 16)
		buffer[16] = 0;
	retVal = os_utils_filepath_join(datastore_path, buffer, derived_path, max_derived_path_length);
	return retVal;
}

/**
 * Given the proposed filename, return the acutal filename on the disk (clean the name and add .data suffix)
 * @param proposed_filename the start
 * @param derived_filename the results
 * @param max_derived_filename_length the buffer size
 * @returns true(1) on success
 */
int ipfs_flatfs_get_filename(const char* proposed_filename, char* derived_filename, size_t max_derived_filename_length) {
	// get rid of slashes
	char buffer[max_derived_filename_length];
	int retVal = ipfs_flatfs_remove_preceeding_slash(proposed_filename, buffer, max_derived_filename_length);
	if (retVal == 0)
		return 0;

	// make sure we have space
	if (max_derived_filename_length < strlen(buffer) + 6) // ".data" plus terminating null
		return 0;

	// add the suffix
	strncat(buffer, ".data", 6);

	// put it in the result buffer
	strncpy(derived_filename, buffer, strlen(buffer) + 1);

	return 1;
}

/**
 * Combines the datastore path, the directory (derived from the filename itself), the proposed
 * filename, and the suffix (.data) to build a complete filename on the disk
 * @param datastore_path where the datastore is
 * @param proposed_filename the filename we want to use
 * @param derived_full_filename where the results will be put
 * @param max_derived_filename_length the size of memory allocated for "derived_full_filename"
 * @returns true(1) on success
 */
int ipfs_flatfs_get_full_filename(const char* datastore_path, const char* proposed_filename,
		char* derived_full_filename, size_t max_derived_filename_length) {
	// get rid of preceeding /
	char directory[max_derived_filename_length];
	int retVal = ipfs_flatfs_remove_preceeding_slash(proposed_filename, directory, max_derived_filename_length);
	if (retVal == 0)
		return 0;

	// start with the path
	retVal = ipfs_flatfs_get_directory(datastore_path, proposed_filename, directory, max_derived_filename_length);
	if (retVal == 0)
		return retVal;

	// now get the filename
	char actual_filename[max_derived_filename_length];
	retVal = ipfs_flatfs_get_filename(proposed_filename, actual_filename, max_derived_filename_length);
	if (retVal == 0)
		return 0;

	// now merge the two
	retVal = os_utils_filepath_join(directory, actual_filename, derived_full_filename, max_derived_filename_length);

	return retVal;
}


/**
 * Write a file given the key and the contents
 * @param datastore_path the root of the flatfs datastore
 * @param key the "filename"
 * @para byte the contents of the file as a byte array
 * @param num_bytes the length of the byte array
 */
int ipfs_flatfs_put(const char* datastore_path, const char* key, unsigned char* byte, size_t num_bytes) {
	size_t filename_length = strlen(datastore_path) + strlen(key) + 24; // subdirectory is 16, 2 slashes, .data suffix, terminating null
	// subdirectory
	char full_filename[filename_length];
	int retVal = ipfs_flatfs_get_directory(datastore_path, key, full_filename, filename_length);
	if (retVal == 0)
		return 0;
	retVal = ipfs_flatfs_create_directory(full_filename);
	if (retVal == 0)
		return 0;

	// filename
	retVal = ipfs_flatfs_get_full_filename(datastore_path, key, full_filename, filename_length);
	if (retVal == 0)
		return 0;

	//TODO: Error checking (i.e. too many open files

	// write temp file
	char temp_filename[filename_length + 5];
	strncpy(temp_filename, full_filename, strlen(full_filename) + 1);
	strcat(temp_filename, ".tmp");
	FILE* out = fopen(temp_filename, "w");
	size_t bytes_written = fwrite(byte, num_bytes, 1, out);
	fclose(out);

	// rename temp file to real name
	retVal = rename(temp_filename, full_filename);
	if (retVal != 0)
		return 0;

	return bytes_written == num_bytes;
}

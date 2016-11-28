/**
 * a datastore implementation that stores all
 * objects in a 2 level directory structure in
 * the local file system, regardless of the
 * hierarchy of the keys. Modeled after go-ds-flatfs
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
		char* derived_path, size_t max_derived_path_length);

/**
 * Given the proposed filename, return the acutal filename on the disk (clean the name and add .data suffix)
 * @param proposed_filename the start
 * @param derived_filename the results
 * @param max_derived_filename_length the buffer size
 * @returns true(1) on success
 */
int ipfs_flatfs_get_filename(const char* proposed_filename, char* derived_filename, size_t max_derived_filename_length);

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
		char* derived_full_filename, size_t max_derived_filename_length);

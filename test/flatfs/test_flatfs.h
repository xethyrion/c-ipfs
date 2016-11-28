#include "ipfs/flatfs/flatfs.h"

int test_flatfs_get_directory() {
	char* datastore_directory = "/tmp/";
	char* proposed_filename = "/ABC123XYZ";
	size_t results_len = 256;
	char results[results_len];

	// buffer too small
	int retVal = ipfs_flatfs_get_directory(datastore_directory, proposed_filename, results, 21);
	if (retVal != 0)
		return 0;

	// buffer just right
	retVal = ipfs_flatfs_get_directory(datastore_directory, proposed_filename, results, 22);
	if (retVal == 0)
		return 0;
	if (strcmp(results, "/tmp/ABC123XYZ_______") != 0)
		return 0;

	// name too long
	proposed_filename = "12345678901234567";
	retVal = ipfs_flatfs_get_directory(datastore_directory, proposed_filename, results, 22);
	if (retVal == 0)
		return 0;
	if (strcmp(results, "/tmp/1234567890123456") != 0)
		return 0;

	return 1;
}

int test_flatfs_get_filename() {
	char* proposed_filename = "/ABC123XYZ";
	size_t results_len = 256;
	char results[results_len];

	// buffer too small
	int retVal = ipfs_flatfs_get_filename(proposed_filename, results, 14);
	if (retVal != 0)
		return 0;

	// buffer just right
	retVal = ipfs_flatfs_get_filename(proposed_filename, results, 15);
	if (retVal == 0)
		return 0;
	if (strcmp(results, "ABC123XYZ.data") != 0)
		return 0;

	return 1;

}

int test_flatfs_get_full_filename() {
	char* datastore_directory = "/tmp/";
	char* proposed_filename = "/ABC123XYZ";
	size_t results_len = 256;
	char results[results_len];

	// buffer too small
	int retVal = ipfs_flatfs_get_full_filename(datastore_directory, proposed_filename, results, 21);
	if (retVal != 0)
		return 0;

	// buffer just right
	retVal = ipfs_flatfs_get_full_filename(datastore_directory, proposed_filename, results, 50);
	if (retVal == 0)
		return 0;
	if (strcmp(results, "/tmp/ABC123XYZ_______/ABC123XYZ.data") != 0)
		return 0;

	// name too long
	proposed_filename = "12345678901234567";
	retVal = ipfs_flatfs_get_full_filename(datastore_directory, proposed_filename, results, 50);
	if (retVal == 0)
		return 0;
	if (strcmp(results, "/tmp/1234567890123456/12345678901234567.data") != 0)
		return 0;

	return 1;
}

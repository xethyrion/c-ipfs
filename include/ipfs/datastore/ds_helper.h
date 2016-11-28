/**
 * Some code to help with the datastore / blockstore interface
 */
#ifndef __IPFS_DATASTORE_DS_HELPER_H__
#define __IPFS_DATASTORE_DS_HELPER_H__

#include <string.h>

/**
 * Generate a key based on the passed in binary_array
 * @param binary_array what to base the key on
 * @param array_length the size of the binary array
 * @param results where the key will be put
 * @param max_results_length the size of the results buffer
 * @param results_length the length of the generated key
 * @returns true(1) on success
 */
int ipfs_datastore_helper_ds_key_from_binary(unsigned char* binary_array, size_t array_length,
		char* results, size_t max_results_length, size_t* results_length);

/**
 * Generate a binary array based on the passed in datastore key
 * @param ds_key the base32 encoded key
 * @param key_length the length of the base32 "string"
 * @param binary_array where to put the decoded value
 * @param max_binary_array_length the memory size of binary_array
 * @param completed_binary_array_length the length of what was written to the binary_array
 * @returns true(1) on success
 */
int ipfs_datastore_helper_binary_from_ds_key(unsigned char* ds_key, size_t key_length, unsigned char* binary_array,
		size_t max_binary_array_length, size_t* completed_binary_array_length);

#endif

#ifndef __DATASTORE_H__
#define __DATASTORE_H__

#include <stdint.h>

//const char* datastore_default_directory = "datastore";

struct Datastore {
	char* type;
	char* path;
	char* storage_max;
	int storage_gc_watermark;
	char* gc_period;
	char* params;
	int no_sync;
	int hash_on_read;
	int bloom_filter_size;

	// function pointers for datastore operations
	int (*datastore_open)(int argc, char** argv, struct Datastore* datastore);
	int (*datastore_close)(int argc, char** argv, struct Datastore* datastore);
	// a handle to the datastore "context" used by the datastore
	void* handle;
};

/***
 * Initialize the structure of the datastore to default settings. Used for
 * creating a new datastore on the disk.
 * @param datastore the struct to initialize
 * @param config_root the path to the root of IPFS
 * @returns true(1) on success
 */
int ipfs_repo_config_datastore_init(struct Datastore* datastore, char* config_root);

/***
 * initialize the structure of the datastore
 * @param datastore the struct to initialize
 * @returns true(1) on success
 */
int ipfs_repo_config_datastore_new(struct Datastore** datastore);


/***
 * deallocate the memory and clear resources from a datastore_init
 * @param datastore the struct to deallocate
 * @returns true(1)
 */
int ipfs_repo_config_datastore_free(struct Datastore* datastore);


#endif

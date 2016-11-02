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
};

/***
 * initialize the structure of the datastore
 * @param datastore the struct to initialize
 * @param config_root the path to the root of IPFS
 * @returns true(1) on success
 */
int repo_config_datastore_init(struct Datastore* datastore, char* config_root);

/***
 * deallocate the memory and clear resources from a datastore_init
 * @param datastore the struct to deallocate
 * @returns true(1)
 */
int repo_config_datastore_free(struct Datastore* datastore);


#endif

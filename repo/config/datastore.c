//
//  datastore.c
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdlib.h>
#include <string.h>

#include "ipfs/repo/config/datastore.h"
#include "ipfs/os/utils.h"

/***
 * initialize the structure of the datastore
 * @param datastore the struct to initialize
 * @returns true(1) on success
 */
int repo_config_datastore_init(struct Datastore* datastore, char* config_root) {
	unsigned long stringLength = strlen(config_root) + 12;
	datastore->path = malloc(sizeof(char) * stringLength);
	os_utils_filepath_join(config_root, "/datastore", datastore->path, stringLength);
	datastore->type = "leveldb";
	datastore->storage_max = "10GB";
	datastore->storage_gc_watermark = 90;
	datastore->gc_period = "1h";
	datastore->hash_on_read = 0;
	datastore->bloom_filter_size = 0;
	return 1;
}

/***
 * deallocate the memory and clear resources from a datastore_init
 * @param datastore the struct to deallocate
 * @returns true(1)
 */
int repo_config_datastore_free(struct Datastore* datastore) {
	//free(datastore);
	return 1;
}

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
int ipfs_repo_config_datastore_init(struct Datastore* datastore, char* config_root) {
	unsigned long stringLength = strlen(config_root) + 12;
	datastore->path = malloc(sizeof(char) * stringLength);
	os_utils_filepath_join(config_root, "datastore", datastore->path, stringLength);
	datastore->type = "lmdb";
	datastore->storage_max = "10GB";
	datastore->storage_gc_watermark = 90;
	datastore->gc_period = "1h";
	datastore->hash_on_read = 0;
	datastore->bloom_filter_size = 0;
	return 1;
}

/***
 * initialize the structure of the datastore
 * @param datastore the struct to initialize
 * @returns true(1) on success
 */
int ipfs_repo_config_datastore_new(struct Datastore** datastore) {
	*datastore = malloc(sizeof(struct Datastore));
	if (*datastore == NULL)
		return 0;
	(*datastore)->path = NULL;
	(*datastore)->handle = NULL;
	return 1;
}

/***
 * deallocate the memory and clear resources from a datastore_init
 * @param datastore the struct to deallocate
 * @returns true(1)
 */
int ipfs_repo_config_datastore_free(struct Datastore* datastore) {
	if (datastore != NULL)
	{
		if (datastore->path != NULL)
			free(datastore->path);
		free(datastore);
	}
	return 1;
}

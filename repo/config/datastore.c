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

int alloc_and_assign(char** result, const char* string) {
	*result = malloc(strlen(string)+1);
	if (*result == NULL)
		return 0;
	strcpy(*result, string);
	return 1;
}

/***
 * initialize the structure of the datastore
 * @param datastore the struct to initialize
 * @returns true(1) on success
 */
int ipfs_repo_config_datastore_init(struct Datastore* datastore, char* config_root) {
	unsigned long stringLength = strlen(config_root) + 12;
	datastore->path = malloc(sizeof(char) * stringLength);
	os_utils_filepath_join(config_root, "datastore", datastore->path, stringLength);
	alloc_and_assign(&datastore->type, "lmdb");
	alloc_and_assign(&datastore->storage_max, "10GB");
	datastore->storage_gc_watermark = 90;
	alloc_and_assign(&datastore->gc_period, "1h");
	datastore->hash_on_read = 0;
	datastore->bloom_filter_size = 0;
	datastore->no_sync = 0;
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
	(*datastore)->type = NULL;
	(*datastore)->storage_max = NULL;
	(*datastore)->gc_period = NULL;
	(*datastore)->params = NULL;
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
		if (datastore->type != NULL)
			free(datastore->type);
		if (datastore->storage_max != NULL)
			free(datastore->storage_max);
		if (datastore->gc_period != NULL)
			free(datastore->gc_period);
		if (datastore->params != NULL)
			free(datastore->params);
		if (datastore->handle != NULL)
			datastore->datastore_close(datastore);
		free(datastore);
	}
	return 1;
}

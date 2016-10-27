#ifndef __REPO_H__
#define __REPO_H__

#include <stdint.h>

#include "config/config.h"

/**
 * Get the config
 * @param config a place to put the buffer (must have been pre-allocated)
 * @returns 0 on error
 */
int repo_get_config(struct Config* config);

/**
 * Retrieves the config
 * @param config a place to get the information
 * @returns 0 on error
 */
int repo_set_config(struct Config* config);
int repo_set_config_key(char* key, void* value);
int repo_get_config_key(char* key, void* value);
int repo_get_datastore(struct Datastore* datastore);
int repo_get_storage_usage(uint64_t* usage);

#endif // __REPO_H__

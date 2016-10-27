#ifndef __DATASTORE_H__
#define __DATASTORE_H__

#include <stdint.h>

const char* datastore_default_directory = "datastore";

struct datastore {
	char* type;
	char* path;
	char* storage_max;
	int64_t storage_gc_watermark;
	char* gc_period;

	char* params;
	int no_sync;
	int hash_on_read;
	int bloom_filter_size;
};

#endif

/***
 * Here are the wrappers for the lightning database
 */

#include <stdlib.h>

#include "lmdb.h"
#include "ipfs/repo/fsrepo/lmdb_datastore.h"

/**
 * Open an lmdb database with the given parameters.
 * Note: for now, the parameters are not used
 * @param argc number of parameters in the following array
 * @param argv an array of parameters
 */
int repo_fsrepro_lmdb_open(int argc, char** argv, struct Datastore* datastore) {
	// create environment
	struct MDB_env* mdb_env;
	int retVal = mdb_env_create(&mdb_env);
	if (retVal < 0) {
		mdb_env_close(mdb_env);
		return 0;
	}

	// open the environment
	retVal = mdb_env_open(mdb_env, datastore->path, 0, 755);
	if (retVal < 0) {
		mdb_env_close(mdb_env);
		return 0;
	}

	datastore->handle = (void*)mdb_env;
	return 1;
}

/***
 * Close an LMDB database
 * NOTE: for now, argc and argv are not used
 * @param argc number of parameters in the argv array
 * @param argv parameters to be passed in
 * @param datastore the datastore struct that contains information about the opened database
 */
int repo_fsrepo_lmdb_close(int argc, char** argv, struct Datastore* datastore) {
	struct MDB_env* mdb_env = (struct MDB_env*)datastore->handle;
	mdb_env_close(mdb_env);
	free(mdb_env);
	return 1;
}

/***
 * Places the LMDB methods into the datastore's function pointers
 * @param datastore the datastore to fill
 * @returns true(1) on success;
 */
int repo_fsrepo_lmdb_cast(struct Datastore* datastore) {
	datastore->datastore_open = &repo_fsrepro_lmdb_open;
	datastore->datastore_close = &repo_fsrepo_lmdb_close;
	return 1;
}


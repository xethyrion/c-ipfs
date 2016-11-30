/***
 * Here are the wrappers for the lightning database
 */

#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include "lmdb.h"
#include "ipfs/repo/fsrepo/lmdb_datastore.h"

/**
 * Write a block to the datastore with the specified key
 * @param key the key
 * @param block the block to be written
 * @returns true(1) on success
 */
int repo_fsrepo_lmdb_put(const char* key, struct Block* block, struct Datastore* datastore) {
	int retVal;
	MDB_txn* mdb_txn;
	MDB_dbi mdb_dbi;
	struct MDB_val db_key;
	struct MDB_val db_value;

	MDB_env* mdb_env = (MDB_env*)datastore->handle;
	if (mdb_env == NULL)
		return 0;

	// open transaction
	retVal = mdb_txn_begin(mdb_env, NULL, 0, &mdb_txn);
	if (retVal != 0)
		return 0;
	retVal = mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi);
	if (retVal != 0)
		return 0;

	// write
	db_key.mv_size = strlen(key) + 1;
	db_key.mv_data = (char*)key;
	db_value.mv_size = block->data_length;
	db_value.mv_data = block->data;
	retVal = mdb_put(mdb_txn, mdb_dbi, &db_key, &db_value, MDB_NODUPDATA);
	if (retVal != 0)
		return 0;

	// cleanup
	mdb_dbi_close(mdb_env, mdb_dbi);
	mdb_txn_commit(mdb_txn);
	return 1;
}



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
	retVal = mdb_env_open(mdb_env, datastore->path, 0, S_IRWXU);
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
	datastore->datastore_put = &repo_fsrepo_lmdb_put;
	//datastore->datastore_get = &repo_fsrepo_lmdb_get;
	return 1;
}

/***
 * Creates the directory
 * @param datastore contains the path that needs to be created
 * @returns true(1) on success
 */
int repo_fsrepo_lmdb_create_directory(struct Datastore* datastore) {
	return mkdir(datastore->path, S_IRWXU) == 0;
}


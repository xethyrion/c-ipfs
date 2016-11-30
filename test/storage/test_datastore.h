#include "libp2p/crypto/encoding/base32.h"
#include "ipfs/datastore/ds_helper.h"
#include "ipfs/blocks/block.h"
#include "ipfs/repo/config/config.h"
#include "ipfs/repo/fsrepo/fs_repo.h"

#include <dirent.h>
#include <sys/stat.h>

int remove_directory(const char *path)
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d)
   {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2;
          buf = malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
   }

   if (!r)
   {
      r = rmdir(path);
   }

   return r;
}

int make_ipfs_repository(struct FSRepo* fs_repo) {
	int retVal;
	struct RepoConfig* repo_config;

	unlink("/tmp/.ipfs/config");
	remove_directory("/tmp/.ipfs/datastore");
	remove_directory("/tmp/.ipfs/blockstore");

	// build a default repo config
	retVal = ipfs_repo_config_new(&repo_config);
	if (retVal == 0)
		return 0;
	retVal = ipfs_repo_config_init(repo_config, 2048, "/tmp/.ipfs");
	if (retVal == 0)
		return 0;
	// now the fs_repo
	retVal = ipfs_repo_fsrepo_new("/tmp/.ipfs", repo_config, &fs_repo);
	if (retVal == 0)
		return 0;
	// this builds a new repo
	retVal = ipfs_repo_fsrepo_init(fs_repo);
	if (retVal == 0)
		return 0;

	// clean up
	ipfs_repo_fsrepo_free(fs_repo);
	// this is cleaned up by fsrepo_free
	//ipfs_repo_config_free(repo_config);

	// make sure the repository exists
	retVal = os_utils_file_exists("/tmp/.ipfs/config");
	return retVal;
}

int test_ipfs_datastore_put() {
	struct Block* block;
	int retVal;
	const unsigned char* input = "Hello, world!";

	// build the ipfs repository, then shut it down, so we can start fresh
	struct FSRepo* fs_repo;
	retVal = make_ipfs_repository(fs_repo);
	ipfs_repo_fsrepo_free(fs_repo);
	if (retVal == 0)
		return 0;

	// build the block
	retVal = ipfs_blocks_block_new(input, strlen((char*)input), &block);
	if (retVal == 0)
		return 0;

	// generate the key
	size_t key_length = libp2p_crypto_encoding_base32_encode_size(block->data_length);
	unsigned char key[key_length];
	retVal = ipfs_datastore_helper_ds_key_from_binary(block->data, block->data_length, &key[0], key_length, &key_length);
	if (retVal == 0)
		return 0;

	// open the repository
	retVal = ipfs_repo_fsrepo_new("/tmp/.ipfs", NULL, &fs_repo);
	if (retVal == 0)
		return 0;
	retVal = ipfs_repo_fsrepo_open(fs_repo);
	if (retVal == 0)
		return 0;

	// send to Put with key
	retVal = fs_repo->config->datastore->datastore_put(key, block, fs_repo->config->datastore);
	if (retVal == 0)
		return 0;

	// save the block

	// check the results

	return 1;
}

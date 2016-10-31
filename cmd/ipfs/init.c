#include <stdlib.h>
#include <stdio.h>

#include "ipfs/cmd/ipfs/init.h"
#include "ipfs/commands/command_option.h"
//#include "ipfs/commands/request.h"
#include "ipfs/os/utils.h"
#include "ipfs/core/ipfs_node.h"
#include "ipfs/core/builder.h"
#include "ipfs/repo/config/config.h"
#include "ipfs/repo/fsrepo/fs_repo.h"

const int nBitsForKeypairDefault = 2048;

/***
 * runs before major processing during initialization
 * @param request the request
 * @returns 0 if a problem, otherwise a 1
 */
int init_pre_run(struct Request* request) {
	//TODO: make sure daemon is not running
	return 0;
}

int initialize_ipns_keyspace(char* repo_root) {
	//TODO: open fs repo
	struct FSRepo repo;
	// get the path
	int retVal = fs_repo_open(repo_root, &repo);
	//TODO: make a new node, then close it
	//TODO: setup offline routing on new node
	struct IpfsNode* ipfs_node;
	struct Context* ctx;
	struct BuildCfg* bld_cfg;
	//TODO: see line 185 of init.go, what does core.BldCfg{Repo: r} do? BldCfg is a structure
	retVal = core_builder_new_node(ctx, bld_cfg, ipfs_node);
	//return namesys_initialize_keyspace(ctx, ipfs_node->DAG, ipfs_node->Namesys, ipfs_node->pinning, ipfs_node->private_key);
	return 0;
}

/**
 * called by init_run, to do the heavy lifting
 * @param out_file an output stream (stdout)
 * @param repo_root a string
 * @param empty true(1) if empty, false(0) if not
 * @param num_bits_for_keypair number of bits for key pair
 * @param conf the configuration struct
 * @returns 0 on error, 1 on success
 */
int do_init(FILE* out_file, char* repo_root, int empty, int num_bits_for_keypair, struct RepoConfig* conf) {
	// make sure the directory is writable
	if (!os_utils_directory_writeable(repo_root))
		return 0;
	// verify that it is not already initialized
	if (!fs_repo_is_initialized(repo_root))
		return 0;
	//TODO: If the conf is null, make one
	if (conf == NULL)
		repo_config_init(conf, num_bits_for_keypair);
	//TODO: initialize the fs repo
	//TODO: add default assets
	return initialize_ipns_keyspace(repo_root);
}

/***
 * does major processing during initialization
 * @param request the request
 * @returns 0 if a problem, otherwise a 1
 */
int init_run(struct Request* request) {
	// TODO: make sure offline
	// TODO: check parameters for logic errors
	// TODO: Initialize
	struct RepoConfig conf;
	// TODO: handle files in request
	// do the heavy lifting
	int num_bits_for_key_pair = request->cmd->options[1]->default_int_val;
	do_init(stdout, request->invoc_context->config_root, 1, num_bits_for_key_pair, &conf);
	
	return 0;
}

/***
 * does the cleanup after major processing during initialization
 * @param request the request
 * @returns 0 if a problem, otherwise a 1
 */
int init_post_run(struct Request* request) {
	// nothing to do
	return 1;
}

int get_init_command(struct Command* cmd) {
	// help text
	cmd->help_text.tagline = "Initializes IPFS config file.";
	cmd->help_text.short_description = "\nInitializes IPFS configuration files and generates a new keypair.\n\nipfs uses a repository in the local file system. By default, the repo is\nlocated at ~/.ipfs. To change the repo location, set the $IPFS_PATH\nenvironment variable.:\n\n    export IPFS_PATH=/path/to/ipfsrepo";

	cmd->argument_count = 1;
	cmd->option_count = 2;
	init_command(cmd);
	// allocate memory for array of pointers
	int retVal = init_string_argument(cmd->arguments[0], "default-config", 0, 0, "Initialize with the given configuration");
	cmd->arguments[0]->enable_stdin = 1;
	
	// options
	cmd->options[0]->name_count = 2;
	retVal = init_command_option(cmd->options[0], "Number of bits to use in the generated RSA private key");
	cmd->options[0]->names[0] = "bits";
	cmd->options[0]->names[1] = "b";
	cmd->options[0]->kind = integer;
	cmd->options[0]->default_int_val = nBitsForKeypairDefault;
	cmd->options[1]->name_count = 2;
	retVal = init_command_option(cmd->options[1], "Don't add and pin help files to the local storage");
	cmd->options[1]->default_bool_val = 0;
	cmd->options[1]->names[0] = "empty-repo";
	cmd->options[1]->names[1] = "e";
	
	// function pointers
	cmd->pre_run = init_pre_run;
	cmd->run = init_run;
	cmd->post_run = init_post_run;
	
	return retVal;
}



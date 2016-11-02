//
//  ipfs_addr.h
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef ipfs_addr_h
#define ipfs_addr_h

struct IPFSAddr {
	char* entire_string;
};

/**
 * initialize a new IPFSAddr struct. NOTE: be sure to use ipfsaddr_free()
 * @param addr the struct to initialize
 * @param string the string that contains the address
 * @returns true(1) on success, false(0) otherwise
 */
int ipfsaddr_init_new(struct IPFSAddr** addr, char* string);

/***
 * frees allocated memory in the struct
 * @param addr the struct that is going away
 * @returns true(1) on success, false(0) otherwise
 */
int ipfsaddr_free(struct IPFSAddr* addr);

#endif /* ipfs_addr_h */

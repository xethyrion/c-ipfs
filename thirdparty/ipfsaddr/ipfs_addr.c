//
//  ipfs_addr.c
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipfs/thirdparty/ipfsaddr/ipfs_addr.h"

int ipfsaddr_new(struct IPFSAddr** addr, char* string) {
	(*addr) = malloc(sizeof(struct IPFSAddr));
	(*addr)->entire_string = malloc(sizeof(char) * (strlen(string) + 1));
	if ((*addr)->entire_string == NULL)
		return 0;
	strncpy((*addr)->entire_string, string, strlen(string));
	return 1;
}

int ipfsaddr_free(struct IPFSAddr* addr) {
	if (addr != NULL) {
		if (addr->entire_string != NULL)
			free(addr->entire_string);
		free(addr);
	}
	return 1;
}

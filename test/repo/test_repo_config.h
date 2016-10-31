//
//  test_repo_config.h
//  c-ipfs
//
//  Created by John Jones on 10/31/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef test_repo_config_h
#define test_repo_config_h

#include "ipfs/repo/config/identity.h"

int test_repo_config_identity_new() {
	struct Identity* identity;
	return repo_config_identity_new(identity, 2046);
}

#endif /* test_repo_config_h */

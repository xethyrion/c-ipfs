//
//  gateway.h
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef gateway_h
#define gateway_h

struct HTTPHeader {
	char* header;
	char* value;
};

struct HTTPHeaders {
	struct HTTPHeader** headers;
	int num_elements;
};

struct PathPrefixes {
	int num_elements;
	char** prefixes;
};

struct Gateway {
	char* root_redirect;
	int writable;
	struct PathPrefixes path_prefixes;
	struct HTTPHeaders* http_headers;
};

int repo_config_gateway_http_header_init(struct HTTPHeaders* http_headers, char** headers, char** values, int num_elements);

int repo_config_gateway_new(struct Gateway** gateway);

int repo_config_gateway_free(struct Gateway* gateway);

#endif /* gateway_h */

//
//  gateway.c
//  c-ipfs
//
//  Created by John Jones on 11/2/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipfs/repo/config/gateway.h"

char* alloc_and_fill(char* source) {
	char* newString = malloc(sizeof(char) * (strlen(source) + 1));
	strncpy(newString, source, strlen(source));
	newString[strlen(source)] = 0;
	return newString;
}

int repo_config_gateway_http_header_init(struct HTTPHeaders* http_headers, char** headers, char** values, int num_elements) {
	// allocate memory for array
	http_headers->headers = malloc(sizeof(struct HTTPHeader*) * num_elements);
	if (http_headers->headers == NULL) {
		http_headers->num_elements = 0;
		return 0;
	}
	
	// now fill in the array
	for(int i = 0; i < num_elements; i++) {
		http_headers->headers[i] = malloc(sizeof(struct HTTPHeader));
		if (http_headers->headers[i] == NULL) {
			http_headers->num_elements = i;
			return 0;
		}
		http_headers->headers[i]->header = alloc_and_fill(headers[i]);
		http_headers->headers[i]->value = alloc_and_fill(values[i]);
	}
	
	http_headers->num_elements = num_elements;
	return 1;
}

int repo_config_gateway_new(struct Gateway** gateway) {
	*gateway = (struct Gateway*)malloc(sizeof(struct Gateway));
	if (*gateway == NULL)
		return 0;
	(*gateway)->http_headers = (struct HTTPHeaders*)malloc(sizeof(struct HTTPHeaders));
	if ((*gateway)->http_headers == NULL) {
		free(*gateway);
		return 0;
	}
	(*gateway)->http_headers->num_elements = 0;
	(*gateway)->http_headers->headers = NULL;
	return 1;
}

int repo_config_gateway_free(struct Gateway* gateway) {
	if (gateway->http_headers != NULL) {
		for(int i = 0; i < gateway->http_headers->num_elements; i++) {
			struct HTTPHeader* currHeader = gateway->http_headers->headers[i];
			free(currHeader->header);
			free(currHeader->value);
			free(currHeader);
		}
		if (gateway->http_headers->headers != NULL)
			free(gateway->http_headers->headers); // from init
		free(gateway->http_headers); // from new
	}
	free(gateway); // from new
	return 1;
}

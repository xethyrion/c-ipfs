//
//  base64.c
//  c-ipfs
//
//  Created by John Jones on 10/31/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include "mbedtls/base64.h"

/**
 * encode using base64
 * @param input_data the data to be encoded
 * @param input_length the length of the input data
 * @param output_data where the data is to be stored
 * @param max_output_length the max size of the output_data
 * @param bytes_written the number of bytes written to output_data
 * @returns true(1) on success, otherwise false
 */
int base64_encode(const unsigned char* input_data, size_t input_length, unsigned char* output_data, size_t max_output_length, size_t* bytes_written) {
	int retVal = mbedtls_base64_encode(output_data, max_output_length, bytes_written, input_data, input_length);
	return retVal == 0;
}

size_t base64_encode_length(const unsigned char* input_data, size_t input_length) {
	size_t req_bytes;
	mbedtls_base64_encode(NULL, 0, &req_bytes, input_data, input_length);
	return req_bytes;
}

/**
 * decode something that was encoded as base64
 * @param input_data the data to decode
 * @param input_length the length of the input data
 * @param output_data the buffer to store the output
 * @param max_output_length the length of the output buffer
 * @param bytes_written the number of bytes written to output_data
 * @returns a pointer to the decoded data
 */
int base64_decode(const unsigned char* input_data, size_t input_length, unsigned char* output_data, size_t max_output_length, size_t* bytes_written) {
	int retVal = mbedtls_base64_decode(output_data, max_output_length, bytes_written, input_data, input_length);
	return retVal == 0;
}

size_t base64_decode_length(const unsigned char* input_data, size_t input_length) {
	size_t req_bytes;
	mbedtls_base64_decode(NULL, 0, &req_bytes, input_data, input_length);
	return req_bytes;
}

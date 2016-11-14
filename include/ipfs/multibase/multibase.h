#include <stdio.h>

#ifndef __IPFS_MULTIBASE_MULTIBASE_H__
#define __IPFS_MULTIBASE_MULTIBASE_H__

// the first digit of data, to determine the encoding used or using
#define MULTIBASE_BASE1 '1'
#define MULTIBASE_BASE2 '0'
#define MULTIBASE_BASE8 '7'
#define MULTIBASE_BASE10 '9'
#define MULTIBASE_BASE16 'f'
#define MULTIBASE_BASE58_FLICKR 'Z'
#define MULTIBASE_BASE58_BTC 'z'

/**
 * Encode data in multibase format
 * @param base the format to use (i.e. MULTIBASE_BASE58_BTC)
 * @param incoming the data to encode
 * @param incoming_length the length of the data to encode
 * @param results where to put the results
 * @param results_max_length the size of the results buffer
 * @param results_length the size of the results after being encoded
 * @returns true(1) on success
 */
int multibase_encode(const char base, const unsigned char* incoming, size_t incoming_length, unsigned char* results, size_t results_max_length, size_t* results_length);

/***
 * Calculates the size of the buffer neccessary to encode the incoming byte array
 * @param base the encoding to use
 * @param incoming the incoming array of bytes
 * @param incoming_length the length of the array in bytes
 * @returns the appropriate size of the buffer
 */
int multibase_encode_size(const char base, const unsigned char* incoming, size_t incoming_length);

/**
 * Decode data that was encoded in multibase format
 * @param incoming the data to decode
 * @param incoming_length the length of the data to decode
 * @param results where to put the results
 * @param results_max_length the size of the results buffer
 * @param results_length the size of the results after being encoded
 * @returns true(1) on success
 */
int multibase_decode(const unsigned char* incoming, size_t incoming_length, unsigned char* results, size_t results_max_length, size_t* results_length);

#endif

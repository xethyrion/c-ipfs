//
//  identity.c
//  c-ipfs
//
//  Created by John Jones on 10/31/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipfs/repo/config/identity.h"
#include "ipfs/repo/config/base64.h"

// mbedtls stuff
#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/bignum.h"
#include "mbedtls/x509.h"
#include "mbedtls/rsa.h"

int repo_config_identity_generate_keypair(unsigned char* private_key, unsigned char* public_key, unsigned long num_bits_for_keypair) {

	mbedtls_rsa_context rsa;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	
	int exponent = 65537;
	int retVal = 1;
	
	const char *pers = "rsa_genkey";
	
	// initialize mbedtls structs
	mbedtls_ctr_drbg_init( &ctr_drbg );
	mbedtls_entropy_init( &entropy );
	
	// seed the routines
	if( ( retVal = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
									  (const unsigned char *) pers,
									  strlen( pers ) ) ) != 0 )
	{
		retVal = 0;
		goto exit;
	}
	
	// initialize the rsa struct
	mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
	
	// finally, generate the key
	if( ( retVal = mbedtls_rsa_gen_key( &rsa, mbedtls_ctr_drbg_random, &ctr_drbg, (unsigned int)num_bits_for_keypair,
									exponent ) ) != 0 )
	{
		retVal = 0;
		goto exit;
	}
	retVal = 1;
	
	struct {
		unsigned long long modulus;
		unsigned long long public_exponent;
	} pub_key;
	
	pub_key.modulus = *(rsa.N.p);
	pub_key.public_exponent = *(rsa.E.p);
	
	struct {
		unsigned long long modulus;
		unsigned long long public_exponent;
		unsigned long long private_exponent;
		unsigned long long prime1;
		unsigned long long prime2;
		unsigned long long exponent1;
		unsigned long long exponent2;
		unsigned long long coefficient;
	} priv_key;
	
	priv_key.modulus = *(rsa.N.p);
	priv_key.public_exponent = *(rsa.E.p);
	priv_key.private_exponent = *(rsa.D.p);
	priv_key.prime1 = *(rsa.P.p);
	priv_key.prime2 = *(rsa.Q.p);
	//TODO: verify these 3
	priv_key.exponent1 = *(rsa.DP.p);
	priv_key.exponent2 = *(rsa.DQ.p);
	priv_key.coefficient = *(rsa.QP.p);
	
	// convert keys to base 64 and then store
	unsigned char* input_buf = malloc(sizeof(pub_key));
	memcpy(input_buf, &pub_key, sizeof(pub_key));
	size_t bufLen = base64_encode_length(input_buf, sizeof(pub_key));
	public_key = malloc(sizeof(char) * bufLen);
	retVal = base64_encode(input_buf, sizeof(pub_key), public_key, bufLen, &bufLen);
	free(input_buf);
	
	input_buf = malloc(sizeof(priv_key));
	memcpy(input_buf, &priv_key, sizeof(priv_key));
	bufLen = base64_encode_length(input_buf, sizeof(pub_key));
	private_key = malloc(sizeof(char) * bufLen);
	retVal = base64_encode(input_buf, sizeof(priv_key), private_key, bufLen, &bufLen);
	
	retVal = 0;
exit:

	mbedtls_rsa_free( &rsa );
	mbedtls_ctr_drbg_free( &ctr_drbg );
	mbedtls_entropy_free( &entropy );
	
	return retVal;
}

/***
 * public methods
 */

/***
 * initializes a new Identity (this should be moved to identity.h)
 * @param identity the identity to fill
 * @param num_bits_for_keypair the number of bits for the keypair
 * @returns true(1) on success, false(0) otherwise
 */
int repo_config_identity_new(struct Identity* identity, unsigned long num_bits_for_keypair) {
	identity = malloc(sizeof(struct Identity));
	if (num_bits_for_keypair < 1024)
		return 0;
	unsigned char* private_key;
	unsigned char* public_key;
	if (!repo_config_identity_generate_keypair(private_key, public_key, num_bits_for_keypair))
		return 0;
	return 0;
}

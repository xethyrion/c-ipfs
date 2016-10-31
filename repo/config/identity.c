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

int repo_config_identity_generate_keypair(struct PrivateKey* private_key, unsigned long num_bits_for_keypair) {

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
	
	// fill in values of structures
	private_key->public_key.modulus = *(rsa.N.p);
	private_key->public_key.exponent = *(rsa.E.p);
	private_key->prime1 = *(rsa.DP.p);
	private_key->prime2 = *(rsa.Q.p);
	private_key->private_exponent = *(rsa.D.p);
	
	//TODO: fill in the rest of the precomputed values
	private_key->precomputed_values.dp = *(rsa.DP.p);
	private_key->precomputed_values.dq = *(rsa.DQ.p);
	
	retVal = 1;
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
	// generate the private key (& public)
	if (!repo_config_identity_generate_keypair(&identity->private_key, num_bits_for_keypair))
		return 0;
	
	// TODO: Get ID from public key
	// TODO: Store peer id in identity struct
	return 0;
}

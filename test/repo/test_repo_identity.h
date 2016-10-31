//
//  test_repo_identity.h
//  c-ipfs
//
//  Created by John Jones on 10/31/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef test_repo_identity_h
#define test_repo_identity_h

#include <stdlib.h>

#include "ipfs/repo/config/identity.h"
#include "ipfs/repo/config/base64.h"

int test_repo_config_identity_new() {
	struct Identity* identity;
	return repo_config_identity_new(identity, 2046);
}

// test this key
int test_repo_config_identity_private_key() {
	const char* priv_b64 = "CAASpwkwggSjAgEAAoIBAQDTDJBWjDzS/HxDNOHazvzH2bu9CPMVHUrrvKRdBUM5ansL6/CC3MVZ6HVm4O6QHRapN6EF2CbrTgI4KBOXIL125Xo8MlROnyfXYk3O5q2tgwL/MbW8kXjtkyCfBak7MUoLOdLU7Svg0gkl3l+uDAiDcCLnwJVcFfq9ch6z4wMOhYJqE5dtx0uXxn6IuKWl1B69FTvBXCc0thw8Rw54b941FDcsBH5ttV9mRNCJym3poZ5qalNgXlxoIIB+PUx5QD+aq7KMJdpAX8HkapBntCOahP/QUceRmma0grlZLeYkH6/oi/hIrM6se3KUZ+F6tBuDFys8UAZy/X2BCUbKjbxtAgMBAAECggEANWfQfpYuLhXGPBt9q6kFPm1SnJtPJ+CpvM2XqhJS2IyhZnrl+bd0GTRBwS7aL42s1lVFYf04nAK5fQxnKK8YQqX/MIxr2RldM5ukpN6qxGWKtJkXrAgD2dqJPrRoBpqKahzPxSHfIJ0Fw5dqDtjsrpYJvyt0oEDPmnDuZAbmFx4sJqnesPNhKxtRMBx1+yxGVuRVJjHcqAgqPqwNiuoMEaYMY+G9yzT6vza8ovCpbX7BBIgM5fAT9PD8TBG//Vu9THvj/ZomiVG2qv6RL0qQyVb+DUzPZz1amBsSvahtXCl72jA3JwAZ943RxSR66P934S0ashkVwLUi46z/EAbJ4QKBgQDojGIO07BEVL2+7VxlGL9XGZQp4Y3qlhh2zDDQGwkCq/KQ+BdNYWitPwqRl9GqFLgpmeQIhyHTOa/IThx+AXGKVQ24ROH+skUs4IbO6R3qY7BKtb5lkZE/Yln09x70BBngUYAzh/rtnsXO3cl1x2XDDqUbCwlGcDAs8Jh/6UnvQwKBgQDoVSQs7Uq9MJCGIUM2bixX89tHzSxq5mn9wMD3/XRVfT5Ua8YkYBuzcmlcT39N7L5BwuyFqX3Vi7lv/Ya/qaQP6XkrZ8W1OAaTlYewfE5ZgknJqSpXcNWhABKeNmqndvqyQ/8HNCv/j8AdraGB2DGO57Xso5J0CQ43W/U9+QIyjwKBgHLL2hw3o+wXaRO3WMUPUmVM2zdRgR0suybp5a7Vqb0H5NZrohUw4NulIzJ8H6Q2VjMzJL6Q9sGu2HepF6ecTtBa7ErqtiVlG4Dr1aCOs5XhYEWBMlwxX+JKSt4Cn+UVoTB7Cy5lEhn7JurX0Xuy0ylXMWoIKKv89cs5eg6quzTBAoGAaq9eEztLjKCWXOE9SetBdYnG8aunb9cqaJlwgu/h0bfXPVDYBbAUSEyLURY4MQI7Q1tM3Pu9iqfEmUZj7/LoIV5mg6X9RX/alT6etk3+dF+9nlqN1OU9U9cCtZ/rTcb2y5EptJcidRH/eCFY/pTV/PcttOJPx/S4kHcroC+N8MUCgYEA6DA5QHxHfNN6Nxv+pEzy2DIxFe9RrBxS+KPBsra1C8jgdeMf4EmfU0Nox92V0q0bRrD5ztqQwSONI0hSRb1iiMWR6MuFnAFajUJfASjjIlZ6nIQjQslI7vjlvYyyHS/p/Codxap+yJlTLWwVEOXp2D9pWwiMq1xEyf0TH1BosvM=";
	size_t decoded_len = base64_decode_length(priv_b64, strlen(priv_b64));
	char* out_buff = malloc(sizeof(char) * decoded_len);
	base64_decode(priv_b64, strlen(priv_b64), out_buff, decoded_len, &decoded_len);
	for (int i = 0; i < decoded_len; i++) {
		printf("%hhX ", out_buff[i]);
	}
	// now test
	return 0;
	
}

#endif /* test_repo_identity_h */

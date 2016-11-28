#include "ipfs/datastore/ds_helper.h"

int test_ds_key_from_binary() {
	size_t original_incoming_length = 10;
	size_t incoming_length = original_incoming_length;

	unsigned char incoming[incoming_length + 5]; // give a little wiggle room
	unsigned char* ptrIncoming = &incoming[0];

	for(int i = 0; i < incoming_length; i++) {
		incoming[i] = i;
	}

	size_t outgoing_length = 100;
	char outgoing[outgoing_length];
	char* ptrOutgoing = &outgoing[0];

	memset(outgoing, 0, outgoing_length);

	int retVal = ipfs_datastore_helper_ds_key_from_binary(ptrIncoming, incoming_length, ptrOutgoing, outgoing_length, &outgoing_length);
	if (retVal == 0)
		return 0;

	// now undo it and see if we get the same thing back...
	retVal = ipfs_datastore_helper_binary_from_ds_key(ptrOutgoing, outgoing_length, incoming, incoming_length + 5, &incoming_length);
	if (retVal == 0)
		return 0;

	if (original_incoming_length != incoming_length)
		return 0;

	for(int i = 0; i < original_incoming_length; i++) {
		if (incoming[i] != i)
			return 0;
	}
	return 1;
}

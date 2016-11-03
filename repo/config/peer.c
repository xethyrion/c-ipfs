//
// NOTE: this should be moved to libp2p-peer when it is built

#include <stdio.h>

#include "ipfs/repo/config/peer.h"

/* from libp2p-crypto/rsa.go

func (pk *RsaPublicKey) Bytes() ([]byte, error) {
	// x509.MarshalPKIXPublicKey(byte[]) serialises a public key to DER-encoded PKIX format
 
	b, err := x509.MarshalPKIXPublicKey(pk.k)
	if err != nil {
		return nil, err
	}
	
	pbmes := new(pb.PublicKey)
	typ := pb.KeyType_RSA
	pbmes.Type = &typ
	pbmes.Data = b
	return proto.Marshal(pbmes)
}

*/

/*
// from libp2p-peer/peer.go
// IDFromPublicKey returns the Peer ID corresponding to pk
func IDFromPublicKey(pk ic.PubKey) (ID, error) {
	b, err := pk.Bytes()
	if err != nil {
		return "", err
	}
	hash := u.Hash(b)
	return ID(hash), nil
	}

*/


/***
 * public methods
 */

int repo_config_peer_id_from_public_key(char* public_key, char* id) {
	// convert to bytes
	// hash
	// return ID(hash)
	return 0;
}

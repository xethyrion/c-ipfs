#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ipfs/cid/cid.h"
#include "ipfs/path/path.h"
#define NAMESYS_C
#include "ipfs/namesys/namesys.h"

/* mpns (a multi-protocol NameSystem) implements generic IPFS naming.
 *
 * Uses several Resolvers:
 * (a) IPFS routing naming: SFS-like PKI names.
 * (b) dns domains: resolves using links in DNS TXT records
 * (c) proquints: interprets string as the raw byte data.
 *
 * It can only publish to: (a) IPFS routing naming.
*/

mpns **ns;
// NewNameSystem will construct the IPFS naming system based on Routing
/*
func NewNameSystem(r routing.ValueStore, ds ds.Datastore, cachesize int) NameSystem {
	return &mpns{
		resolvers: map[string]resolver{
			"dns":      newDNSResolver(),
			"proquint": new(ProquintResolver),
			"dht":      NewRoutingResolver(r, cachesize),
		},
		publishers: map[string]Publisher{
			"/ipns/": NewRoutingPublisher(r, ds),
		},
	}
}*/

const DefaultResolverCacheTTL = time.Minute;

// Resolve implements Resolver.
int Resolve(char **path, char *name)
{
    return ResolveN(path, name, DefaultDepthLimit);
}

// ResolveN implements Resolver.
int ResolveN(char **path, char *name, int depth)
{
    char ipfs_prefix[] = "/ipfs/";
    char p[500];
    char *ps[] = {"/ipns/", NULL};
    int err;
    resolver r;

    r.resolveOnce = resolveOnce;

    if (memcmp(name, ipfs_prefix, strlen(ipfs_prefix)) == 0) {
        ParsePath(p, name);
        *path = malloc(strlen(p) + 1);
        if (*p) {
            strcpy(*path, p);
        } else {
            err = ErrAllocFailed;
        }
        return err;
    }

    if (*name == '/') {
        int err;
        char *str = malloc(sizeof(ipfs_prefix) + strlen(name));
        if (!str) {
            return ErrAllocFailed;
        }
        strcpy(str, ipfs_prefix);
        strcat(str, name+1);     // ignore inital / from name, because ipfs_prefix already has it.
        err = ParsePath(p, str); // save return value.
        free (str);              // so we can free allocated memory before return.
        *path = malloc(strlen(p) + 1);
        if (*p) {
            strcpy(*path, p);
        } else {
            err = ErrAllocFailed;
        }
        return err;
    }

    return resolve(&r, path, name, depth, ps);
}

// resolveOnce implements resolver.
int resolveOnce (char **path, char *name)
{
    char ipns_prefix[] = "/ipns/";
    char *ptr = NULL;
    char **segs;
    int i, err = 0;

    if (!name) { // NULL pointer.
        return ErrNULLPointer;
    }

    if (memcmp (name, ipns_prefix, strlen(ipns_prefix)) == 0) { // prefix missing.
        ptr = malloc(strlen(name) + sizeof(ipns_prefix));
        if (!ptr) { // allocation fail.
            return ErrAllocFailed;
        }
        strcpy(ptr, ipns_prefix);
        strcat(ptr, name);
        segs = Segments(ptr);
        free (ptr);
    } else {
        segs = Segments(name);
    }

    if (!segs || SegmentsLength(segs) < 2) {
        //log.Warningf("Invalid name syntax for %s", name);
        return ErrResolveFailed;
    }

    for (i = 0 ; ns[i] ; i++) {
        char *p;
        //log.Debugf("Attempting to resolve %s with %s", segments[1], ns[i]->resolver->protocol);
        err = ns[i]->resolver->func(&p, segs[1]);
        if (!err) {
            if (SegmentsLength(segs) > 2) {
                *path = PathFromSegments(p, segs+2);
            } else {
                *path = p;
            }
            return 0;
        }
    }
    //log.Warningf("No resolver found for %s", name);
    return ErrResolveFailed;
}

// Publish implements Publisher
int Publish (char *proto, ciPrivKey name, char *value)
{
    int i;

    for (i = 0 ; ns[i] ; i++) {
        if (strcmp(ns[i]->Publisher->protocol, proto)==0) {
            return ns[i]->Publisher->func(name, value);
        }
    }
    return ErrPublishFailed;
}

int PublishWithEOL (char *proto, ciPrivKey name, char *value, time_t eol)
{
    int i;

    for (i = 0 ; ns[i] ; i++) {
        if (strcmp(ns[i]->Publisher->protocol, proto)==0) {
            return ns[i]->Publisher->func_eol(name, value, eol);
        }
    }
    return ErrPublishFailed;
}

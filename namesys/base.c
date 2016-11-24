#include <string.h>
#include <time.h>
#include "ipfs/cid/cid.h"
#include "ipfs/path/path.h"
#include "ipfs/namesys/namesys.h"

int resolve (resolver *r, char **p, char *str, int depth, char **prefixes)
{
    int err, i;
    char ipfs_prefix[] = "/ipfs/";

    for (;;) {
        err = r->resolveOnce(p, str);
        if (err) {
            //log.Warningf("Could not resolve %s", name);
            *p = NULL;
            return err;
        }
        //log.Debugf("Resolved %s to %s", name, p.String());
        if (memcmp(p, ipfs_prefix, strlen(ipfs_prefix)) == 0) {
            // we've bottomed out with an IPFS path
            return 0;
        }
        if (depth == 1) {
            return ErrResolveRecursion;
        }
        for (i = 0 ; prefixes[i] ; i++) {
            if (memcmp(*p, prefixes[i], strlen(prefixes[i])) == 0) {
                if (SegmentsLength(prefixes) == 1) {
                    str += strlen(prefixes[i]);
                }
                break;
            }
        }
        if ( !prefixes[i] ) {
            return 0;
        }
        if (depth > 1) {
            depth--;
        }
    }
}

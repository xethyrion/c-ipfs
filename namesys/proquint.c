#include <stdlib.h>
#include "ipfs/namesys/namesys.h"
#include "ipfs/cid/cid.h"
#include "ipfs/path/path.h"

// resolveOnce implements resolver. Decodes the proquint string.
int ProquintResolveOnce (char **p, char *name)
{
    int err = proquintIsProquint(name);

    if (err) {
        *p = NULL;
        err = ErrInvalidProquint;
    } else {
        err = pathFromString(p, proquintDecode(name));
    }
    return err;
}

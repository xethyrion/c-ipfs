#include "ipfs/cid/cid.h"
#include "ipfs/path/path.h"

Resolver* NewBasicResolver (DAGService *ds)
{
    Resolver *ret = malloc(sizeof(Resolver));
    if (!ret) return NULL;
    ret->DAG = ds;
    ret->ResolveOnce = ResolveSingle;
    return ret;
}

// SplitAbsPath clean up and split fpath. It extracts the first component (which
// must be a Multihash) and return it separately.
int SplitAbsPath (struct Cid* cid, char ***parts, char *fpath)
{
    *parts = Segments(fpath);

    if (strcmp (**parts, "ipfs") == 0) *parts++;

    // if nothing, bail.
    if (!**parts) return ErrNoComponents;

    // first element in the path is a cid
    cid_decode_from_string(**parts, strlen(**parts), cid);
    return 0;
}

// ResolvePath fetches the node for given path. It returns the last item
// returned by ResolvePathComponents.
int ResolvePath(Node **nd, Context ctx, Resolver *s, char *fpath)
{
    int err = IsValid(fpath);
    Node **ndd;

    if (err) {
        return err;
    }
    err = ResolvePathComponents(&ndd, ctx, s, fpath);
    if (err) {
        return err;
    }
    if (ndd == NULL) {
        return ErrBadPath;
    }
    while(*ndd) {
        *nd = *ndd;
        ndd++;
    }
    return 0;
}

int ResolveSingle(NodeLink **lnk, Context ctx, DAGService *ds, Node **nd, char *name)
{
    return ResolveLink(lnk, name);
}

// ResolvePathComponents fetches the nodes for each segment of the given path.
// It uses the first path component as a hash (key) of the first node, then
// resolves all other components walking the links, with ResolveLinks.
int ResolvePathComponents(Node ***nd, Context ctx, Resolver *s, char *fpath)
{
    int err;
    struct Cid h;
    char **parts;

    err = SplitAbsPath(&h, &parts, fpath);
    if (err) {
        return err;
    }

    //log.Debug("resolve dag get");
    //*nd = s->DAG.Get(ctx, h);
    //if (nd == DAG_ERR_VAL) {
    //   return DAG_ERR_VAL;
    //}

    return ResolveLinks(ctx, *nd, parts);
}

// ResolveLinks iteratively resolves names by walking the link hierarchy.
// Every node is fetched from the DAGService, resolving the next name.
// Returns the list of nodes forming the path, starting with ndd. This list is
// guaranteed never to be empty.
//
// ResolveLinks(nd, []string{"foo", "bar", "baz"})
// would retrieve "baz" in ("bar" in ("foo" in nd.Links).Links).Links
int ResolveLinks(Node ***result, Context ctx, Node *ndd, char **names)
{
    int err, idx = 0;
    NodeLink *lnk;
    Node *nd;

    *result = calloc (sizeof(Node*), SegmentsLength(names) + 1);
    if (!*result) {
        return -1;
    }
    memset (*result, NULL, sizeof(Node*) * (SegmentsLength(names)+1));

    *result[idx++] = ndd;
    nd = ndd; // dup arg workaround

    while (*names) {
        //TODO
        //var cancel context.CancelFunc
        //ctx, cancel = context.WithTimeout(ctx, time.Minute)
        //defer cancel()

        // for each of the path components
        err = ResolveLink(&lnk, *names);
        if (err) {
            char msg[51];
            *result[idx] = NULL;
            snprintf(msg, sizeof(msg), ErrPath[ErrNoLinkFmt], *names, nd->Cid);
            if (ErrPath[ErrNoLink]) {
                free(ErrPath[ErrNoLink]);
            }
            ErrPath[ErrNoLink] = malloc(strlen(msg) + 1);
            if (ErrPath[ErrNoLink]) {
                strcpy(ErrPath[ErrNoLink], msg);
            }
            free (*result);
            return ErrNoLink;
        }
        names++;
    }
    return 0;
}

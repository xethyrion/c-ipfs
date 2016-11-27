#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ipfs/cid/cid.h"
#include "ipfs/path/path.h"
#include "ipfs/namesys/namesys.h"

/*type LookupTXTFunc func(name string) (txt []string, err error)

// DNSResolver implements a Resolver on DNS domains
type DNSResolver struct {
	lookupTXT LookupTXTFunc
	// TODO: maybe some sort of caching?
	// cache would need a timeout
}

// NewDNSResolver constructs a name resolver using DNS TXT records.
func NewDNSResolver() Resolver {
	return &DNSResolver{lookupTXT: net.LookupTXT}
}

// newDNSResolver constructs a name resolver using DNS TXT records,
// returning a resolver instead of NewDNSResolver's Resolver.
func newDNSResolver() resolver {
	return &DNSResolver{lookupTXT: net.LookupTXT}
}

// Resolve implements Resolver.
func (r *DNSResolver) Resolve(ctx context.Context, name string) (path.Path, error) {
	return r.ResolveN(ctx, name, DefaultDepthLimit)
}

// ResolveN implements Resolver.
func (r *DNSResolver) ResolveN(ctx context.Context, name string, depth int) (path.Path, error) {
	return resolve(ctx, r, name, depth, "/ipns/")
}

type lookupRes struct {
	path  path.Path
	error error
}*/

// resolveOnce implements resolver.
// TXT records for a given domain name should contain a b58
// encoded multihash.
int DNSResolverResolveOnce (char **path, char *name)
{
    char **segments, *domain, *dnslink, buf[500], dlprefix[] = "_dnslink.";
    int p1[2], p2[2], r, c=2;
    struct pollfd event[2], *e;

    segments = SplitN(name, "/", 2);
    domain = segments[0];

    *path = NULL;

    if (!IsDomain(domain)) {
        return ErrInvalidDomain;
    }
    //log.Infof("DNSResolver resolving %s", domain);

    if (pipe(p1) || pipe(p2)) {
        return ErrPipe;
    }

    dnslink = malloc(strlen(domain) + sizeof(dlprefix));
    if (!dnslink) {
        return ErrAllocFailed;
    }
    strcpy (dnslink, dlprefix);
    strcat (dnslink, domain);

    r = fork();
    switch(r) {
        case -1:
            return ErrPipe;
        case 0: // child
            close(p1[STDIN_FILENO]); // we don't need to read at child process.
            return workDomain (p1[STDOUT_FILENO], r, domain);
    }
    close(p1[STDOUT_FILENO]); // we don't need to write at main process.
    r = fork();
    switch(r) {
        case -1:
            return ErrPipe;
        case 0: // child
            close(p2[STDIN_FILENO]); // we don't need to read at child process.
            return workDomain (p2[STDOUT_FILENO], r, dnslink);
    }
    close(p2[STDOUT_FILENO]); // we don't need to write at main process.

    memset(&event, 0, sizeof(struct pollfd));
    event[0].fd = p1[STDIN_FILENO];
    event[0].events = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    event[1].fd = p2[STDIN_FILENO];
    event[1].events = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    e = event;

    do {
        r = poll(e, c, -1);
        if (r == -1) {
            return ErrPoll;
        }
        for (r = 0 ; r < c ; r++) {
            if (e[r].revents & POLLIN) {
                r = read(e[r].fd, buf, sizeof(buf));
                if (r > 0) {
                    buf[r] = '\0';
                    *path = malloc(r+1);
                    if (*path) {
                        strcpy(*path, buf);
                    }
                } else if (r <= 0) {
                    return ErrPoll;
                }
            }
        }
        if (event[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
            event[0].events = 0;
            e++; c--;
            wait(&r);
        }
        if (event[1].revents & (POLLERR | POLLHUP | POLLNVAL)) {
            event[1].events = 0;
            c--;
            wait(&r);
        }
    } while (c); // wait for child process finish.

    if (!*path) {
        return ErrResolveFailed;
    }

    if (SegmentsLength (segments) > 1) {
        name = *path + strlen(*path) - 1;
        while (*name == '/') {
            *name-- = '\0';
        }
        name = *path;
        *path = PathFromSegments (name, segments+1);
        free (name);
        if (!*path) {
            return ErrResolveFailed;
        }
    }
    FreeSegments (&segments);
    return 0;
}

int workDomain (int output, DNSResolver *r, char *name)
{
    char **txt, *path;
    int i, err = r->lookupTXT(&txt, name);

    if (err) {
        return err;
    }

    for (i = 0 ; txt[i] ; i++) {
        err = parseEntry (&path, txt[i]);
        if (!err) {
            err = (write (output, path, strlen(path)) != strlen(path));
            free (path);
            if (err) {
                return ErrPipe;
            }
            return 0;
        }
    }
    return ErrResolveFailed;
}

int parseEntry (char **path, char *txt)
{
    char buf[500];
    int err;

    err = ParseCidToPath(buf, txt); // bare IPFS multihashes
    if (! err) {
        *path = malloc(strlen(buf) + 1);
        if (!*path) {
            return ErrAllocFailed;
        }
        strcpy(*path, buf);
        return 0;
    }
    return tryParseDnsLink(path, txt);
}

int tryParseDnsLink(char **path, char *txt)
{
    char **parts = SplitN(txt, "=", 2), buf[500];
    int err;

    if (SegmentsLength(parts) == 2 && strcmp(parts[0], "dnslink")==0) {
        err = ParsePath(buf, parts[1]);
        if (err == 0) {
            *parts = malloc(strlen(buf) + 1);
            if (! *parts) {
                return ErrAllocFailed;
            }
            strcpy(*parts, buf);
            return 0;
        }
        return err;
    }
    return ErrInvalidDNSLink;
}

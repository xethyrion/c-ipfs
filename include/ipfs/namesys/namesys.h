#ifndef NAMESYS_H
    #define NAMESYS_H

    #define DefaultDepthLimit 32

    #ifdef NAMESYS_C
        char *ErrNamesys[] = {
            NULL,
            "ErrAllocFailed",
            "ErrNULLPointer",
            "ErrPipe",
            "ErrPoll",
            "Could not publish name."
            "Could not resolve name.",
            "Could not resolve name (recursion limit exceeded).",
            "expired record",
            "unrecognized validity type",
            "not a valid proquint string",
            "not a valid domain name",
            "not a valid dnslink entry"
        };
    #else
        extern char *ErrNamesys;
    #endif // NAMESYS_C

    enum {
        ErrAllocFailed = 1,
        ErrNULLPointer,
        ErrPipe,
        ErrPoll,
        ErrPublishFailed,
        ErrResolveFailed,
        ErrResolveRecursion,
        ErrExpiredRecord,
        ErrUnrecognizedValidity,
        ErrInvalidProquint,
        ErrInvalidDomain,
        ErrInvalidDNSLink
    } NamesysErrs;

    typedef struct s_resolvers {
        char *protocol;
        int (*func)(char**, char*);
    } resolvers;

    typedef struct s_resolver {
        // resolveOnce looks up a name once (without recursion).
        int (*resolveOnce) (char **, char *);
    } resolver;

    //TODO ciPrivKey from c-libp2p-crypto
    typedef void* ciPrivKey;

    typedef struct s_publishers {
        char *protocol;
        int (*func) (ciPrivKey, char*);
        int (*func_eol) (ciPrivKey, char*, time_t);
    } publishers;

    typedef struct s_mpns {
        resolvers  *resolver;
        publishers *Publisher;
    } mpns;

    typedef struct s_tlds {
        char *str;
        int  condition;
    } tlds;

    int resolve (resolver *r, char **p, char *str, int depth, char **prefixes);
    int Resolve(char **path, char *name);
    int ResolveN(char **path, char *name, int depth);
    int resolveOnce (char **path, char *name);
    int Publish (char *proto, ciPrivKey name, char *value);
    int PublishWithEOL (char *proto, ciPrivKey name, char *value, time_t eol);

    int ProquintResolveOnce (char **p, char *name);

    int domainMatchString (char *d);
    int IsICANNTLD(char *s);
    int IsExtendedTLD (char *s);
    int IsTLD (char *s);
    int IsDomain (char *s);

    int DNSResolverResolveOnce (DNSResolver *r, char **path, char *name);
    int workDomain (int output, DNSResolver *r, char *name);
    int parseEntry (char **Path, char *txt);
    int tryParseDnsLink (char **Path, char *txt);
#endif //NAMESYS_H

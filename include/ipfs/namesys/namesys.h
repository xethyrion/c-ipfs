#ifndef NAMESYS_H
    #define NAMESYS_H

    #define DefaultDepthLimit 32

    char *ErrNamesys[] = {
        NULL,
        "ErrAllocFailed",
        "ErrNULLPointer",
        "Could not publish name."
        "Could not resolve name.",
        "Could not resolve name (recursion limit exceeded).",
        "expired record",
        "unrecognized validity type",
        "not a valid proquint string"
    };

    enum {
        ErrAllocFailed = 1,
        ErrNULLPointer,
        ErrPublishFailed,
        ErrResolveFailed,
        ErrResolveRecursion,
        ErrExpiredRecord,
        ErrUnrecognizedValidity,
        ErrInvalidProquint
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

    int resolve (resolver *r, char **p, char *str, int depth, char **prefixes);
    int Resolve(char **path, char *name);
    int ResolveN(char **path, char *name, int depth);
    int resolveOnce (char **path, char *name);
    int Publish (char *proto, ciPrivKey name, char *value);
    int PublishWithEOL (char *proto, ciPrivKey name, char *value, time_t eol);
#endif //NAMESYS_H

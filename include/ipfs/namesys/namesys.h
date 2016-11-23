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
        "unrecognized validity type"
    };

    enum {
        ErrAllocFailed = 1,
        ErrNULLPointer,
        ErrPublishFailed,
        ErrResolveFailed,
        ErrResolveRecursion,
        ErrExpiredRecord,
        ErrUnrecognizedValidity
    } NamesysErrs;

    typedef struct s_resolvers {
        char *protocol;
        int (*func)(char**, char*);
    } resolvers;

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
#endif //NAMESYS_H

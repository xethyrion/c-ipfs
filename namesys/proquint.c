#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ipfs/namesys/namesys.h"
#include "ipfs/cid/cid.h"
#include "ipfs/path/path.h"

const uint8_t  conse[] = {'b', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'r', 's', 't', 'v', 'z'};
const uint8_t  vowse[] = {'a', 'i', 'o', 'u'};

// Find decoded number from the encoded consonant.
static inline int consd(char c)
{
    int i;

    for (i = 0 ; i < sizeof(conse) ; i++) {
        if (c == conse[i]) {
            return i;
        }
    }

    return 0;
}

// Find decoded number of encoded vowel.
static inline int vowsd(char c)
{
    int i;

    for (i = 0 ; i < sizeof(vowse) ; i++) {
        if (c == vowse[i]) {
            return i;
        }
    }

    return 0;
}

/**
* Tests if a given string is a Proquint identifier
*
* @param {string} str The candidate string.
*
* @return {bool} Whether or not it qualifies.
* @return {error} Error
*/
int ProquintIsProquint(char *str)
{
    int i, c;

    // if str is null, or length isn't 11 or don't have - at middle
    if (!str || strlen(str) != 11 || str[5] != '-') {
        return 0; // it's not a proquint
    }

    // run every position
    for (i = 0 ; i < 11 ; i++) {
        if (i == 5) i++; // skip -, already tested.
        switch (i) {
            case 1:
            case 3:
            case 7:
            case 9:
                // compare with vowse array
                c = vowsd(str[i]);
                if (str[i] != vowse[c]) {
                    return 0; // it's not a proquint
                }
                break;
            default: // 0,2,4,6,8,10
                // compare with conse array
                c = consd(str[i]);
                if (str[i] != conse[c]) {
                    return 0; // it's not a proquint
                }
        }
    }

    return 1; // passed on every value.
}

/**
* Encodes an arbitrary byte slice into an identifier.
*
* @param {[]byte} buf Slice of bytes to encode.
*
* @return {string} The given byte slice as an identifier.
*/
char *ProquintEncode(char *buf)
{
    char *ret;
    int i, c;
    uint16_t n;

    if (!buf) {
        return NULL;
    }

    ret = malloc(12);
    if (!ret) {
        return NULL;
    }

    for (i = 0, c = 0; i < 4; i += 2) {
        n = ((buf[i] & 0xff) << 8) | (buf[i + 1] & 0xff);

        ret[c++] = conse[(n >> 12) & 0x0f];
        ret[c++] = vowse[(n >> 10) & 0x03];
        ret[c++] = conse[(n >> 6)  & 0x0f];
        ret[c++] = vowse[(n >> 4)  & 0x03];
        ret[c++] = conse[n         & 0x0f];
        ret[c++] = '-';
    }
    ret[--c] = '\0';

    return ret;
}

/**
* Decodes an identifier into its corresponding byte slice.
*
* @param {string} str Identifier to convert.
*
* @return {[]byte} The identifier as a byte slice.
*/
char *ProquintDecode(char *str)
{
    char *ret;
    int i, c;
    uint16_t x;

    // make sure its a valid Proquint string.
    if (!ProquintIsProquint(str)) {
        return NULL;
    }

    ret = malloc(4);
    if (!ret) {
        return NULL;
    }

    for (i = 0, c = 0 ; i < 11 ; i += 6) {
        x =(consd(str[i + 0]) << 12) | \
           (vowsd(str[i + 1]) << 10) | \
           (consd(str[i + 2]) <<  6) | \
           (vowsd(str[i + 3]) <<  4) | \
           (consd(str[i + 4]) <<  0);

        ret[c++] = x >> 8;
        ret[c++] = x & 0xff;
    }

    return ret;
}

// resolveOnce implements resolver. Decodes the proquint string.
int ProquintResolveOnce (char **p, char *name)
{
    int err = ProquintIsProquint(name);
    char buf[500];

    if (err) {
        *p = NULL;
        err = ErrInvalidProquint;
    } else {
        err = ParsePath(buf, ProquintDecode(name));
        if (!err) {
            *p = malloc (strlen(buf) + 1);
            if (p) {
                strcpy(*p, buf);
            }
        }
    }
    return err;
}

#include <string.h>
#include <ctype.h>
#include <time.h>
#include "ipfs/namesys/namesys.h"
#include "ipfs/namesys/isdomain.h"

void ToUpper(char *dst, char *src)
{
    while(*src) {
        *dst++ = toupper(*src++);
    }
    *dst = '\0';
}

int HasSuffix (char *s, char *suf)
{
    char *p;

    p = s + strlen(s) - strlen(suf);
    return strcmp(p, suf) == 0;
}

int IsAtArray(tlds *a, char *s)
{
    char str[strlen(s)+1];

    ToUpper(str, s);
    while(a->str) {
        if (strcmp(a->str, str) == 0) {
            return a->condition;
        }
        a++;
    }
    return 0;
}

int domainMatchString (char *d)
{
    char str[strlen(d)+1], *p = str, *l;

    ToUpper(str, d);

    // l point to last two chars.
    l = p + strlen(p) - 2;

    // can't start with a dot
    if (*p == '.') {
        return 0; // invalid
    }

    // last 2 chars can't be a dot or a number.
    if ((*l >= 'A' && *l <= 'Z') && (l[1] >= 'A' && l[1] <= 'Z')) {
        while (*p) {
            if ((*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '.' || *p == '-') {
                p++;
            } else {
                return 0; // invalid
            }
        }
    } else {
        return 0; // invalid
    }

    return 1; // valid
}

// IsICANNTLD returns whether the given string is a TLD (Top Level Domain),
// according to ICANN. Well, really according to the TLDs listed in this
// package.
int IsICANNTLD(char *s)
{
    return IsAtArray (TLDs, s);
}

// IsExtendedTLD returns whether the given string is a TLD (Top Level Domain),
// extended with a few other "TLDs", .bit, .onion
int IsExtendedTLD (char *s)
{
    return IsAtArray (ExtendedTLDs, s);
}

// IsTLD returns whether the given string is a TLD (according to ICANN, or
// in the set of ExtendedTLDs listed in this package.
int IsTLD (char *s)
{
    return IsICANNTLD (s) || IsExtendedTLD(s);
}

// IsDomain returns whether given string is a domain.
// It first checks the TLD, and then uses a regular expression.
int IsDomain (char *s)
{
    char str[strlen(s)];
    char *tld;

    strcpy(str, s);
    s = str; // work with local copy.

    if (HasSuffix (s, ".")) {
        s[strlen(s) - 1] = '\0';
    }

    tld = strrchr(s, '.');

    if (!tld) { // don't have a dot.
        return 0;
    }

    tld++; // ignore last dot

    if (!IsTLD (tld)) {
        return 0;
    }

    return domainMatchString(s);
}

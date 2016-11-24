#include <stdio.h>
#include <time.h>
#include "ipfs/namesys/namesys.h"

int main(int argc, char **argv)
{
    int i;
    tlds chk[] = {
		{ "foo.bar.baz.com",  1 },
		{ "foo.bar.baz",      0 },
		{ "foo.bar.baz.com.", 1 },
		{ "com",              0 }, // yeah yeah...
		{ ".",                0 }, // yeah yeah...
		{ "..",               0 },
		{ ".foo.com.",        0 },
		{ ".foo.com",         0 },
		{ "fo o.com",         0 },
		{ "example.com",      1 },
		{ "fjdoisajfdiosafdsa8fd8saf8dsa8fdsafdsa-fd-sa-fd-saf-dsa.org",   1 },
		{ "fjdoisajfdiosafdsa8fd8saf8dsa8fdsafdsa-fd-sa-fd-saf-dsa.bit",   1 },
		{ "fjdoisajfdiosafdsa8fd8saf8dsa8fdsafdsa-fd-sa-fd-saf-dsa.onion", 1 },
		{ "a.b.c.d.e.f.g.h.i.j.k.l.museum",                                1 },
		{ "a.b.c.d.e.f.g.h.i.j.k.l",                                       0 },
		{ NULL,                                                            0 }
	};

    for (i = 0 ; chk[i].str ; i++) {
        if (IsDomain (chk[i].str) != chk[i].condition) {
            printf ("Misclassification: %s should be %d\n", chk[i].str, chk[i].condition);
        }
    }
    return 0;
}

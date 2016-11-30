#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int ProquintIsProquint(char *str);
char *ProquintEncode(char *buf);
char *ProquintDecode(char *str);

int main(void) {
    char *r, *s;
    int i;
    char *p[] = {"lusab-babad", "gutih-tugad", "gutuk-bisog", "mudof-sakat",
                 "haguz-biram", "mabiv-gibot", "natag-lisaf", "tibup-zujah",
                 "tobog-higil", "todah-vobij", "sinid-makam", "budov-kuras",
                 NULL};

    for (i = 0 ; p[i] ; i++) {
        r = ProquintDecode (p[i]);
        if (r) {
            struct in_addr ip_addr;
            memcpy (&(ip_addr.s_addr), r, sizeof(ip_addr.s_addr));
            printf ("%s\t%s", p[i], inet_ntoa(ip_addr));
            s = ProquintEncode(r);
            free (r);
            if (s) {
                printf ("\t%s", s);
                free (s);
            }
            printf("\n");
        }
    }
    return 0;
}

#include <string.h>
#include <stdlib.h>
#include <ipfs/cid/cid.h>
#include <ipfs/path/path.h>

// FromCid safely converts a cid.Cid type to a Path type
char* PathFromCid (struct Cid *c)
{
   const char prefix[] = "/ipfs/";
   char *rpath, *cidstr = CidString(c);

   rpath = malloc(sizeof(prefix) + strlen(cidstr));
   if (!rpath) return NULL;
   strcpy(rpath, prefix);
   strcat(rpath, cidstr);
   return rpath;
}

char** SplitN (char *p, char *delim, int n)
{
   char *c, **r, *rbuf;
   int i, dlen = strlen(delim);

   if (n == 0) {
      return NULL; // no split?
   }

   if (n < 0) { // negative, count all delimiters + 1.
      for (c = p , n = 0 ; c ; n++) {
         c = strstr(c, delim);
         if (c) {
            c += dlen;
         }
      }
   } else {
      n++; // increment param value.
   }

   rbuf = malloc(strlen(p) + 1);
   if (!rbuf) {
      return NULL;
   }

   r = calloc(sizeof(char*), n + 1); // splits plus NULL pointer termination
   if (!r) {
      free(r);
      return NULL;
   }

   strcpy(rbuf, p); // keep original
   for (c = rbuf, i = 0 ; i < n && c ; i++) {
      r[i] = c;
      c = strstr(c, delim);
      if (c) {
         *c = '\0';
         c += dlen;
      }
   }
   r[i] = NULL;

   return r;
}

char** Segments (char *p)
{
   if (*p == '/') p++; // Ignore leading slash

   return SplitN (p, "/", -1);
}

// Count Segments
int SegmentsLength (char **s)
{
   int r = 0;

   if (s) {
      while (s[r]) r++;
   }

   return r;
}

// free memory allocated by Segments
void FreeSegments (char ***s)
{
   if (*s && **s) {
      free(**s); // free string buffer
      free(*s); // free array
      *s = NULL;
   }
}

// IsJustAKey returns true if the path is of the form <key> or /ipfs/<key>.
int IsJustAKey (char *p)
{
   char **parts;
   int ret = 0;
   parts = Segments (p);
   if (parts) {
      if (SegmentsLength (parts) == 2 && strcmp (parts[0], "ipfs") == 0) ret++;
      FreeSegments(&parts);
   }
   return ret;
}

// PopLastSegment returns a new Path without its final segment, and the final
// segment, separately. If there is no more to pop (the path is just a key),
// the original path is returned.
int PopLastSegment (char **str, char *p)
{
   if (IsJustAKey(p)) return 0;
   *str = strrchr(p, '/');
   if (!*str) return ErrBadPath; // error
   **str = '\0';
   *str++;
   return 0;
}

char *PathFromSegments(char *prefix, char **seg)
{
   int retlen, i;
   char *ret;

   if (!prefix || !seg) return NULL;

   retlen = strlen(prefix);
   for (i = 0 ; seg[i] ; i++) {
      retlen += strlen(seg[i]) + 1; // count each segment length + /.
   }

   ret = malloc(retlen + 1); // allocate final string size + null terminator.
   if (!ret) return NULL;

   strcpy(ret, prefix);
   for (i = 0 ; seg[i] ; i++) {
      strcat(ret, "/");
      strcat(ret, seg[i]);
   }
   return ret;
}

int ParseCidToPath (char *dst, char *txt)
{
   struct Cid *c;
   char *r;

   if (!txt || txt[0] == '\0') return ErrNoComponents;

   c = cidDecode(txt);

   if (!c) {
      return ErrCidDecode;
   }

   r = PathFromCid(c);

   if (!r) {
      return ErrCidDecode;
   }
   strcpy (dst, r);
   free (r);
   return 0;
}

int ParsePath (char *dst, char *txt)
{
   int err, i;
   char *c;
   const char prefix[] = "/ipfs/";
   const int plen = strlen(prefix);

   if (!txt || txt[0] == '\0') return ErrNoComponents;

   if (*txt != '/' || strchr (txt+1, '/') == NULL) {
      if (*txt == '/') {
         txt++;
      }
      err = ParseCidToPath (dst+plen, txt);
      if (err == 0) { // only change dst if ParseCidToPath returned success.
         // Use memcpy instead of strcpy to avoid overwriting
         // result of ParseCidToPath with a null terminator.
         memcpy (dst, prefix, plen);
      }
      return err;
   }

   c = txt;
   for (i = 0 ; (c = strchr(c, '/')) ; i++) c++;
   if (i < 3) return ErrBadPath;

   if (strcmp (txt, prefix) == 0) {
      char buf[strlen(txt+5)];
      strcpy (buf, txt+6); // copy to temp buffer.
      c = strchr(buf, '/');
      if (c) *c = '\0';
      return ParseCidToPath(dst, buf);
   } else if (strcmp (txt, "/ipns/") != 0) {
      return ErrBadPath;
   }
   return 0;
}

int PathIsValid (char *p)
{
   char buf[4096];
   return ParsePath(buf, p);
}

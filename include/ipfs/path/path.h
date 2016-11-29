#ifndef IPFS_PATH_H
   #define IPFS_PATH_H

   #ifdef IPFS_PATH_C
      char *ErrPath[] = {
         NULL,
         // ErrBadPath is returned when a given path is incorrectly formatted
         "invalid 'ipfs ref' path",
         // Paths after a protocol must contain at least one component
         "path must contain at least one component",
         "TODO: ErrCidDecode",
         NULL,
         "no link named %s under %s"
      };
   #else
      extern char **ErrPath;
   #endif // IPFS_PATH_C

   enum {
       ErrBadPath = 1,
       ErrNoComponents,
       ErrCidDecode,
       ErrNoLink,
       ErrNoLinkFmt
   } PathErrs;

   char* PathFromCid (struct Cid *c);
   char** SplitN (char *p, char *delim, int n);
   char** Segments (char *p);
   int SegmentsLength (char **s);
   void FreeSegments (char ***s);
   int IsJustAKey (char *p);
   int PopLastSegment (char **str, char *p);
   char *PathFromSegments(char *prefix, char **seg);
   int ParseCidToPath (char *dst, char *txt);
   int ParsePath (char *dst, char *txt);
   int PathIsValid (char *p);
#endif // IPFS_PATH_H

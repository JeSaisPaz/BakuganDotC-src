// bdc 0x08a19744 GmoModelPtrTablesRelease
#include "bdc.h"

/* Drops a reference on the GmoPtrTables block a GMO model holds at `model+0x54`: at zero frees,
   for each of its five pointer arrays, every block it points to and the array itself, then the
   block (`GmoHeapReleaseThunk`). Returns `tables`. Called by `GmoModelDestroyContents` right
   after `GmoDlCacheRelease`. */

void *GmoModelPtrTablesRelease(void *tables)

{
  GmoPtrTables *t = (GmoPtrTables *)tables;
  int i;
  int j;

  if (t != NULL) {
    t->refCount = (s16)((u16)t->refCount - 1);
    if ((u16)t->refCount == 0) {
      for (i = 0; i < 5; i++) {
        for (j = 0; j < t->counts[i]; j++) {
          GmoHeapReleaseThunk(0, t->arrays[i][j]);
        }
        GmoHeapReleaseThunk(0, t->arrays[i]);
      }
      GmoHeapReleaseThunk(0, tables);
    }
  }
  return tables;
}

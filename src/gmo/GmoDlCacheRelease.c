// bdc 0x08a19690 GmoDlCacheRelease
#include "bdc.h"

/* Drops a reference on a model's display-list cache (`model+0x50`): at zero frees the per-entry
   blocks of its patch table (0xc-byte entries at `+4`, count `+0xe`), the list copies (`+8`, pool
   1), the table and the object (`GmoHeapRelease` via its thunk). Returns `cache`. */

void *GmoDlCacheRelease(void *cache)
{
    GmoDlCache *c = cache;
    int i;

    if (c != NULL && --c->refCount == 0) {
        for (i = 0; i < c->entryCount; i++) {
            GmoHeapReleaseThunk(0, c->entries[i].block);
        }
        GmoHeapReleaseThunk(1, c->lists);
        GmoHeapReleaseThunk(0, c->entries);
        GmoHeapReleaseThunk(0, c);
    }
    return c;
}

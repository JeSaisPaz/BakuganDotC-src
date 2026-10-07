// bdc 0x089be178 CorePackChainFindSize
#include "bdc.h"

/* Returns the `size` of the first directory entry named `name` in the pack chain, or 0 when
   absent; companion of `CorePackChainFind` which returns the data pointer. */
u32 CorePackChainFindSize(void *pack, char *name)
{
    const CorePack *p;
    CorePackDirEntry *entry;

    for (p = pack; p != NULL; p = (const CorePack *)p->base.next) {
        entry = CorePackDirFind(p->dir, name);
        if (entry != NULL)
            return entry->size;
    }
    return 0;
}

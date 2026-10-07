// bdc 0x089be1e8 CorePackChainFindEntry
#include "bdc.h"

/* Returns the directory entry named `name` from the first pack in the chain that has it
   (skipping packs without a directory), or NULL. */
void *CorePackChainFindEntry(void *pack, char *name)
{
    const CorePack *p;
    CorePackDirEntry *entry;

    for (p = pack; p != NULL; p = (const CorePack *)p->base.next) {
        if (p->dir == NULL)
            continue;
        entry = CorePackDirFind(p->dir, name);
        if (entry != NULL)
            return entry;
    }
    return NULL;
}

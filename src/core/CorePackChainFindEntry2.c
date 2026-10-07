// bdc 0x089be258 CorePackChainFindEntry2
#include "bdc.h"

/* Second compiled copy of `CorePackChainFindEntry`: returns the directory entry named `name`
   from the first pack in the chain that has it (skipping packs without a directory), or NULL. */
void *CorePackChainFindEntry2(void *pack, char *name)
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

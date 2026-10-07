// bdc 0x089be108 CorePackChainFindData
#include "bdc.h"

/* Byte-identical copy of `CorePackChainFind`: returns the `data` pointer of the first
   directory entry named `name` in the pack chain, or NULL. */
void *CorePackChainFindData(void *pack, char *name)
{
    const CorePack *p;
    CorePackDirEntry *entry;

    for (p = pack; p != NULL; p = (const CorePack *)p->base.next) {
        entry = CorePackDirFind(p->dir, name);
        if (entry != NULL)
            return entry->data;
    }
    return NULL;
}

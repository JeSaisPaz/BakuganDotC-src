// bdc 0x089be014 CorePackChainFind
#include "bdc.h"

/* Finds a resource by name in the chain of loaded resource packs (`CorePack`): searches each
   pack's directory with `CorePackDirFind`, following `next` to the end, and returns the `data`
   pointer of the first match, or NULL. */
void *CorePackChainFind(void *packChain, char *name)
{
    const CorePack *p;
    CorePackDirEntry *entry;

    for (p = packChain; p != NULL; p = (const CorePack *)p->base.next) {
        entry = CorePackDirFind(p->dir, name);
        if (entry != NULL)
            return PspPtr(entry->data);
    }
    return NULL;
}

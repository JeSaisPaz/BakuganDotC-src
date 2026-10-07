// bdc 0x089be084 CorePackChainFindNamed
#include "bdc.h"

/* Looks `name` up in each pack of the chain starting at `pack` (`CorePackDirFind` on `+0x24`,
   following `+4`) and returns the first match's data pointer (entry `+4`), storing a pointer to the
   entry's stored name (entry `+0x10`) in `*outName`; returns 0 if not found. */
void *CorePackChainFindNamed(CorePack *pack, char *name, char **outName)
{
    for (; pack != NULL; pack = (CorePack *)pack->base.next) {
        CorePackDirEntry *entry = CorePackDirFind(pack->dir, name);

        if (entry != NULL) {
            *outName = entry->name;
            return entry->data;
        }
    }
    return NULL;
}

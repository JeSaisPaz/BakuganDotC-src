// bdc 0x089be300 CorePackGetEntrySize
#include "bdc.h"

/* Returns the size of directory entry `index` of `pack`'s directory, or 0 when the pack has no
   directory or `index` is out of range. */
u32 CorePackGetEntrySize(CorePack *pack, int index)
{
    CorePackDirEntry *dir = pack->dir;

    if (dir != NULL && dir->count != 0 && index >= 0 && index < (int)dir->count) {
        return dir[index].size;
    }
    return 0;
}

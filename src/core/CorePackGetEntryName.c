// bdc 0x089be338 CorePackGetEntryName
#include "bdc.h"

/* Returns the name (entry `+0x10`) of directory entry `index` of `pack`'s directory, or the empty
   string at `0x08aa0488` when out of range. */
char *CorePackGetEntryName(CorePack *pack, int index)
{
    CorePackDirEntry *dir = pack->dir;

    if (dir != NULL && dir->count != 0 && index >= 0 && index < (int)dir->count) {
        return dir[index].name;
    }
    return "";
}

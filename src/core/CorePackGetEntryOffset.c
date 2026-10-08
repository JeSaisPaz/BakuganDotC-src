// bdc 0x089be2c8 CorePackGetEntryOffset
#include "bdc.h"

/* Returns the `data` field (file offset, or pointer once relocated) of directory entry `index` of the package
   `pack` (`CorePack` `dir`), or NULL when the package has no directory or `index` is out of range.
    */
void *CorePackGetEntryOffset(CorePack *pack, int index)
{
    CorePackDirEntry *dir = pack->dir;

    if (dir != NULL && dir->count != 0 && index >= 0 && index < (int)dir->count) {
        return PspPtr(dir[index].data);
    }
    return NULL;
}

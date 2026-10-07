// bdc 0x089bd710 CorePackDirFind
#include "bdc.h"

/* Looks `name` up in a resource-pack directory and returns its directory entry (NULL if absent).
   The comparison is case-insensitive (any character above `0x60` is reduced by `0x20`); entries of
   type `0x78` (`'x'`) are sub-directories searched recursively through their `data` pointer. */
CorePackDirEntry *CorePackDirFind(CorePackDirEntry *dir, char *name)
{
    CorePackDirEntry *found;
    s32 i;
    s32 k;
    s32 a;
    s32 b;
    s32 mismatch;

    for (i = 0; i < (s32)dir[0].count; i++) {
        mismatch = 0;
        for (k = 0;; k++) {
            a = (s8)dir[i].name[k];
            b = (s8)name[k];
            if (a == 0) {
                mismatch = (b != 0);
                break;
            }
            if (a > 0x60) {
                a = (s8)(a - 0x20);
            }
            if (b > 0x60) {
                b = (s8)(b - 0x20);
            }
            if (a != b) {
                mismatch = 1;
                break;
            }
        }
        if (!mismatch) {
            return &dir[i];
        }
        if (dir[i].type == 0x78) {
            found = CorePackDirFind((CorePackDirEntry *)dir[i].data, name);
            if (found != NULL) {
                return found;
            }
        }
    }
    return NULL;
}

// bdc 0x0893aa40 UiUnlockResultMapFigureIndex
#include "bdc.h"

/* Looks up `index` in one of two byte tables of `UiUnlockResult`: table 0 (21
   bytes, `0x08ac19f4`) or table 1 (20 bytes, `0x08ac1a09`). */

u8 UiUnlockResultMapFigureIndex(UiUnlockResult *self, u8 table, u8 index)
{
    static const u8 kTable0[21] = {255, 0, 18, 3, 15, 1, 2, 5, 4, 9, 19, 11, 6, 10, 7, 8, 12, 13, 14, 17, 16};
    static const u8 kTable1[20] = {1, 5, 6, 3, 8, 7, 12, 14, 15, 9, 13, 11, 16, 17, 18, 4, 20, 19, 2, 10};
    u8 t0[24];
    u8 t1[20];

    memcpy(t0, kTable0, sizeof(kTable0));
    memcpy(t1, kTable1, sizeof(kTable1));
    if (table == 0) {
        return t0[index];
    }
    return t1[index];
}

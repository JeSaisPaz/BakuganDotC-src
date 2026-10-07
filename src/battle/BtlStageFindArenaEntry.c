// bdc 0x0889d2b8 BtlStageFindArenaEntry
#include "bdc.h"

/* Returns the `BtlArenaEntry` of `g_btlArenaTable` whose id is `arena` (the current arena
   `g_btlArenaIndex` when `arena == -1`), or NULL when none of the first
   `BtlStageGetArenaCount` entries has that id (the count is re-read every iteration).
   Used by `BtlStageGetArenaDataB` and `BtlStageLoadMap`. */
BtlArenaEntry *BtlStageFindArenaEntry(s32 arena)
{
    BtlArenaEntry *entry = g_btlArenaTable;
    int i;

    if (arena == -1) {
        arena = g_btlArenaIndex;
    }
    for (i = 0; i < BtlStageGetArenaCount(); i++, entry++) {
        if (entry->id == arena) {
            return entry;
        }
    }
    return NULL;
}

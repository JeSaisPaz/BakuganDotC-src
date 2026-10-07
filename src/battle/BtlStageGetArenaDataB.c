// bdc 0x0889d324 BtlStageGetArenaDataB
#include "bdc.h"

/* Returns the `mapPath` of the `BtlArenaEntry` whose id is `arena` (-1 = current arena), found by
   `BtlStageFindArenaEntry`; the entry is dereferenced without a NULL check.
   Used by `BtlMainPhaseLoad`. */
const char *BtlStageGetArenaDataB(s32 arena)
{
    return BtlStageFindArenaEntry(arena)->mapPath;
}

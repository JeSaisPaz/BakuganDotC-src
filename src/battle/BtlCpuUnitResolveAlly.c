// bdc 0x08899a90 BtlCpuUnitResolveAlly
#include "bdc.h"

/* Resolves a CPU unit's ally: when `allyIndex` is not -1, stores the `allyIndex`-th crystal
   (`BtlGetNthCrystal`, NULL when there are fewer) in `ally`, which the AI follow layer
   (`BtlAiFollowLayerCheck`) takes as its leader; otherwise `ally` is left unchanged. */
void BtlCpuUnitResolveAlly(BtlCpuUnit *unit)
{
    if (unit->allyIndex != -1) {
        unit->ally = BtlGetNthCrystal(unit->allyIndex);
    }
}

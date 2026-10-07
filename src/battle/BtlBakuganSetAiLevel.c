// bdc 0x0889ac74 BtlBakuganSetAiLevel
#include "bdc.h"

/* Sets a CPU unit's AI skill level: stores `level` clamped to `0..9` in `BtlCpuUnit``.aiLevel`
   and, when the unit has an AI object (`ai`), passes it the unclamped `level` truncated to `u8`
   through `BtlAiSetLevel`. */
void BtlBakuganSetAiLevel(BtlBakugan *self, int level)
{
    BtlCpuUnit *unit = (BtlCpuUnit *)self;
    BtlAi *ai = unit->ai;

    if (level < 0) {
        unit->aiLevel = 0;
    } else if (level > 9) {
        unit->aiLevel = 9;
    } else {
        unit->aiLevel = level;
    }
    if (ai != NULL) {
        BtlAiSetLevel(ai, (u8)level);
    }
}

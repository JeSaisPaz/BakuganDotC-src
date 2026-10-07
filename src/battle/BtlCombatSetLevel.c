// bdc 0x08888a24 BtlCombatSetLevel
#include "bdc.h"

/* Sets the stat level (`combat->level`, `+0xac`) of a `BtlCombatState` to `level` clamped to 0..9,
   recomputes maximum HP with `BtlCombatComputeMaxHp`, stores it in both `maxHp` (`+0x94`) and `hp`
   (`+0x98`, i.e. the unit is refilled) and, when the owner unit exists and has an HP gauge (`hpGauge`,
   `owner+0x554`), snaps the gauge to the new `maxHp` with `UiHpGaugeSetValues`. */

void BtlCombatSetLevel(BtlCombatState *combat, int level)
{
    BtlBakugan *owner;
    float maxHp;

    if (level < 0) {
        level = 0;
    } else if (level > 9) {
        level = 9;
    }
    combat->level = level;
    maxHp = BtlCombatComputeMaxHp(combat);
    combat->maxHp = maxHp;
    owner = (BtlBakugan *)combat->owner;
    combat->hp = maxHp;
    if (owner != NULL && owner->hpGauge != NULL) {
        UiHpGaugeSetValues(combat->maxHp, (UiHpGauge *)owner->hpGauge);
    }
}

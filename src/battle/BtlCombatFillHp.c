// bdc 0x08886c18 BtlCombatFillHp
#include "bdc.h"

/* Writes `value` to both `maxHp` and `hp` of a `BtlCombatState` and, when the owner unit exists
   and has an HP gauge, snaps the gauge to the new `maxHp` (`UiHpGaugeSetValues`). */

void BtlCombatFillHp(BtlCombatState *combat, float value)
{
    BtlBakugan *owner;

    combat->maxHp = value;
    owner = (BtlBakugan *)combat->owner;
    combat->hp = value;
    if (owner != NULL && owner->hpGauge != NULL) {
        UiHpGaugeSetValues(combat->maxHp, (UiHpGauge *)owner->hpGauge);
    }
}

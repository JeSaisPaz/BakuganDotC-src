// bdc 0x08887c9c BtlCombatGetHpRatio
#include "bdc.h"

/* Returns the unit's hit-point fraction `hp / maxHp` (0.0..1.0) from its `BtlCombatState`:
   `BtlCombatGetHp` divided by `BtlCombatGetMaxHp` converted as an unsigned int to float. There
   is no zero guard, so a state without stat table yields 0/0. */
float BtlCombatGetHpRatio(BtlCombatState *combat)
{
    float hp = BtlCombatGetHp(combat);
    u32 maxHp = (u32)BtlCombatGetMaxHp(combat);
    return hp / (float)maxHp;
}

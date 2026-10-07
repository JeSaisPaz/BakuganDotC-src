// bdc 0x0889ad00 BtlUnitApplySpawnParams
#include "bdc.h"

/* Applies a four-float spawn record to a battle unit: fills its HP with `params[0]`
   (`BtlCombatFillHp` on the embedded `combat`), then sets the attack level, defense level and AI
   targeting style to `params[1..3]` truncated to int (`BtlBakuganSetAttackLevel`,
   `BtlBakuganSetDefenseLevel`, `BtlBakuganSetAiTargetStyle`). Used when an egg crystal hatches
   (`ActorStageObjEggCrystalHatch`). */
void BtlUnitApplySpawnParams(BtlBakugan *unit, float *params)
{
    BtlCombatFillHp(&unit->combat, params[0]);
    BtlBakuganSetAttackLevel(unit, (s32)params[1]);
    BtlBakuganSetDefenseLevel(unit, (s32)params[2]);
    BtlBakuganSetAiTargetStyle(unit, (s32)params[3]);
}

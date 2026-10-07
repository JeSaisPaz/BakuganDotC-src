// bdc 0x0887795c BtlAttackLaunchStage
#include "bdc.h"

/* Launches the attack `self` with `BtlAttackLaunch` (same `pos`, `dir`, `def`) on the battle's
   unit effect manager `g_btlUnitEffectMgr`, then sets `useStageEffects = 1`. The stage-effect
   variant of `BtlAttackLaunchAuto`. */
void BtlAttackLaunchStage(BtlAttack *self, float *pos, float *dir, s16 *def)
{
    BtlAttackLaunch(self, pos, dir, def, g_btlUnitEffectMgr);
    self->useStageEffects = 1;
}

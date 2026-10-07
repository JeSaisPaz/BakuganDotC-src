// bdc 0x088778c4 BtlAttackLaunchAuto
#include "bdc.h"

/* Launches attack `attack` from definition `def` with BtlAttackLaunch,
   picking the effect manager by the definition's type: the battle unit effect
   manager when the type uses stage effects (then useStageEffects = 1), else
   the attack effect manager (useStageEffects = 0). */
void BtlAttackLaunchAuto(void *attack, float *pos, float *dir, s16 *def)
{
    BtlAttack *self = (BtlAttack *)attack;

    if (BtlAttackDefUsesStageEffects(self, def) == 0) {
        BtlAttackLaunch(self, pos, dir, def, g_btlAttackEffectMgr);
        self->useStageEffects = 0;
    } else {
        BtlAttackLaunch(self, pos, dir, def, g_btlUnitEffectMgr);
        self->useStageEffects = 1;
    }
}

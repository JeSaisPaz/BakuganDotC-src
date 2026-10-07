// bdc 0x088840ac BtlAttackType83Update
#include "bdc.h"

/* Per-frame handler of attack type 0x83 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a simple projectile; calls `BtlAttackUpdateProjectile` with speed 45, max speed 80, hit kind
   0xa6, hit effect 0x55 on the unit effect manager `g_btlUnitEffectMgr`, trail effect 0xe2,
   lifetime 40 frames, sound 0x200096 and both flags 0. */
void BtlAttackType83Update(BtlAttack *self)
{
    BtlAttackUpdateProjectile(45.0f, 80.0f, self, 0xa6, 0x55, g_btlUnitEffectMgr, 0xe2, 40, 0,
                              0x200096, 0);
}

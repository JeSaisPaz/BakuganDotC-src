// bdc 0x08884348 BtlAttackType4EUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x4e (`g_btlAttackTypeHandlers`, run by `BtlAttackUpdate`):
   a simple projectile; calls `BtlAttackUpdateProjectile` with speed 40, max speed 80, hit kind
   0x71, hit effect 0x55 on the unit effect manager `g_btlUnitEffectMgr`, trail effect 0xf0,
   lifetime 60 frames, sound 0x200096, `flagA` 0 and `flagB` 1. */
void BtlAttackType4EUpdate(BtlAttack *self)
{
    BtlAttackUpdateProjectile(40.0f, 80.0f, self, 0x71, 0x55, g_btlUnitEffectMgr, 0xf0, 60, 0,
                              0x200096, 1);
}

// bdc 0x088841a4 BtlAttackType4AUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x4a (`g_btlAttackTypeHandlers`, run by `BtlAttackUpdate`):
   a simple projectile; calls `BtlAttackUpdateProjectile` with speed 40, max speed 80, hit kind
   0x6d, hit effect 0x45 on the unit effect manager `g_btlUnitEffectMgr`, trail effect 0xf0,
   lifetime 60 frames, sound 0x200096, `flagA` 1 and `flagB` 1. */
void BtlAttackType4AUpdate(BtlAttack *self)
{
    BtlAttackUpdateProjectile(40.0f, 80.0f, self, 0x6d, 0x45, g_btlUnitEffectMgr, 0xf0, 60, 1,
                              0x200096, 1);
}

// bdc 0x088842f4 BtlAttackType4FUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x4f (`g_btlAttackTypeHandlers`, run by `BtlAttackUpdate`):
   a simple projectile; calls `BtlAttackUpdateProjectile` with speed 35, max speed 80, hit kind
   0x72, hit effect 0x55 on the unit effect manager `g_btlUnitEffectMgr`, trail effect 0xf0,
   lifetime 60 frames, sound 0x200096, `flagA` 0 and `flagB` 1. */
void BtlAttackType4FUpdate(BtlAttack *self)
{
    BtlAttackUpdateProjectile(35.0f, 80.0f, self, 0x72, 0x55, g_btlUnitEffectMgr, 0xf0, 60, 0,
                              0x200096, 1);
}

// bdc 0x088842a0 BtlAttackType4DUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x4d (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a simple projectile, calls `BtlAttackUpdateProjectile` with speed 25, max speed 80, hit kind
   0x70, hit effect 0x51 on `g_btlUnitEffectMgr`, trail effect 0xf0, lifetime 60 frames, flagA 0,
   sound 0x200096, flagB 1. */

void BtlAttackType4DUpdate(BtlAttack *self)
{
    BtlAttackUpdateProjectile(25.0f, 80.0f, self, 0x70, 0x51, g_btlUnitEffectMgr, 0xf0, 60, 0,
                              0x200096, 1);
}

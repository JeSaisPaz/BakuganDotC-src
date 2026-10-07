// bdc 0x088845bc BtlAttackType53Update
#include "bdc.h"

/* Per-frame handler of attack type 0x53 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a simple projectile, calls `BtlAttackUpdateProjectile` with speed 40, max speed 80, hit kind
   0x76, hit effect 0x51 on `g_btlUnitEffectMgr`, trail effect -1 (none), lifetime 60 frames,
   flagA 0, sound 0x200096, flagB 1. */

void BtlAttackType53Update(BtlAttack *self)
{
    BtlAttackUpdateProjectile(40.0f, 80.0f, self, 0x76, 0x51, g_btlUnitEffectMgr, -1, 60, 0,
                              0x200096, 1);
}

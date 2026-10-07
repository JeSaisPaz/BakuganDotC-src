// bdc 0x088840fc BtlAttackType47Update
#include "bdc.h"

/* Per-frame handler of attack type 0x47 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a simple projectile, calls `BtlAttackUpdateProjectile` with speed 40, max speed 80, hit kind
   0x6a, hit effect 0x38 on `g_btlUnitEffectMgr`, trail effect 0xf0, lifetime 60 frames, flagA 1,
   sound 0x200096, flagB 1. */

void BtlAttackType47Update(BtlAttack *self)
{
    BtlAttackUpdateProjectile(40.0f, 80.0f, self, 0x6a, 0x38, g_btlUnitEffectMgr, 0xf0, 60, 1,
                              0x200096, 1);
}

// bdc 0x08884760 BtlAttackType81Update
#include "bdc.h"

/* Per-frame handler of attack type 0x81 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   simple projectile, calls `BtlAttackUpdateProjectile` with speed 50, max speed 80, hit kind
   0xa4, hit effect 0x1a on `g_btlAttackEffectMgr`, trail effect none (-1), lifetime 60 frames,
   flagA 0, sound 0x200096, flagB 1. */

void BtlAttackType81Update(BtlAttack *self)
{
    BtlAttackUpdateProjectile(50.0f, 80.0f, self, 0xa4, 0x1a, g_btlAttackEffectMgr, -1, 60, 0,
                              0x200096, 1);
}

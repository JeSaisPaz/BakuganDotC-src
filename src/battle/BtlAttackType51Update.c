// bdc 0x088843f0 BtlAttackType51Update
#include "bdc.h"

/* Per-frame handler of attack type 0x51 (run by `BtlAttackUpdate` from its handler table): a
   simple projectile; calls `BtlAttackUpdateProjectile` with speed and max speed 80, hit kind
   0x74, hit effect 0x38 on the unit effect manager `g_btlUnitEffectMgr`, trail effect 0xf0,
   lifetime 60 frames, sound 0x200096, flagA 0 and flagB 1. */
void BtlAttackType51Update(BtlAttack *self)
{
    BtlAttackUpdateProjectile(80.0f, 80.0f, self, 0x74, 0x38, g_btlUnitEffectMgr, 0xf0, 60, 0,
                              0x200096, 1);
}

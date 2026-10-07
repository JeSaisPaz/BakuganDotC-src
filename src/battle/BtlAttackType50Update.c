// bdc 0x0888439c BtlAttackType50Update
#include "bdc.h"

/* Per-frame handler of attack type 0x50 (`g_btlAttackTypeHandlers`, run by `BtlAttackUpdate`):
   a simple projectile; calls `BtlAttackUpdateProjectile` with speed 40, max speed 80, hit kind
   0x73, hit effect 0x38 on the unit effect manager `g_btlUnitEffectMgr`, trail effect 0xf0,
   lifetime 60 frames, sound 0x200096, `flagA` 0 and `flagB` 1. */
void BtlAttackType50Update(BtlAttack *self)
{
    BtlAttackUpdateProjectile(40.0f, 80.0f, self, 0x73, 0x38, g_btlUnitEffectMgr, 0xf0, 60, 0,
                              0x200096, 1);
}

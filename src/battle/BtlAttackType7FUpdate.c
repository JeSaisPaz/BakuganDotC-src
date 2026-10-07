// bdc 0x08883be0 BtlAttackType7FUpdate
#include "bdc.h"

/* Per-frame handler of attack types 0x7f and 0x82 (both entries of `g_btlAttackTypeHandlers`,
   run by `BtlAttackUpdate`): a simple projectile; calls `BtlAttackUpdateProjectile` with speed
   `param0` (`+0xfc`, converted int → float), max speed 80, hit kind 0xa2, hit effect 0x55 on the
   unit effect manager `g_btlUnitEffectMgr`, trail effect 0xe2, lifetime `param1` (`+0x100`)
   frames, sound 0x200096 and both flags 0. */
void BtlAttackType7FUpdate(BtlAttack *self)
{
    BtlAttackUpdateProjectile((float)self->param0, 80.0f, self, 0xa2, 0x55, g_btlUnitEffectMgr, 0xe2,
                              self->param1, 0, 0x200096, 0);
}

// bdc 0x08883b8c BtlAttackType7EUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x7e (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   simple projectile; calls `BtlAttackUpdateProjectile` with speed `param0` (converted from int to
   float), max speed 80, hit kind 0xa1, hit effect 0x55 on the unit effect manager
   `g_btlUnitEffectMgr`, no trail effect (-1), lifetime `param1` frames, sound 0x200096, flagA 1
   and flagB 0. */
void BtlAttackType7EUpdate(BtlAttack *self)
{
    BtlAttackUpdateProjectile((float)self->param0, 80.0f, self, 0xa1, 0x55, g_btlUnitEffectMgr, -1,
                              self->param1, 1, 0x200096, 0);
}

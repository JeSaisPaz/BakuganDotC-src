// bdc 0x0887af9c BtlAttackType03Update
#include "bdc.h"

/* Per-frame handler of attack type 0x3 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a guided projectile; runs `BtlAttackUpdateGuided` with turn rate 0.3, speed 0, hit kind 0x26,
   hit effect 0x25 and cancel effect 0x1b, ignoring its result. */
void BtlAttackType03Update(BtlAttack *self)
{
    BtlAttackUpdateGuided(0.300000012f, 0.0f, self, 0x26, 0x25, 0x1b);
}

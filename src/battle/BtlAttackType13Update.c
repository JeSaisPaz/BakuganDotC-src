// bdc 0x0887ce5c BtlAttackType13Update
#include "bdc.h"

/* Per-frame handler of attack type 0x13 (run by `BtlAttackUpdate` from its handler table): runs
   the shared guided-projectile update `BtlAttackUpdateGuided` with turn rate 0.1, speed -300,
   hit kind 0x36, hit effect 0x25 and cancel effect 0x1b; its result is ignored. */
void BtlAttackType13Update(BtlAttack *self)
{
    BtlAttackUpdateGuided(0.100000001f, -300.0f, self, 0x36, 0x25, 0x1b);
}

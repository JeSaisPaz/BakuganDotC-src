// bdc 0x0887afd0 BtlAttackType0FUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0xf (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   calls `BtlAttackUpdateGuided``(turn 0.3, speed paramF2, attack, hit kind 0x32, hit effect
   0x27, cancel effect 0x27)` and ignores its result. */

void BtlAttackType0FUpdate(BtlAttack *self)
{
    BtlAttackUpdateGuided(0.300000012f, self->paramF2, self, 0x32, 0x27, 0x27);
}

// bdc 0x0887de44 BtlAttackType1EUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x1e (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   calls `BtlAttackUpdateDecelHomingOrb``(attack, 0x41, 0xbd)`. */

void BtlAttackType1EUpdate(BtlAttack *self)
{
    BtlAttackUpdateDecelHomingOrb(self, 0x41, 0xbd);
}

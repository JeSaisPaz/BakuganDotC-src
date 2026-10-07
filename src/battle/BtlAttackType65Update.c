// bdc 0x08881790 BtlAttackType65Update
#include "bdc.h"

/* Per-frame handler of attack type 0x65 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   calls `BtlAttackUpdateArcShot``(attack, 0x88)`. */
void BtlAttackType65Update(BtlAttack *self)
{
    BtlAttackUpdateArcShot(self, 0x88);
}

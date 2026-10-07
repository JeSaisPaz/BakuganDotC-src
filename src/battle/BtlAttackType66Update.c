// bdc 0x088817ac BtlAttackType66Update
#include "bdc.h"

/* Per-frame handler of attack type 0x66 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared slowing homing orb `BtlAttackUpdateDecelHomingOrb` with hit id 0x89 and burst
   effect 0xbd. */
void BtlAttackType66Update(BtlAttack *self)
{
    BtlAttackUpdateDecelHomingOrb(self, 0x89, 0xbd);
}

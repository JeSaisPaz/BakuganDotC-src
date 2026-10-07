// bdc 0x0887ae48 BtlAttackType01Update
#include "bdc.h"

/* Per-frame handler of attack type 0x1 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared beam `BtlAttackUpdateBeam` with width 6.5, effect 0x20 and the ids 0x21,
   0x22, 0x24, 0x28. */
void BtlAttackType01Update(BtlAttack *self)
{
    BtlAttackUpdateBeam(6.5f, self, 0x20, 0x21, 0x22, 0x24, 0x28);
}

// bdc 0x08880394 BtlAttackType29Update
#include "bdc.h"

/* Per-frame handler of attack type 0x29 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared beam `BtlAttackUpdateBeam` with width 6.5, effect 0xaa and the ids 0xab,
   0xac, 0x4c, 0x20. */
void BtlAttackType29Update(BtlAttack *self)
{
    BtlAttackUpdateBeam(6.5f, self, 0xaa, 0xab, 0xac, 0x4c, 0x20);
}

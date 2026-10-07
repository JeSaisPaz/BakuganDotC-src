// bdc 0x08880fb4 BtlAttackType59Update
#include "bdc.h"

/* Per-frame handler of attack type 0x59 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared beam update `BtlAttackUpdateBeam` with width 5.0, effect 0x20 and the
   arguments 0x21, 0x22, 0x7c, 0x28. */
void BtlAttackType59Update(BtlAttack *self)
{
    BtlAttackUpdateBeam(5.0f, self, 0x20, 0x21, 0x22, 0x7c, 0x28);
}

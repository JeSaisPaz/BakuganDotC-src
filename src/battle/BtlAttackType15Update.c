// bdc 0x0887cecc BtlAttackType15Update
#include "bdc.h"

/* Per-frame handler of attack type 0x15 (run by `BtlAttackUpdate` from its handler table): runs
   the shared beam update `BtlAttackUpdateBeam` with width 6.5, effect id 0xaa and the
   type-specific arguments 0xab, 0xac, 0x38, 0x28. */
void BtlAttackType15Update(BtlAttack *self)
{
    BtlAttackUpdateBeam(6.5f, self, 0xaa, 0xab, 0xac, 0x38, 0x28);
}

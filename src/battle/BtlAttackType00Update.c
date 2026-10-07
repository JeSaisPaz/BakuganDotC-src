// bdc 0x0887a168 BtlAttackType00Update
#include "bdc.h"

/* Per-frame handler of attack type 0x0 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared homing shot `BtlAttackUpdateHomingShotA` with hit id 0x23. */
void BtlAttackType00Update(BtlAttack *self)
{
    BtlAttackUpdateHomingShotA(self, 0x23);
}

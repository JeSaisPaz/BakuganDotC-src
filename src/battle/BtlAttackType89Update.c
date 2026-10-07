// bdc 0x08882c80 BtlAttackType89Update
#include "bdc.h"

/* Per-frame handler of attack type 0x89 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared homing shot `BtlAttackUpdateHomingShotA` with hit id 0xac. */
void BtlAttackType89Update(BtlAttack *self)
{
    BtlAttackUpdateHomingShotA(self, 0xac);
}

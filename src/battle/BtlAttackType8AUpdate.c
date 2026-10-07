// bdc 0x08882c9c BtlAttackType8AUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x8a (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared homing-shot update `BtlAttackUpdateHomingShotB` with argument 0xad. */
void BtlAttackType8AUpdate(BtlAttack *self)
{
    BtlAttackUpdateHomingShotB(self, 0xad);
}

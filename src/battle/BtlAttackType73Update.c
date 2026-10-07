// bdc 0x088822bc BtlAttackType73Update
#include "bdc.h"

/* Per-frame handler of attack type 0x73 (handler table run by `BtlAttackUpdate`): a guided
   projectile, `BtlAttackUpdateGuided` with turn rate 1.0, speed `paramF2`, hit kind 0x96, hit
   effect 0xff and cancel effect 0x1b. */
void BtlAttackType73Update(BtlAttack *self)
{
    BtlAttackUpdateGuided(1.0f, self->paramF2, self, 0x96, 0xff, 0x1b);
}

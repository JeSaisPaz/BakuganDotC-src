// bdc 0x088664bc BtlBakuganCanUseLinkedObject
#include "bdc.h"

/* Returns 1 when the unit has energy (`BtlCombatHasEnergy`) and its `attacker` unit exists with
   `counterOpening` set, else 0. Only caller: `BtlHudUpdateLinkIcon`. */

int BtlBakuganCanUseLinkedObject(BtlBakugan *self)

{
  if (BtlCombatHasEnergy(&self->combat) != 0 && self->attacker != NULL) {
    return self->attacker->counterOpening != 0;
  }
  return 0;
}

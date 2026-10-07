// bdc 0x088a4168 ActorStageObjEggCrystalOwnerAlive
#include "bdc.h"

/* Returns 1 while the egg crystal's owner unit `+0x324` is in `g_btlBakuganList` and not defeated
   (`+0x4c1` clear), else 0. */

int ActorStageObjEggCrystalOwnerAlive(ActorStageObjEggCrystal *self)

{
  BtlBakugan *unit;

  unit = (BtlBakugan *)BtlBakuganListFind((BtlBakugan *)self->owner);
  if (unit != NULL && (unit->combat).dead == 0) {
    return 1;
  }
  return 0;
}


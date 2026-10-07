// bdc 0x088a4068 ActorStageObjEggCrystalUnlinkOwner
#include "bdc.h"

/* Clears the egg crystal's slot `owner+0x7f0[index]` in its owner unit `+0x324` (if still in
   `g_btlBakuganList`). Called by `ActorStageObjEggCrystalDtor`. */

void ActorStageObjEggCrystalUnlinkOwner(ActorStageObjEggCrystal *self)

{
  BtlCrystalOwnerUnit *unit;
  
  unit = (BtlCrystalOwnerUnit *)BtlBakuganListFind(self->owner);
  if (unit != NULL) {
    unit->crystalSlots[self->ownerIndex] = NULL;
  }
  return;
}


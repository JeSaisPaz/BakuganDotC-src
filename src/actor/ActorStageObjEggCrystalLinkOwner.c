// bdc 0x088a3cc8 ActorStageObjEggCrystalLinkOwner
#include "bdc.h"

/* Registers the egg crystal in its owner unit's crystal slots: `owner+0x7f0[index] = obj` with
   owner `+0x324` (if still in `g_btlBakuganList`) and index `+0x340`. Counterpart of
   `ActorStageObjEggCrystalUnlinkOwner`. */

void ActorStageObjEggCrystalLinkOwner(ActorStageObjEggCrystal *self)

{
  BtlCrystalOwnerUnit *unit;
  
  unit = (BtlCrystalOwnerUnit *)BtlBakuganListFind(self->owner);
  if (unit != NULL) {
    unit->crystalSlots[self->ownerIndex] = self;
  }
  return;
}


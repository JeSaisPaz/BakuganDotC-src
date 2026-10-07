// bdc 0x088a50c0 ActorStageObjEggCrystalState02Hatch
#include "bdc.h"

/* State 2 of the egg crystal. Step 0: returns at once while the owner is not in the battle Bakugan
   chain (`BtlBakuganListFind`); otherwise sets `timer` to a random 200..229 + 60*ownerIndex
   frames (0 when script global flag 5 is set) and advances. Step 1: while flag 5 is set, `unitFlag`
   is nonzero and `BtlAnyUnitHasEntrySequence` reports a pending entry, waits without counting;
   otherwise counts `timer` down and advances when it reaches 0. Step 2: tries to hatch
   (`ActorStageObjEggCrystalHatch`); on failure (too many units) goes back to step 1 with
   `timer` = 60. On success, or for any other step, returns to state 0 and sets `dead`. */

void ActorStageObjEggCrystalState02Hatch(ActorStageObjEggCrystal *self)
{
  int timer;
  u32 rnd;

  switch (self->step) {
  case 0:
    if (BtlBakuganListFind(self->owner) == NULL) {
      return;
    }
    timer = 0;
    if (!CoreBitsetTest(5, g_scriptGlobalBits)) {
      rnd = PlatformRandU32();
      timer = (int)(((rnd >> 16) * 30) >> 16) + self->ownerIndex * 60 + 200;
    }
    self->timer = timer;
    self->step = self->step + 1;
    /* fallthrough */
  case 1:
    if (CoreBitsetTest(5, g_scriptGlobalBits) && self->unitFlag != 0 &&
        BtlAnyUnitHasEntrySequence() != 0) {
      return;
    }
    self->timer = self->timer - 1;
    if (self->timer > 0) {
      return;
    }
    self->step = self->step + 1;
    /* fallthrough */
  case 2:
    if (ActorStageObjEggCrystalHatch(self) == 0) {
      self->step = 1;
      self->timer = 60;
      return;
    }
    self->step = self->step + 1;
    break;
  default:
    break;
  }
  ActorStageObjEggCrystalSetState(self, 0);
  self->base.dead = 1;
}

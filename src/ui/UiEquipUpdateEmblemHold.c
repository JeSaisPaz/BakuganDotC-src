// bdc 0x0895cc70 UiEquipUpdateEmblemHold
#include "bdc.h"

/* Counts down the emblem hold armed by `UiEquipStartEmblemHold` on the UiEquip Bakugan/gear
   loadout screen (task 302, `UiEquipCtor`); returns true once the counter is 0. */

bool UiEquipUpdateEmblemHold(UiEquip *self)

{
  UiPulse *hold = (UiPulse *)&self->tweens[self->spriteIdx[0xb]];

  if (hold->waitFrames != 0) {
    hold->waitFrames = hold->waitFrames - 1;
    return false;
  }
  return true;
}


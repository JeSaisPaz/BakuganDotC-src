// bdc 0x0895ee60 UiEquipResetRandomPick
#include "bdc.h"

/* Clears the 0x20-byte random-pick state at `+0x4f90` (step byte `+0x4f90`, candidate count
   `+0x4f94`, list `+0x4f9c`) of the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`)
   before `UiEquipRandomPick`. */

void UiEquipResetRandomPick(UiEquip *self)

{
  memset(&self->rouletteStep,0,0x20);
  return;
}


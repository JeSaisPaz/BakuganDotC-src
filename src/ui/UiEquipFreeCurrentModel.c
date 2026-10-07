// bdc 0x0895e510 UiEquipFreeCurrentModel
#include "bdc.h"

/* Frees the current player's Bakugan model (`UiEquipFreeSlotModel` with `+0x4cdb`) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`) and clears the model-swap delay
   `+0x4f78`. */

void UiEquipFreeCurrentModel(UiEquip *self)

{
  UiEquipFreeSlotModel(self,self->editPlayer);
  self->modelSwapDelay = '\0';
  return;
}


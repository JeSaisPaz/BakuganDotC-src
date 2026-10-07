// bdc 0x0895bc18 UiEquipFreeSlotModel
#include "bdc.h"

/* Releases the Bakugan model of player slot `slot` (`+0x4d08[slot]`, loaded by
   `UiEquipLoadSlotModel`) on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`)
   through `CoreObjectDeferDelete` and clears the pointer. */

void UiEquipFreeSlotModel(UiEquip *self, u8 slot)

{
  if (self->bakuganModels[slot] != (GfxModel *)0x0) {
    CoreObjectDeferDelete(&self->bakuganModels[slot]->base,0);
    self->bakuganModels[slot] = (GfxModel *)0x0;
  }
  return;
}


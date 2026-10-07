// bdc 0x0895bc58 UiEquipFreePedestalModels
#include "bdc.h"

/* Releases the four `"menu_daiza.gmo"` pedestal models (`+0x4d18[0..3]`, created per player by
   `UiEquipLoadPedestalModel`) of the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) through
   `CoreObjectDeferDelete` and clears the pointers. */

void UiEquipFreePedestalModels(UiEquip *self)
{
  s32 i;

  for (i = 0; i < 4; i++) {
    if (self->pedestalModels[i] != NULL) {
      CoreObjectDeferDelete(&self->pedestalModels[i]->base, 0);
      self->pedestalModels[i] = NULL;
    }
  }
}

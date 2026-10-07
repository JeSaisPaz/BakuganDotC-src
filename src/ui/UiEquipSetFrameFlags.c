// bdc 0x0895cca4 UiEquipSetFrameFlags
#include "bdc.h"

/* Sets (`set`) or clears the bits `mask` of the per-frame flag byte `+0x4ce1` of the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`). */

void UiEquipSetFrameFlags(UiEquip *self, bool set, u8 mask)

{
  if (set) {
    self->animFlags = self->animFlags | mask;
    return;
  }
  self->animFlags = self->animFlags & ~mask;
  return;
}


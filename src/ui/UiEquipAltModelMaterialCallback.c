// bdc 0x08956408 UiEquipAltModelMaterialCallback
#include "bdc.h"

/* Material callback of the pedestal Bakugan model of the loadout screen (`UiEquipLoadSlotModel`,
   run via `GfxModelForEachMaterial` when `UiEquipHasAltMotion` is true): when the 2-bit field
   in bits 0–1 of material flag byte `+3` is non-zero, inverts it (1↔2, 3→0). */

void UiEquipAltModelMaterialCallback(u8 *material)

{
  u8 flags;
  
  flags = material[3];
  if ((flags & 3) != 0) {
    material[3] = (flags & 0xfc) | ((flags & 3) ^ 3);
  }
  return;
}


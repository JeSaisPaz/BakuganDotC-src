// bdc 0x0895b838 UiEquipIsMirroredSlot
#include "bdc.h"

/* Returns true for the odd (right-hand, mirrored) player slots of the UiEquip Bakugan/gear loadout
   screen (task 302, `UiEquipCtor`) (`slot & 1`). */

bool UiEquipIsMirroredSlot(UiEquip *self, u8 slot)

{
  return (slot & 1) != 0;
}


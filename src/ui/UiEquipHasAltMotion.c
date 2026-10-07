// bdc 0x08956e50 UiEquipHasAltMotion
#include "bdc.h"

/* Returns 1 when Bakugan `id` has a nonzero entry in the 22-byte flag table `g_equipAltMotionFlags`
   and `variant` bit 0 is set, else 0. */

s32 UiEquipHasAltMotion(UiEquip *self, u32 variant, u8 id)
{
  u8 flags[24];

  memcpy(flags, &g_equipAltMotionFlags, 0x16);
  if (flags[id] != 0 && (variant & 1) != 0) {
    return 1;
  }
  return 0;
}

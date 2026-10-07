// bdc 0x089b1b98 SaveProfileClearPlacedHolograms
#include "bdc.h"

/* Resets the hologram placement of the player profile: clears the four placed-hologram bytes
   `*profile + 0x84..0x87`, the pending-points word 0x2d and the bit word 0x30
   (`SaveProfileSetWord`). */

void SaveProfileClearPlacedHolograms(void)
{
  SaveProfile *profile;
  u32 i;

  i = 0;
  do {
    profile = (SaveProfile *)SaveGetProfile();
    profile->data->placedHolograms[i & 0xff] = 0;
    i++;
  } while ((s32)i < 4);
  SaveProfileSetWord((SaveProfile *)SaveGetProfile(), 0x2d, 0);
  SaveProfileSetWord((SaveProfile *)SaveGetProfile(), 0x30, 0);
}

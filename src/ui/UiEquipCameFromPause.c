// bdc 0x08958294 UiEquipCameFromPause
#include "bdc.h"

/* Returns 1 when the previous screen (`0x08ac0e78`) was task 410 (0x19a, the pause screen), else 0.
    */

s32 UiEquipCameFromPause(UiEquip *self)

{
  if (g_lastScreenTaskId == 0x19a) {
    return 1;
  }
  return 0;
}


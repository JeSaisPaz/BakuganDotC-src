// bdc 0x0890f720 UiRepairGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the Repair screen (task id 430, vtable `0x08af48bc`): indices 0-2
   come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`); other indices return 0. */

u32 UiRepairGetField(UiScreen *screen, u32 index)

{
  u32 result = 0;

  if (index < 3) {
    return CoreTaskGetField(&screen->base, index);
  }
  if (index == 3) {
    result = screen->phase;
  }
  return result;
}

// bdc 0x089502d8 UiTitleMenuGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the title/system menu screen (task id 1000, vtable `0x08af4db4`):
   indices 0-2 come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`); other indices
   return 0. */

u32 UiTitleMenuGetField(UiScreen *screen, u32 index)
{
  u32 value = 0;

  if (index < 3) {
    value = CoreTaskGetField(&screen->base, index);
    return value;
  }
  if (index == 3) {
    value = screen->phase;
  }
  return value;
}

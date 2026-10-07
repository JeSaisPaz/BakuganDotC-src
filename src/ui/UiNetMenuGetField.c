// bdc 0x0894dd90 UiNetMenuGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the multiplayer (ad-hoc) top menu (task id 1999, vtable
   `0x08af4d7c`): indices 0-2 come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`);
   other indices return 0. */

u32 UiNetMenuGetField(UiScreen *screen, u32 index)

{
  u32 value = 0;

  if (index < 3) {
    return CoreTaskGetField(&screen->base, index);
  }
  if (index == 3) {
    value = screen->phase;
  }
  return value;
}


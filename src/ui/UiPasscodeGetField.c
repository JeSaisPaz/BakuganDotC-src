// bdc 0x0893d7e8 UiPasscodeGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the passcode (symbol sequence) puzzle screen (task id 374, vtable
   `0x08af4b5c`): indices 0-2 come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`);
   other indices return 0. */

u32 UiPasscodeGetField(UiScreen *screen, u32 index)
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

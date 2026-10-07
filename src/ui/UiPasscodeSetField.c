// bdc 0x0893d784 UiPasscodeSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the passcode (symbol sequence) puzzle screen (task id 374, vtable
   `0x08af4b5c`): indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase (`+0x28`) and
   resets `phaseStep` (`+0x2c`) when the value changes. */

void UiPasscodeSetField(UiScreen *screen, u32 index, u32 value)

{
  if (index < 3) {
    CoreTaskSetField(&screen->base,index,value);
    return;
  }
  if ((index == 3) && (screen->phase != value)) {
    screen->phase = value;
    screen->phaseStep = 0;
  }
  return;
}


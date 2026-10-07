// bdc 0x08950274 UiTitleMenuSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the title/system menu screen (task id 1000, vtable `0x08af4db4`):
   indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase (`+0x28`) and resets `phaseStep`
   (`+0x2c`) when the value changes. */

void UiTitleMenuSetField(UiScreen *screen, u32 index, u32 value)

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


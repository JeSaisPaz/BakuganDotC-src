// bdc 0x08940404 UiScreen390SetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of `UiScreen390` (terminal-use cutscene, task id
   390, vtable `g_uiScreen390Vtable`): indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase
   (`+0x28`) and resets `phaseStep` (`+0x2c`) when the value changes. */

void UiScreen390SetField(UiScreen *screen, u32 index, u32 value)

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


// bdc 0x0890f6bc UiRepairSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the Repair screen (task id 430, vtable `0x08af48bc`): indices 0-2
   go to `CoreTaskSetField`; index 3 sets the phase (`+0x28`) and resets `phaseStep` (`+0x2c`)
   when the value changes. */

void UiRepairSetField(UiScreen *screen, u32 index, u32 value)

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


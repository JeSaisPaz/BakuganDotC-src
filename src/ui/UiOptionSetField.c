// bdc 0x0896fe58 UiOptionSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the battle-options screen (`UiOption`): indices
   0-2 go to `CoreTaskSetField`; index 3 sets the screen phase (`+0x28`) and clears the phase
   step (`+0x2c`) when the value changes. Same code as `UiConfirmDialogSetField`. */

void UiOptionSetField(CoreTask *task, u32 index, u32 value)
{
  UiScreen *screen = (UiScreen *)task;

  if (index < 3) {
    CoreTaskSetField(task, index, value);
    return;
  }
  if ((index == 3) && (screen->phase != value)) {
    screen->phase = value;
    screen->phaseStep = 0;
  }
}

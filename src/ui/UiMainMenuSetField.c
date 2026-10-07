// bdc 0x089a3e4c UiMainMenuSetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogSetField` for `UiMainMenu` (same instructions
   up to relocated addresses). */

void UiMainMenuSetField(CoreTask *task, u32 index, u32 value)

{
  UiScreen *screen = (UiScreen *)task;

  if (index < 3) {
    CoreTaskSetField(task,index,value);
    return;
  }
  if ((index == 3) && ((u32)screen->phase != value)) {
    screen->phase = value;
    screen->phaseStep = 0;
  }
  return;
}

// bdc 0x089577c4 UiEquipSetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogSetField` for `UiEquip` (same instructions up
   to relocated addresses). */

void UiEquipSetField(CoreTask *task, u32 index, u32 value)
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

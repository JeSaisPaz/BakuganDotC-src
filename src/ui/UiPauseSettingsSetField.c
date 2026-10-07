// bdc 0x089aba04 UiPauseSettingsSetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogSetField` for `UiPauseSettings` (same
   instructions up to relocated addresses). */

void UiPauseSettingsSetField(CoreTask *task, u32 index, u32 value)

{
  if (index < 3) {
    CoreTaskSetField(task,index,value);
    return;
  }
  if ((index == 3) && (((UiScreen *)task)->phase != value)) {
    ((UiScreen *)task)->phase = value;
    ((UiScreen *)task)->phaseStep = 0;
  }
  return;
}


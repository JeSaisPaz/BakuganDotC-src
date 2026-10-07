// bdc 0x089a3eb0 UiMainMenuGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiMainMenu` (same instructions
   up to relocated addresses). */

u32 UiMainMenuGetField(CoreTask *task, u32 index)

{
  u32 result;

  result = 0;
  if (index < 3) {
    result = CoreTaskGetField(task,index);
    return result;
  }
  if (index == 3) {
    result = ((UiScreen *)task)->phase;
  }
  return result;
}


// bdc 0x08950c54 UiTitleGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiTitle` (same instructions up
   to relocated addresses). Index 3 returns the phase (`+0x28`). */

u32 UiTitleGetField(CoreTask *task, u32 index)
{
  u32 value = 0;

  if (index < 3) {
    value = CoreTaskGetField(task, index);
    return value;
  }
  if (index == 3) {
    value = ((UiScreen *)task)->phase;
  }
  return value;
}

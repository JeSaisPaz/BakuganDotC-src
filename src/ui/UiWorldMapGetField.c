// bdc 0x08997064 UiWorldMapGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiWorldMap` (same instructions
   up to relocated addresses). Index 3 returns the screen phase (`+0x28`). */

u32 UiWorldMapGetField(CoreTask *task, u32 index)
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

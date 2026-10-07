// bdc 0x089696b4 UiCardEquipGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiCardEquip` (same instructions
   up to relocated addresses). Index 3 returns the screen phase (`+0x28`). */

u32 UiCardEquipGetField(CoreTask *task, u32 index)
{
  u32 value = 0;

  if (index < 3) {
    return CoreTaskGetField(task, index);
  }
  if (index == 3) {
    value = ((UiScreen *)task)->phase;
  }
  return value;
}

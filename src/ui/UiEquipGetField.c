// bdc 0x08957828 UiEquipGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiEquip` (same instructions up
   to relocated addresses): indices 0-2 from `CoreTaskGetField`, 3 = screen phase (`+0x28`), else 0. */

u32 UiEquipGetField(CoreTask *task, u32 index)
{
  u32 result = 0;

  if (index < 3) {
    return CoreTaskGetField(task, index);
  }
  if (index == 3) {
    result = ((UiScreen *)task)->phase;
  }
  return result;
}

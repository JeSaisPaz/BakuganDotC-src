// bdc 0x0896febc UiOptionGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the battle-options screen (`UiOption`): indices
   0-2 come from `CoreTaskGetField`; index 3 returns the screen phase (`+0x28`); other indices
   return 0. Same code as `UiConfirmDialogGetField`. */

u32 UiOptionGetField(CoreTask *task, u32 index)
{
  if (index < 3) {
    return CoreTaskGetField(task, index);
  }
  if (index == 3) {
    return ((UiScreen *)task)->phase;
  }
  return 0;
}

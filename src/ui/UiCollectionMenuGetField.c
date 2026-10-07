// bdc 0x08973910 UiCollectionMenuGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiCollectionMenu` (same
   instructions up to relocated addresses); index 3 returns the screen phase (`+0x28`), indices >3 return 0. */

u32 UiCollectionMenuGetField(CoreTask *task, u32 index)

{
  u32 result;
  
  result = 0;
  if (index < 3) {
    result = CoreTaskGetField(task,index);
    return result;
  }
  if (index == 3) {
    result = (u32)((UiScreen *)task)->phase;
  }
  return result;
}


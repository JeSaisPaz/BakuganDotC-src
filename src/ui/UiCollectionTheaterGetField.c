// bdc 0x08987d28 UiCollectionTheaterGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiCollectionTheater` (same
   instructions up to relocated addresses). */

u32 UiCollectionTheaterGetField(CoreTask *task, u32 index)

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


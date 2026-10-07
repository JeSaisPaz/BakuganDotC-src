// bdc 0x0898b574 UiCollectionFigureGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiCollectionFigure` (same
   instructions up to relocated addresses). */

u32 UiCollectionFigureGetField(CoreTask *task, u32 index)

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


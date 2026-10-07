// bdc 0x089afe24 UiBattleModeSelectGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiBattleModeSelect` (same
   instructions up to relocated addresses). */

u32 UiBattleModeSelectGetField(UiBattleModeSelect *self, u32 index)

{
  u32 result;
  
  result = 0;
  if (index < 3) {
    result = CoreTaskGetField((CoreTask *)self,index);
    return result;
  }
  if (index == 3) {
    result = (self->base).phase;
  }
  return result;
}


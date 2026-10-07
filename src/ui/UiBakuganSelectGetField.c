// bdc 0x0892b6f8 UiBakuganSelectGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiBakuganSelect` (same
   instructions up to relocated addresses). */

u32 UiBakuganSelectGetField(UiBakuganSelect *self, u32 index)

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


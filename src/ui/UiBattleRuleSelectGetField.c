// bdc 0x089525d8 UiBattleRuleSelectGetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogGetField` for `UiBattleRuleSelect` (same
   instructions up to relocated addresses). */

u32 UiBattleRuleSelectGetField(UiBattleRuleSelect *self, u32 index)

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


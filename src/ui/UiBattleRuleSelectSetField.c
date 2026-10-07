// bdc 0x08952574 UiBattleRuleSelectSetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogSetField` for `UiBattleRuleSelect` (same
   instructions up to relocated addresses). */

void UiBattleRuleSelectSetField(UiBattleRuleSelect *self, u32 index, u32 value)

{
  if (index < 3) {
    CoreTaskSetField((CoreTask *)self,index,value);
    return;
  }
  if ((index == 3) && ((self->base).phase != value)) {
    (self->base).phase = value;
    (self->base).phaseStep = 0;
  }
  return;
}


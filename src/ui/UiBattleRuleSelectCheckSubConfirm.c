// bdc 0x089542c0 UiBattleRuleSelectCheckSubConfirm
#include "bdc.h"

/* Checks Cross in the sub-option window of `UiBattleRuleSelect`: 0 when
   not pressed, 1 when the focused option `+0xa44` is enabled (`+0x5fd + i`), 2 otherwise. */

s32 UiBattleRuleSelectCheckSubConfirm(UiBattleRuleSelect *self)
{
  s32 result;

  if ((self->base.pad->pressed & 0x4000) != 0) {
    result = 1;
    if (self->subEnabled[self->subCursor] == 0) {
      result = 2;
    }
    return result;
  }
  return 0;
}

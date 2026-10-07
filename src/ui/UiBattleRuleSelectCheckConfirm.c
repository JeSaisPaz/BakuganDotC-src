// bdc 0x08953628 UiBattleRuleSelectCheckConfirm
#include "bdc.h"

/* Checks Cross (pad `pressed` 0x4000) in `UiBattleRuleSelect`: 0 when not
   pressed, 1 when the focused rule `+0x74` is enabled, 2 when it is disabled. */

s32 UiBattleRuleSelectCheckConfirm(UiBattleRuleSelect *self)

{
  if ((((self->base).pad)->pressed & 0x4000) == 0) {
    return 0;
  }
  if (self->buttonEnabled[self->cursor] != '\0') {
    return 1;
  }
  return 2;
}


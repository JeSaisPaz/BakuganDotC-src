// bdc 0x089541b4 UiBattleRuleSelectMoveSubCursor
#include "bdc.h"

/* Up/Down (pad `repeat`) cycles the sub-option focus `subCursor` of
   `UiBattleRuleSelect` through 0–2; returns 1 when it moved. */

s32 UiBattleRuleSelectMoveSubCursor(UiBattleRuleSelect *self)
{
  PadState *pad = self->base.pad;

  if ((pad->repeat & 0x10) != 0) {
    self->subCursor = self->subCursor - 1;
    if (self->subCursor < 0) {
      self->subCursor = 2;
    }
    return 1;
  }
  if ((pad->repeat & 0x40) != 0) {
    self->subCursor = self->subCursor + 1;
    if (self->subCursor >= 3) {
      self->subCursor = 0;
    }
    return 1;
  }
  return 0;
}

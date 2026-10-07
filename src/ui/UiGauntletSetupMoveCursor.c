// bdc 0x0893523c UiGauntletSetupMoveCursor
#include "bdc.h"

/* D-pad navigation of `UiGauntletSetup` (pad `repeat` bits): on the card
   slots (`+0x74` = 0) Down moves to the OK button, Left/Right cycle the slot `+0x76` through 0–3
   (wrapping); on the OK button Up returns to the slots. Returns 1 when the focus changed, else 0.
    */

s32 UiGauntletSetupMoveCursor(UiGauntletSetup *self)
{
  PadState *pad = self->base.pad;

  if (self->focusArea == 0) {
    if ((pad->repeat & 0x40) != 0) {
      self->focusArea = 1;
      return 1;
    }
    if (((s8)pad->repeat & 0x80) != 0) {
      s8 v = 3;
      if (self->item != 0) {
        v = (s8)(self->item - 1);
      }
      self->item = v;
      return 1;
    }
    if ((pad->repeat & 0x20) != 0) {
      s8 v = 0;
      if (self->item != 3) {
        v = (s8)(self->item + 1);
      }
      self->item = v;
      return 1;
    }
  } else if ((pad->repeat & 0x10) != 0) {
    self->focusArea = 0;
    return 1;
  }
  return 0;
}

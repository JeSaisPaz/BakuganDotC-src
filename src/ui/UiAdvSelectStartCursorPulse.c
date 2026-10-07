// bdc 0x08919f08 UiAdvSelectStartCursorPulse
#include "bdc.h"

/* Starts the highlight pulse (`UiPulseStepTint`, period 40) of cursor sprite 0x11 of the adventure
   partner-select screen (`UiAdvSelectCtor`, task 376; cursor `+0x74`, candidates `+0x8a0` as
   4-byte `{id, partner, locked, ?}` slots) (record `+0x320`). */

void UiAdvSelectStartCursorPulse(UiAdvSelect *self)

{
  UiPulseStepTint(40.0f, ((GfxSprite **)self->base.data)[0x11], (UiPulse *)&self->tweens[0x11]);
  return;
}


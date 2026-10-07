// bdc 0x0892eb64 UiBakuganSelectStartCursorPulse
#include "bdc.h"

/* Starts the highlight pulse (`UiPulseStepTint`, period 40) of cursor sprite 0x19 of the Bakugan
   select screen (`UiBakuganSelectCtor`, task 371; cursor `+0x74`, current entry `+0x75`, owned
   list `+0x1ba4` with 0xc-byte entries) (record `+0x460`). */

void UiBakuganSelectStartCursorPulse(UiBakuganSelect *self)

{
  UiPulseStepTint(40.0f, ((GfxSprite **)self->base.data)[0x19], (UiPulse *)&self->tweens[0x19]);
  return;
}


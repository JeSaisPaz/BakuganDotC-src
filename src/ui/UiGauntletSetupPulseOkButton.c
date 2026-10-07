// bdc 0x08934e50 UiGauntletSetupPulseOkButton
#include "bdc.h"

/* While the OK button of `UiGauntletSetup` is focused (`+0x74` = 1), pulses
   the brightness of sprite 0x26 with the shared highlight pulse `UiCursorGlowStep`. */

void UiGauntletSetupPulseOkButton(UiGauntletSetup *self)

{
  if (self->focusArea == '\x01') {
    UiCursorGlowStep(((GfxSprite **)self->base.data)[0x26]);
  }
  return;
}


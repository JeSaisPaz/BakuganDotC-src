// bdc 0x08934dfc UiGauntletSetupPulseCursor
#include "bdc.h"

/* Per-frame pulse of the cursor of `UiGauntletSetup`: on the card slots
   (`+0x74` = 0) pulses sprite 0x12 with pulse state `+0x348`, on the OK button pulses sprite 0x27
   with `+0x690` (`UiPulseStepTint`, 40-frame period). */

void UiGauntletSetupPulseCursor(UiGauntletSetup *self)
{
  GfxSprite **spr = (GfxSprite **)self->base.data;

  if (self->focusArea == 0) {
    UiPulseStepTint(40.0f, spr[18], (UiPulse *)&self->tweens[18]);
    return;
  }
  UiPulseStepTint(40.0f, spr[39], (UiPulse *)&self->tweens[39]);
}

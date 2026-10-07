// bdc 0x089754a4 UiCollectionMenuPulseCursor
#include "bdc.h"

/* Runs the 40-frame pulse (`UiPulseStepTint`) of the cursor sprite 0x0c of
   `UiCollectionMenu`. */

void UiCollectionMenuPulseCursor(UiCollectionMenu *self)

{
  UiPulseStepTint(40.0f,((GfxSprite **)self->base.data)[0xc],(UiPulse *)&self->tweens[0xc]);
  return;
}


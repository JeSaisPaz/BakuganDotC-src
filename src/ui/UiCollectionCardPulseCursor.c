// bdc 0x089845a0 UiCollectionCardPulseCursor
#include "bdc.h"

/* Runs the 40-frame pulse (`UiPulseStepTint`) of the cursor sprite of
   `UiCollectionCard`. */

void UiCollectionCardPulseCursor(UiCollectionCard *self)

{
  UiPulseStepTint(40.0f,((GfxSprite **)self->base.data)[4],(UiPulse *)&self->tweens[4]);
  return;
}


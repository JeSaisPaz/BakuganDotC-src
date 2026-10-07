// bdc 0x0897b5a0 UiCollectionSpherePulseCursor
#include "bdc.h"

/* Runs the 40-frame pulse (`UiPulseStepTint`) of the cursor sprite of
   `UiCollectionSphere`. */

void UiCollectionSpherePulseCursor(UiCollectionSphere *self)

{
  UiPulseStepTint(40.0f,((GfxSprite **)self->base.data)[6],(UiPulse *)&self->tweens[6]);
  return;
}


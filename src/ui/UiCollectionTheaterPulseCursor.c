// bdc 0x08989930 UiCollectionTheaterPulseCursor
#include "bdc.h"

/* Runs the 40-frame pulse (`UiPulseStepTint`) of the cursor sprite of
   `UiCollectionTheater`. */

void UiCollectionTheaterPulseCursor(UiScreen *screen)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  UiPulseStepTint(40.0f, ((GfxSprite **)screen->data)[6], &self->cursorPulse);
}

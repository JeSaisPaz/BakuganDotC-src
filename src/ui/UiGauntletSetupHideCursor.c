// bdc 0x08934db8 UiGauntletSetupHideCursor
#include "bdc.h"

/* Hides the cursor sprites of `UiGauntletSetup` (sprites 0x12, 0x3b and 0x27:
   clears visibility flag 1 at `+0xd0`). */

void UiGauntletSetupHideCursor(UiGauntletSetup *self)

{
  GfxSprite **sprites = (GfxSprite **)(self->base).data;

  sprites[0x48 / 4]->flags &= ~1u;
  sprites[0xec / 4]->flags &= ~1u;
  sprites[0x9c / 4]->flags &= ~1u;
  return;
}

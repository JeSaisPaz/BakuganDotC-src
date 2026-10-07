// bdc 0x088c1c98 GameFieldPulseSpriteReset
#include "bdc.h"

/* Resets the pulse sprite `locationPulse` of the field task (id 500, `GameFieldCtor`): copies the
   location label sprite `locationLabel` into it (`GfxSpriteCopy`), moves the label's `posZ` back by
   1.0, centres the pulse sprite's pivot, rebuilds its matrix (`GfxSpriteResetMatrix`), insets its UV
   rect by 0.5 texels (`GfxSpriteInsetUv`) and clears `pulseTimer`. */

void GameFieldPulseSpriteReset(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;

  GfxSpriteCopy(field->locationLabel, field->locationPulse);
  field->locationLabel->posZ = field->locationLabel->posZ - 1.0f;
  GfxSpriteCenterPivot(field->locationPulse);
  GfxSpriteResetMatrix(field->locationPulse);
  GfxSpriteInsetUv(0.5f, field->locationPulse);
  field->pulseTimer = 0;
}

// bdc 0x088c22ec GameFieldHideLocationSprites
#include "bdc.h"

/* Hides the location label, marker and pulse sprites of the field task
   (id 500, `GameFieldCtor`) (clears bit 0 of their `flags`). */

void GameFieldHideLocationSprites(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;

  field->locationLabel->flags &= ~1u;
  field->locationMarker->flags &= ~1u;
  field->locationPulse->flags &= ~1u;
}

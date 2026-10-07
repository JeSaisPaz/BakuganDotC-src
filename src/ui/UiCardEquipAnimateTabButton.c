// bdc 0x0896c694 UiCardEquipAnimateTabButton
#include "bdc.h"

/* Animates the tab button sprite under the cursor of `UiCardEquip`
   (`UiCursorGlowStep`). */

void UiCardEquipAnimateTabButton(UiCardEquip *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  UiCursorGlowStep(sprites[self->groups[4][0] + self->rowCursor[self->row]]);
}

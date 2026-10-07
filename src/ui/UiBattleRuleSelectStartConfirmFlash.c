// bdc 0x08953668 UiBattleRuleSelectStartConfirmFlash
#include "bdc.h"

/* Starts the 2-frame confirm flash (`UiFlashStart`, channel 0) on rule button sprite 1+`cursor` of
   `UiBattleRuleSelect`. */

void UiBattleRuleSelectStartConfirmFlash(UiBattleRuleSelect *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  UiFlashStart(2.0f, sprites[self->cursor + 1], 0, 0);
}

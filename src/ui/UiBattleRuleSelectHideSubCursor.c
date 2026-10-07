// bdc 0x08954018 UiBattleRuleSelectHideSubCursor
#include "bdc.h"

/* Hides the sub-option cursor sprites 0x15 and 0x1c of
   `UiBattleRuleSelect`. */

void UiBattleRuleSelectHideSubCursor(UiBattleRuleSelect *self)
{
  GfxSprite **sprites;

  sprites = (GfxSprite **)self->base.data;
  sprites[0x15]->flags &= ~1u;
  sprites = (GfxSprite **)self->base.data;
  sprites[0x1c]->flags &= ~1u;
}

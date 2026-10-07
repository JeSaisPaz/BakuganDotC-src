// bdc 0x08953c08 UiBattleRuleSelectStartSubLabelTweens
#include "bdc.h"

/* Starts the pop tweens of the three sub-option labels (sprites 0x11–0x13, tweens `+0x320`) of
   `UiBattleRuleSelect`, making them visible with blend 2. */

void UiBattleRuleSelectStartSubLabelTweens(UiBattleRuleSelect *self, u8 closing)

{
  int i;

  for (i = 0x11; i < 0x14; i++) {
    GfxSprite *sprite = ((GfxSprite **)(self->base).data)[i];

    sprite->layerMask = 2;
    sprite = ((GfxSprite **)(self->base).data)[i];
    sprite->flags |= 1;
    UiTweenBegin(1.4f, closing, ((GfxSprite **)(self->base).data)[i], &self->tweens[i], 3);
  }
}

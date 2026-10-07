// bdc 0x089133f4 UiUpgradeTweenHeaderPanels
#include "bdc.h"

/* Starts the mode-3 tweens (`tweens[5..9]`) of sprites 5..9 of the Bakugan upgrade screen
   (`UiUpgradeCtor`, task 490): when showing (`hide == 0`) sets each sprite's flag bit 0 and
   fades it in, then sets the cells of sprites 6/7 to (0,2)/(0,1); when hiding runs the same tweens
   as fade-outs. */

void UiUpgradeTweenHeaderPanels(UiUpgrade *self, u8 hide)
{
  int i;

  if (hide == 0) {
    for (i = 5; i < 10; i++) {
      GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 1;
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[6], 0.0f, 2.0f);
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[7], 0.0f, 1.0f);
  }
  else {
    for (i = 5; i < 10; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}

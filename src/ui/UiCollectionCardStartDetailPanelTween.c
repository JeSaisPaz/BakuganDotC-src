// bdc 0x08984e94 UiCollectionCardStartDetailPanelTween
#include "bdc.h"

/* Starts the tween of the detail panel sprites (sprite slots 37-38 and 40-41 of the screen's
   sprite table, tweens 37-38 / 40-41) of the card collection screen (task 313,
   `maybe_UiScreen313Ctor`). When a card is opened (`out` = 0) each sprite is made visible
   (flag bit 0) on layer mask 4 and the first one is reset to 50 % grey, alpha 0; then
   UiTweenBegin (scale 1.0, flags 3) runs for every sprite, fading in or out per `out`. */

void UiCollectionCardStartDetailPanelTween(UiCollectionCard *self, u8 out)
{
  int i;

  if (out == 0) {
    for (i = 0x25; i < 0x27; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      if (i == 0x25) {
        GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
        sprite->tint[0] = 0.5f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 0.0f;
      }
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x28; i < 0x2a; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
  else {
    for (i = 0x25; i < 0x27; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x28; i < 0x2a; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}

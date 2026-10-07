// bdc 0x089857f0 UiCollectionCardStartAltPanelTween
#include "bdc.h"

/* Starts the tween of the second detail panel (sprites 0x36..0x37 and 0x3a..0x3b, tweens of the
   same index) of the card collection screen (task 313, `maybe_UiScreen313Ctor`; pages of ability
   cards `"collection_ability_%02d"` in `"waku_4_a"`/`"waku_4_b"` frames, large card art
   `"card_L_%03d"`, help text `"DWCardHelp"`) for the alternative detail view (sub-states 0xf/0x12
   of `UiCollectionCardMainPhase`). When `out` is 0 the sprites are first made visible on layer
   0x10: sprites 0x36/0x37 get button icons 4/5, sprites 0x3a/0x3b a white tint and alpha 0. */

void UiCollectionCardStartAltPanelTween(UiCollectionCard *self, u8 out)
{
  GfxSprite *sprite;
  s32 i;

  if (out == 0) {
    for (i = 0x36; i < 0x38; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i - 0x36 > 0) {
        if (i - 0x36 < 2) {
          UiSetButtonIcon(sprite, 5);
          sprite = ((GfxSprite **)self->base.data)[i];
        }
      } else if (i - 0x36 >= 0) {
        UiSetButtonIcon(sprite, 4);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->layerMask = 0x10;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x3a; i < 0x3c; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 0x10;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->tint[0] = 1.0f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 1.0f;
      sprite->alpha = 0.0f;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0x36; i < 0x38; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x3a; i < 0x3c; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}

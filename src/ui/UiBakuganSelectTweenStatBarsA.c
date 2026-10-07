// bdc 0x0892d9a0 UiBakuganSelectTweenStatBarsA
#include "bdc.h"

/* Starts the appear tweens (`UiTweenBegin`, flags 3) of the stat sprites 0x56 and 0x58 of the
   Bakugan select screen (`UiBakuganSelectCtor`, task 371): each is made visible (flags bit 0,
   layer mask 2) and set to cell row `display[2] + 5` of the cursor's owned-list entry; sprite 0x58
   also gets tint (1, 1, 0) and alpha 0. When `hide` is set, only starts their disappear tweens. */

void UiBakuganSelectTweenStatBarsA(UiBakuganSelect *self, u8 hide)
{
  GfxSprite *sprite;
  int i;

  if (hide == 0) {
    for (i = 0x56; i < 0x57; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f,
                       (float)((u16)self->entries[self->cursor].display[2] + 5));
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x58; i < 0x59; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f,
                       (float)((u16)self->entries[self->cursor].display[2] + 5));
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->tint[0] = 1.0f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 0.0f;
      sprite->alpha = 0.0f;
    }
  } else {
    for (i = 0x56; i < 0x57; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x58; i < 0x59; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}

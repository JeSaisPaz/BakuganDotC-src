// bdc 0x0892dd28 UiBakuganSelectTweenStatBarsB
#include "bdc.h"

/* Starts the appear tweens of the stat sprites 0x57 and 0x59 of the Bakugan select screen
   (`UiBakuganSelectCtor`): each is made visible (`flags` bit 0) on layer mask 2 and shows cell
   row `entries[cursor].display[3]` (`GfxSpriteSetCell`). When `hide` is set only the disappear
   tweens are started on the same sprites. */

void UiBakuganSelectTweenStatBarsB(UiBakuganSelect *self, u8 hide)
{
  int i;

  if (hide == 0) {
    for (i = 0x57; i < 0x58; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f,
                       (float)(u16)self->entries[self->cursor].display[3]);
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x59; i < 0x5a; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f,
                       (float)(u16)self->entries[self->cursor].display[3]);
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0x57; i < 0x58; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x59; i < 0x5a; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}

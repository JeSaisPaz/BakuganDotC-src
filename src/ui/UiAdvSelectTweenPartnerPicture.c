// bdc 0x08918d0c UiAdvSelectTweenPartnerPicture
#include "bdc.h"

/* Starts the fade tweens of the partner picture (sprite 3, shown as the shadow of the selected
   candidate's partner via `UiAdvSelectSetBakuganPicture`) and sprite 0x25 of the adventure
   partner-select screen (`UiAdvSelectCtor`, task 376; cursor `+0x74`, candidates `+0x8a0` as
   4-byte `{id, partner, locked, ?}` slots), or the reverse when `hide` is set. */

void UiAdvSelectTweenPartnerPicture(UiAdvSelect *self, u8 hide)
{
  int i;

  if (hide == 0) {
    for (i = 3; i < 4; i++) {
      UiAdvSelectSetBakuganPicture(self, ((GfxSprite **)self->base.data)[i],
                                   self->candidates[self->cursor].partner, false);
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
    for (i = 0x25; i < 0x26; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
  }
  else {
    for (i = 3; i < 4; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
    for (i = 0x25; i < 0x26; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
  }
}

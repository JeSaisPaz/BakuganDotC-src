// bdc 0x08983a20 UiCollectionCardStartFrameTween
#include "bdc.h"

/* Starts the zoom tween (start scale 1.0, flags 3) of the frame/title sprites 0x1b, 0x1d..0x1e and
   0x1c (records `self->tweens[i]`) of the card collection screen (task 313,
   `maybe_UiScreen313Ctor`; pages of ability cards `"collection_ability_%02d"` in
   `"waku_4_a"`/`"waku_4_b"` frames, large card art `"card_L_%03d"`, help text `"DWCardHelp"`).
   When appearing (`out == 0`) sprites 0x1b and 0x1c are made visible (`flags |= 1`) first and the
   page label is refreshed (`UiCollectionCardSetPageNumber`) after sprite 0x1b. The sprite table
   `base.data` is re-read for every access. */

void UiCollectionCardStartFrameTween(UiCollectionCard *self, u8 out)

{
  s32 i;

  if (out == 0) {
    for (i = 0x1b; i < 0x1c; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    UiCollectionCardSetPageNumber(self);
    for (i = 0x1d; i < 0x1f; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x1c; i < 0x1d; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
  else {
    for (i = 0x1b; i < 0x1c; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x1d; i < 0x1f; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x1c; i < 0x1d; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
  return;
}

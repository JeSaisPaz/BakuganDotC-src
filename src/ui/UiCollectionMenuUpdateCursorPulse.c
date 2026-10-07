// bdc 0x08975504 UiCollectionMenuUpdateCursorPulse
#include "bdc.h"

/* Grows the selected entry of `UiCollectionMenu` (collection top menu,
   task 311): `pulse` steps by 0.1 while below 1.0 and gives the scale 1 + 0.2*pulse, capped at
   1.2. Applies that scale to the entry button (sprite 0x0d + selection, depth -500), its label
   (sprite 3 + selection or 7 + selection on page 0, 3 + 3*page + selection otherwise, depth -501)
   and the cursor sprite 0x0c (depth -502). Selection is `(&selMain)[page]`. */

void UiCollectionMenuUpdateCursorPulse(UiCollectionMenu *self)

{
  float pulse;
  float scale;
  int page;
  int sel;
  GfxSprite *sprite;

  pulse = self->pulse;
  sprite = ((GfxSprite **)self->base.data)[13 + (&self->selMain)[self->page]];
  if (pulse < 1.0f) {
    pulse = pulse + 0.1f;
    self->pulse = pulse;
  }
  scale = pulse * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }
  UiSpriteSetScaleRotation(sprite, scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[13 + (&self->selMain)[self->page]]->posZ = -500.0f;

  page = self->page;
  sel = (&self->selMain)[page];
  if (page == 0) {
    if (sel < 3) {
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[3 + sel], scale, scale, 0.0f);
      ((GfxSprite **)self->base.data)[3 + self->page * 5 + (&self->selMain)[self->page]]->posZ =
          -501.0f;
    }
    else {
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[7 + sel], scale, scale, 0.0f);
      ((GfxSprite **)self->base.data)[7 + self->page * 5 + (&self->selMain)[self->page]]->posZ =
          -501.0f;
    }
  }
  else {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[3 + page * 3 + sel], scale, scale,
                             0.0f);
    ((GfxSprite **)self->base.data)[3 + self->page * 3 + (&self->selMain)[self->page]]->posZ =
        -501.0f;
  }
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[12], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[12]->posZ = -502.0f;
  return;
}
